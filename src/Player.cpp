#include <../include/Player.h>
#include <iostream>

glm::vec4 Player::getBBoxMax(void)
{
    return glm::vec4(position.x + width / 2.0f, position.y + height, position.z + depth / 2.0f, 1.0f);
}

glm::vec4 Player::getBBoxMin(void){
    return glm::vec4(position.x - width / 2.0f, position.y, position.z - depth / 2.0f, 1.0f);
}

void Player::initializeWeapon(SceneObject *weapon)
{
    weapon_obj = weapon;
    if (weapon_obj)
    {
        Bezier weapon_bob_curve = Bezier(glm::vec4(0.400f, -0.600f, -0.900f, 1.0f),
                                         glm::vec4(0.400f, -0.566f, -0.933f, 1.0f),
                                         glm::vec4(0.400f, -0.566f, -0.966f, 1.0f),
                                         glm::vec4(0.400f, -0.600f, -1.000f, 1.0f));
        weapon_obj->setBobbingCurve(weapon_bob_curve);
        weapon_obj->setBobbingCPS(2.0f); // 2.0 cycles per second
    }
}

void Player::fly()
{
    if (Callbacks::getKeyState(GLFW_KEY_SPACE) == GLFW_PRESS ||
        Callbacks::getKeyState(GLFW_KEY_SPACE) == GLFW_REPEAT)
    {
        position.y += 10.1f * Callbacks::getDeltaTime(); // Move up
    }

    if (Callbacks::getKeyState(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        Callbacks::getKeyState(GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT)
    {
        position.y -= 10.1f * Callbacks::getDeltaTime(); // Move down
    }
}

// Check for collisions between the player and objects in the scene
// Returns the direction of the collision as a glm::vec3
glm::vec3 Player::CheckCollisions(SobjectMap objects)
{
    Player p = *this;

    glm::vec4 current_pos = p.getPosition();
    glm::vec4 future_pos = p.getPosition() + p.getNextDisplacement();
    glm::vec3 collision_direction(0.0f, 0.0f, 0.0f);

    glm::vec4 future_pos_x = glm::vec4(future_pos.x, current_pos.y, current_pos.z, 1.0f);
    // this -1.0f avoids player from entering the ground
    glm::vec4 future_pos_y = glm::vec4(current_pos.x, future_pos.y - 1.0f, current_pos.z, 1.0f);
    glm::vec4 future_pos_z = glm::vec4(current_pos.x, current_pos.y, future_pos.z, 1.0f);

    for (const auto &pair : objects)
    {
        SceneObject *obj = pair.second;
        if (!obj->collidable())
            continue; // Skip non-collidable objects

        glm::vec4 bbox_min = obj->getBBoxMin();
        glm::vec4 bbox_max = obj->getBBoxMax();

        bbox_min -= glm::vec4(0.15f, 0.0f, 0.15f, 0.0f);
        bbox_max += glm::vec4(0.15f, 0.0f, 0.15f, 0.0f);

        if (CheckCollisionPointPrism(future_pos_x, bbox_min, bbox_max))
        {
            collision_direction.x = 1.0f;
        }
        if (CheckCollisionPointPrism(future_pos_y, bbox_min, bbox_max))
        {
            collision_direction.y = 1.0f;
        }
        if (CheckCollisionPointPrism(future_pos_z, bbox_min, bbox_max))
        {
            collision_direction.z = 1.0f;
        }
    }

    return collision_direction;
}

void Player::updateForwardVector(const glm::vec4 &newForward)
{
    forward = newForward;
    forward.y = 0.0f; // No vertical movement, that's handled by jumping
    forward = normalize(forward);
}

glm::vec4 Player::getNextDisplacement(void) const
{
    glm::vec4 next_displacement = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    next_displacement += forward * -vel.z;
    next_displacement += right * vel.x; // Right vector is (1, 0, 0)
    next_displacement.w = 0.0f;

    next_displacement *= speed * (float)Callbacks::getDeltaTime();
    next_displacement.y = vel.y * (float)Callbacks::getDeltaTime();

    return next_displacement;
}

void Player::initializeProjectiles(ObjModel *model)
{
    this->projectile_model = model;
    num_projectiles = 0;
}

bool Player::can_shoot(void) const
{
    return shooting_cooldown >= shooting_speed;
}

void Player::move_projectiles(SobjectMap objects)
{
    for (Projectile *p : projectiles)
    {
        p->move(objects);
        p->checkCollisions(objects);
    }
}

void Player::manageShooting(VirtualScene &virtual_scene, const Camera &cam, Shader shader, SobjectMap objects)
{
    if (Callbacks::isLeftMouseButtonPressed() && can_shoot())
    {
        std::string projectile_name = "projectile_" + std::to_string(num_projectiles);
        glm::vec4 starting_pos = position + forward;

        Projectile *new_proj = new Projectile(*projectile_model, projectile_name, shader, cam, false, starting_pos, forward);
        new_proj->setHeight(0.01f);
        virtual_scene.addObject(new_proj);
        num_projectiles++;

        this->projectiles.push_back(new_proj);
        this->shooting_cooldown = 0.0f; // Reset cooldown after shooting
    }
    else if(!can_shoot()){
        shooting_cooldown += Callbacks::getDeltaTime();
    }

    move_projectiles(objects);
}

void Player::move(Camera cam, SobjectMap objects)
{
    updateDirection();
    forward = cam.getForwardVector();
    right = cam.getRightVector();

    glm::vec3 collision_direction = CheckCollisions(objects);
    vel.x *= (1.0f - collision_direction.x);
    vel.y *= (1.0f - collision_direction.y);
    vel.z *= (1.0f - collision_direction.z);
    position += getNextDisplacement();

    // Check if player is running
    if (Callbacks::getKeyState(GLFW_KEY_LEFT_SHIFT) != GLFW_RELEASE)
    {
        speed = 2.0f;
    }
    else
    {
        speed = 1.0f;
    }

    if (weapon_obj)
        weapon_obj->bob(); // Apply the bobbing effect to the weapon
}

void Player::updateDirection(void)
{

    bool move_forward = Callbacks::getKeyState(GLFW_KEY_W) == GLFW_PRESS ||
                        Callbacks::getKeyState(GLFW_KEY_W) == GLFW_REPEAT;

    bool move_backward = Callbacks::getKeyState(GLFW_KEY_S) == GLFW_PRESS ||
                         Callbacks::getKeyState(GLFW_KEY_S) == GLFW_REPEAT;

    bool move_left = Callbacks::getKeyState(GLFW_KEY_A) == GLFW_PRESS ||
                     Callbacks::getKeyState(GLFW_KEY_A) == GLFW_REPEAT;

    bool move_right = Callbacks::getKeyState(GLFW_KEY_D) == GLFW_PRESS ||
                      Callbacks::getKeyState(GLFW_KEY_D) == GLFW_REPEAT;

    bool moving = move_forward || move_backward || move_left || move_right;

    vel.w = 0.0f;
    vel.y = -1.0f;

    if ((move_forward && move_backward))
        vel.z = 0.0f;
    else if (move_forward)
        vel.z = -1.0f;
    else if (move_backward)
        vel.z = 1.0f;
    else
        vel.z = 0.0f; // No forward/backward movement

    if ((move_left && move_right))
        vel.x = 0.0f;
    else if (move_left)
        vel.x = -1.0f;
    else if (move_right)
        vel.x = 1.0f;
    else
        vel.x = 0.0f; // No left/right movement

    // Don't normalize the vel vector

    bool running = Callbacks::getKeyState(GLFW_KEY_LEFT_SHIFT) != GLFW_RELEASE;

    if (weapon_obj)
    {
        if (running)
        {
            weapon_obj->setBobbingCPS(2.0f); // 2.0 cycles per second when running
        }

        else if (moving)
        {
            weapon_obj->setBobbingCPS(1.0f);
        }
        else
        {
            weapon_obj->setBobbingCPS(0.0f); // No bobbing when not moving
        }
    }
}

void Player::updateObject(void)
{
    if (player_obj)
    {
        player_obj->setPosition(position);
        player_obj->setUpVector(up);
        player_obj->setPosition(glm::vec3(position.x, position.y, position.z));
    }
}

Player::Player(SceneObject *obj, glm::vec4 pos)
{
    player_obj = obj;
    position = pos;
    vel = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    forward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f); // Default forward direction
    speed = 1.0f;                                // Default speed
}
