#include <../include/Player.h>
#include <iostream>

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

void Player::updateForwardVector(const glm::vec4 &newForward)
{
    forward = newForward;
    forward.y = 0.0f; // No vertical movement, that's handled by jumping
    forward = normalize(forward);
}

void Player::move(float deltaTime, Camera cam)
{
    updateDirection();
    forward = cam.getForwardVector();
    glm::vec4 right = cam.getRightVector();

    std::cout << "Player forward: " << forward.x << ", " << forward.y << ", " << forward.z << std::endl;
    std::cout << "Player right: " << right.x << ", " << right.y << ", " << right.z << std::endl;

    glm::vec4 movement;
    
    movement += forward * -vel.z;
    movement += right * vel.x;
    
    movement.w = 0.0f; // No movement in the fourth dimension, please
    movement.y = 0.0f; // No vertical movement, that's handled by jumping
    
    // Avoid division by zero
    if(length(movement) > 0.0f)
        movement = movement / length(movement);
        
    position += movement * speed * deltaTime;
    
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

    if ((move_forward && move_backward))
    {
        vel.z = 0.0f;
    }
    else if (move_forward)
    {
        vel.z = -1.0f;
    }
    else if (move_backward)
    {
        vel.z = 1.0f;
    }
    else
    {
        vel.z = 0.0f; // No forward/backward movement
    }

    if ((move_left && move_right))
    {
        vel.x = 0.0f;
    }
    else if (move_left)
    {
        vel.x = -1.0f;
    }
    else if (move_right)
    {
        vel.x = 1.0f;
    }
    else
    {
        vel.x = 0.0f; // No left/right movement
    }

    std::cout << "Player velocity: " << vel.x << ", " << vel.y << ", " << vel.z << std::endl;

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
