#include "Camera.h"
#include "Callbacks.h"
#include <cmath>
#include <iostream>


Camera::Camera(const glm::vec4* track_position, glm::vec4 up, float yaw, float pitch) 
    : forward_vector(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), fov(FOV) {
    this->position = track_position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const 
{
    return Matrix_Camera_View(*position, this->forward_vector, this->up_vector);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    float aspectRatio = Callbacks::getScreenRatio();
    return Matrix_Perspective(fov, aspectRatio, -0.1f, -100.0f);
}

void Camera::lookAt(glm::vec4 target_position) {
    forward_vector = glm::normalize(target_position - (*position));

    // Mantém a câmera consistente ao alternar para o modo livre
    pitch = glm::degrees(asin(forward_vector.y));
    yaw = glm::degrees(atan2(forward_vector.z, forward_vector.x));
    
    updateCameraVectors();
}

glm::vec4 Camera::getPosition() const {
    return *position;
}

glm::vec4 Camera::getForwardVector() const {
    return forward_vector;
}

glm::vec4 Camera::getUpVector() const{
    return up_vector;
}

glm::vec4 Camera::getRightVector() const{
    return right_vector;
}

void Camera::processMouseMovement(bool constrain_pitch) {

    glm::vec2 offset = Callbacks::getMouseOffset();

    if (offset.x >= 100.0f || offset.y >= 100.0f) {
        std::cout << "Mouse Offset: " << offset.x << ", " << offset.y << std::endl;
    }
    

    offset.x *= mouse_sensitivity;
    offset.y *= mouse_sensitivity;

    yaw += offset.x;
    pitch -= offset.y;

    if (constrain_pitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec4 new_forward = glm::vec4 (0.0f, 0.0f, 0.0f, 0.0f);
    new_forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_forward.y = sin(glm::radians(pitch));
    new_forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward_vector = normalize(new_forward);
    
    right_vector = normalize(crossproduct(forward_vector, world_up));
    up_vector = normalize(crossproduct(right_vector, forward_vector));
}