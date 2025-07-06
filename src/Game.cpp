
#include "Game.h"

#include <iostream>

/*============= CONSTRUCTOR ============*/

Game::Game()
{

    num_enemies = 0;
}

/*============= INITIALIZE FUNCTIONS ============*/

void Game::initializeCamera()
{
    camera = Camera(player.getPositionPtr());
}

void Game::initializeShader()
{
    shader = Shader();
}

void Game::initializePlayer(const std::string &weapon_model_name, const std::string &projectile_objmodel_name)
{
    player.setPosition(glm::vec4(4.5f, 2.0f, 4.5f, 1.0f));
    player.setModel(nullptr);

    player.initializeWeapon(objects[weapon_model_name]);
    player.initializeProjectiles(obj_models[projectile_objmodel_name]);
}

/*============= ADD FUNCTIONS ============*/

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

    if (texture_name != "")
        scene_object->setTexture(*textures[texture_name]);
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
    }
}

void Game::movePlayer()
{
    player.move(camera, objects);
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
    camera.processMouseMovement();
}

void Game::updateCurrentGameState(GameState *currentGameState)
{
    if( *currentGameState == GameState::IN_MENU )
    {
        if (Callbacks::getKeyState(GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            *currentGameState = GameState::IN_GAME;
        }
    }
    else if (*currentGameState == GameState::IN_GAME)
    {
        // add any game-specific updates here
    }
}

void Game::useShader()
{
    shader.Use();
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
                              objects, projectiles);
    }
}

void Game::managePlayerShooting()
{
    player.manageShooting(virtual_scene, camera, shader, objects, projectiles);
}