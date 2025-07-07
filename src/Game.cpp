
#include "Game.h"

#include <iostream>

/*============= CONSTRUCTOR ============*/

Game::Game()
{

    num_enemies = 0;
}

/*============= INITIALIZE FUNCTIONS ============*/

void Game::initializeEnemies()
{
}

void Game::initializeSoundEngine()
{
    if (ma_engine_init(nullptr, &sound_engine) != MA_SUCCESS)
    {
        throw std::runtime_error("Failed to initialize sound engine");
    }
}

void Game::initializeCamera()
{
    camera = Camera(player.getPosition());
}

void Game::initializeShader()
{
    shader = Shader();
}

void Game::initializePlayer(const std::string &weapon_model_name, const std::string &projectile_objmodel_name,
                            const std::string &projectile_texture_name)
{
    player.setPosition(starting_position);
    player.setModel(nullptr);

    player.initializeWeapon(objects[weapon_model_name]);
    player.initializeProjectiles(obj_models[projectile_objmodel_name]);
    player.setProjectileTexture(textures[projectile_texture_name]);

    player.setSoundMap(&sounds);
}

/*============= ADD FUNCTIONS ============*/

void Game::addSound(const std::string &name, const std::string &file_path)
{
    ma_sound *sound = new ma_sound();
    if (ma_sound_init_from_file(&sound_engine, file_path.c_str(), 0, nullptr, nullptr, sound) != MA_SUCCESS)
    {
        throw std::runtime_error("Failed to load sound: " + file_path);
    }
    sounds[name] = sound;
}

void Game::addObjModel(const std::string &name, const std::string &file_path)
{
    ObjModel *model = new ObjModel(file_path.c_str());
    model->BuildTriangles();
    model->ComputeNormals();
    model->setName(name);

    obj_models[model->getName()] = model;
}

void Game::addSceneObject(const std::string &name, const std::string &obj_model_name, const std::string &texture_name,
                          int object_id, bool use_view_matrix, bool collidable)
{
    ObjModel *model = obj_models[obj_model_name];

    if (!model)
    {
        throw std::runtime_error("Model not found: " + obj_model_name);
    }

    SceneObject *scene_object = new SceneObject(model, name, shader, camera, use_view_matrix, collidable);

    if (texture_name != "") scene_object->setTexture(textures[texture_name]);
    scene_object->setID(object_id);
    virtual_scene.addObject(scene_object);
    objects[name] = scene_object;
}

void Game::addProjectile(Projectile *projectile)
{
    virtual_scene.addObject(projectile);
    projectiles[projectile->getName()] = projectile;
}

void Game::addEnemy(const glm::vec4 &position)
{
    const int enemy_id = 1;

    Enemy *enemy = new Enemy(obj_models["enemy_obj"], "enemy_" + std::to_string(num_enemies++),
                             shader, camera, position, 1.0f);

    enemy->setHeight(1.0f);
    enemy->setID(enemy_id);
    enemy->setProjectileModel(obj_models["projectile_obj"]);
    virtual_scene.addObject(enemy);
    enemies[enemy->getName()] = enemy;
}

void Game::addTexture(const std::string &name, const std::string &file_path)
{
    Texture3D *texture = new Texture3D();
    texture->LoadTextureImage(file_path.c_str());
    textures[name] = texture;
}

/*============= MOVE FUNCTIONS ============*/
void Game::moveEnemies()
{
    for (auto &pair : enemies)
    {
        Enemy *enemy = pair.second;
        enemy->move(objects, player.getPosition());
        enemy->checkCollisionsWithProjectiles(projectiles);
    }
}

void Game::moveProjectiles()
{
    for (auto &pair : projectiles)
    {
        pair.second->move(objects);
        pair.second->checkCollisions(objects);
    }
}

void Game::movePlayer()
{
    player.move(camera, objects);
    player.checkCollisionsWithProjectiles(projectiles);
    player.updateTimeModifier();
}

/*============= GET FUNCTIONS ============*/

const Texture3D &Game::getTexture(const std::string &name)
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return *(it->second);
    }
    throw std::runtime_error("Texture not found: " + name);
}

const Shader &Game::getShader()
{
    return shader;
}

/*============= SET FUNCTIONS ============*/

