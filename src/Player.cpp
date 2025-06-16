#include <../include/Player.h>

void Player::updateForwardVector(const glm::vec4 &newForward){
    forward = newForward;
    forward.y = 0.0f; // No vertical movement, that's handled by jumping
    forward = normalize(forward);
}

void Player::move(float deltaTime, Camera cam) {
    position = cam.position; // Update position to camera position    
    updateDirection();

    glm::vec4 movement;
    movement.w = 0.0f; // No movement in the fourth dimension, please 
    movement.y = 0.0f; // No vertical movement, that's handled by jumping


    movement.z = forward.z * vel.z;
    
    movement = normalize(movement);

    position += movement * speed * deltaTime;

}

void Player::updateDirection(void){

    bool move_forward = Callbacks::getKeyState(GLFW_KEY_W) == GLFW_PRESS;
    bool move_backward = Callbacks::getKeyState(GLFW_KEY_S) == GLFW_PRESS;
    bool move_left = Callbacks::getKeyState(GLFW_KEY_A) == GLFW_PRESS;
    bool move_right = Callbacks::getKeyState(GLFW_KEY_D) == GLFW_PRESS;

    vel.w = 0.0f;

    if(move_forward && move_backward){
        vel.z = 0.0f;
    }
    else if(move_forward){
        vel.z = -1.0f;
    }
    else if(move_backward){
        vel.z = 1.0f;
    }
    
    if(move_left && move_right){
        vel.x = 0.0f;
    }
    else if(move_left){
        vel.x = -1.0f;
    }
    else if(move_right){
        vel.x = 1.0f;
    }

    // Don't normalize the vector

}

void Player::updateObject(void) {
    if (player_obj) {
        player_obj->setPosition(position);
        player_obj->setUpVector(up);
        player_obj->setTranslationMatrix(Matrix_Translate(position.x, position.y, position.z));
    }
}

Player::Player(SceneObject* obj, glm::vec4 pos){
    player_obj = obj;
    position = pos;
    vel = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    forward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f); // Default forward direction
    speed = 1.0f; // Default speed
}