void Game::setObjectScale(const std::string &name, const glm::vec3 &scale)
{
    auto it = objects.find(name);
    if (it != objects.end())
    {
        it->second->setScale(scale);
    }
    else
    {
        throw std::runtime_error("Object not found: " + name);
    }
}

void Game::setObjectTextureScale(const std::string &name, const glm::vec2 &scale)
{
    auto it = objects.find(name);
    if (it != objects.end())
    {
        it->second->setTextureScale(scale);
    }
    else
    {
        throw std::runtime_error("Object not found: " + name);
    }
}
void Game::setObjectPosition(const std::string &name, const glm::vec4 &position)
{
    auto it = objects.find(name);
    if (it != objects.end())
    {
        it->second->setPosition(position);
    }
    else
    {
        throw std::runtime_error("Object not found: " + name);
    }
}

void Game::setObjectRotation(const std::string &name, const glm::vec3 &rotation)
{
    auto it = objects.find(name);
    if (it != objects.end())
    {
        it->second->setRotation(rotation);
    }
    else
    {
        throw std::runtime_error("Object not found: " + name);
    }
}

/*============= UPDATE FUNCTIONS ============*/

void Game::updateCamera()
{
    if(camera_mode == CameraMode::FIRST_PERSON)
    {
    camera.position = player.getPosition();
    camera.processMouseMovement();
    }
    else
    {
        // Get the last projectile's name from the player
        std::string lastProjectileName = player.getLast_projectile_name();

       // Find the projectile in the projectiles map
        auto it = projectiles.find(lastProjectileName);

        // If found, get its position
        if (it != projectiles.end())
        {
            Projectile* projectile = it->second;
            glm::vec4 projectilePos = projectile->getPosition();

            // --- Orbit logic ---
            glm::vec2 mouseOffset = Callbacks::getMouseOffset();
            float sensitivity = 0.25f;
            projectile_orbit_yaw   += mouseOffset.x * sensitivity;
            projectile_orbit_pitch -= mouseOffset.y * sensitivity;

            // Clamp pitch to avoid flipping
            if (projectile_orbit_pitch > 89.0f) projectile_orbit_pitch = 89.0f;
            if (projectile_orbit_pitch < -89.0f) projectile_orbit_pitch = -89.0f;

            // Convert spherical coordinates to Cartesian
            float yawRad = glm::radians(projectile_orbit_yaw);
            float pitchRad = glm::radians(projectile_orbit_pitch);

            glm::vec4 offset;
            offset.x = -1.0f*(projectile_orbit_radius * cos(pitchRad) * cos(yawRad));
            offset.y = -1.0f*(projectile_orbit_radius * sin(pitchRad));
            offset.z = -1.0f*(projectile_orbit_radius * cos(pitchRad) * sin(yawRad));
            offset.w = 0.0f;

            camera.position = projectilePos + offset;
            camera.lookAt(projectilePos);
        }
        else
        {
            camera_mode = CameraMode::FIRST_PERSON;
            camera.position = player.getPosition();
            camera.processMouseMovement();
            return;

        }

    }
}

void Game::updateCameraMode(void)
{
    // Check if any player projectile exists
    can_look_at_bullet = false;
    for (const auto& pair : projectiles)
    {
        if (pair.first.rfind(player.getLast_projectile_name(), 0) == 0)
        {
            can_look_at_bullet = true;
            break;
        }
    }

    if (is_asking_to_look_at_bullet && can_look_at_bullet)
    {
        camera_mode = CameraMode::FOLLOW_PROJECTILE;
    }
    else
    {
        camera_mode = CameraMode::FIRST_PERSON;
    }
}

void Game::updateGameState(void)
{
    if (player.wasHit())
    {
        game_state = GameState::GAME_OVER;
    }
    else if (CheckCollisionPrisms(player.getBBoxMin(),
                                  player.getBBoxMax(),
                                  objects["eye_sobj"]->getBBoxMin(),
                                  objects["eye_sobj"]->getBBoxMax()))
    {
        game_state = GameState::GAME_WON;
    }
}

void Game::checkAndSpawnWaves() {
    glm::vec4 player_pos = player.getPosition() - glm::vec4(0.0f, 1.5f, 0.0f, 0.0f); // Adjust player position to match floor height
    
    if (!wave1_spawned && CheckCollisionPointPrism(player_pos, objects["floor1"]->getBBoxMin(), 
                                                               objects["floor1"]->getBBoxMax())) {
        // Spawn wave 1 enemies
        addEnemy(glm::vec4(4.5f, 1.5f, 0.0f, 1.0f));
        addEnemy(glm::vec4(1.5f, 1.5f, 4.5f, 1.0f));
        addEnemy(glm::vec4(7.5f, 1.5f, 4.5f, 1.0f));
        addEnemy(glm::vec4(3.5f, 1.5f, 8.5f, 1.0f));
        addEnemy(glm::vec4(5.5f, 1.5f, 8.5f, 1.0f));
        wave1_spawned = true;
    }

    if (!wave2_spawned && CheckCollisionPointPrism(player_pos, objects["floor2"]->getBBoxMin(), 
                                                               objects["floor2"]->getBBoxMax())) {
        addEnemy(glm::vec4(3.0f, 1.5f, 11.0f, 1.0f));
        addEnemy(glm::vec4(0.0f, 1.5f, 14.0f, 1.0f));
        wave2_spawned = true;
    }

    if (!wave3_spawned && CheckCollisionPointPrism(player_pos, objects["floor3"]->getBBoxMin(), 
                                                               objects["floor3"]->getBBoxMax())) {
        addEnemy(glm::vec4(11.5f, 1.5f, 13.5f, 1.0f));
        addEnemy(glm::vec4(11.5f, 1.5f, 15.5f, 1.0f));
        addEnemy(glm::vec4(16.0f, 1.5f, 14.0f, 1.0f));
        addEnemy(glm::vec4(16.0f, 1.5f, 15.0f, 1.0f));
        wave3_spawned = true;
    }
}

void Game::useShader()
{
    shader.Use();
}

void Game::resetLevel()
{
    // Delete all enemies
    for (auto& pair : enemies) {
        // mark enemy for deletion
        pair.second->markForDeletion();
    }

    // Optionally delete all projectiles
    for (auto& pair : projectiles) {
        pair.second->markForDeletion();
    }

    // Reset wave triggers
    wave1_spawned = false;
    wave2_spawned = false;
    wave3_spawned = false;

    // Reset other state as needed
    // Example: Reset player position
    player.setPosition(starting_position);
    player.resetHit(); // Reset hit state

    // Example: Reset camera mode
    camera_mode = CameraMode::FIRST_PERSON;
    game_state = GameState::IN_GAME;
}
/*============= OTHER FUNCTIONS ============*/

void Game::allowPlayerToFly()
{
    player.fly();
}

void Game::draw()
{
    shader.ClearZBuffer();
    virtual_scene.drawScene();
}

void Game::deleteMarkedObjects()
{
    virtual_scene.deleteMarkedObjects();
    for (auto it = objects.begin(); it != objects.end();)
    {
        if (it->second->markedForDeletion())
        {
            it = objects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if (it->second->markedForDeletion())
        {
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        if (it->second->markedForDeletion())
        {
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::enterGodMode()
{
    printf("Toggling God Mode\n");
    if(!is_in_god_mode)
    {
        is_in_god_mode = true;
    }
    else
    {
        is_in_god_mode = false;
    }
}

void Game::manageGodMode()
{
    if(is_in_god_mode)
    {
        player.resetHit(); // Reset hit state every frame
        allowPlayerToFly();
    }
}

/*============= MANAGE SHOOTING ============*/

void Game::manageEnemyShooting()
{
    for (auto &pair : enemies)
    {
        Enemy *enemy = pair.second;
        glm::vec4 target_position = player.getPosition();
        glm::vec4 target_bbox_min = player.getBBoxMin();
        glm::vec4 target_bbox_max = player.getBBoxMax();

        enemy->manageShooting(target_position, virtual_scene,
                              target_bbox_min, target_bbox_max, camera, shader,
                              objects, projectiles, textures,
                              &num_projectiles);
    }
}

void Game::managePlayerShooting()
{
    if(camera_mode == CameraMode::FOLLOW_PROJECTILE)
    {
        // If the camera is in FOLLOW_PROJECTILE mode, do not allow shooting
        player.updateShootingCooldown();
        return;
    }
    player.manageShooting(virtual_scene, camera, shader, objects, projectiles);
}