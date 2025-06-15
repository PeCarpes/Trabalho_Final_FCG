#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec4 position, glm::vec4 up, float yaw, float pitch) 
    : forward_vector(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), fov(FOV) {
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() 
{
    return Matrix_Camera_View(this->position, this->forward_vector, this->up_vector);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio)
{
    return Matrix_Perspective(45.0f, aspectRatio, 0.1f, 100.0f);
}

void Camera::lookAt(glm::vec4 target_position) {
    forward_vector = glm::normalize(target_position - position);

    // Mantém a câmera consistente ao alternar para o modo livre
    pitch = glm::degrees(asin(forward_vector.y));
    yaw = glm::degrees(atan2(forward_vector.z, forward_vector.x));
    
    updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, float delta_time) {
    float velocity = movement_speed * delta_time;
    if (direction == CameraMovement::FORWARD)
        position += forward_vector * velocity;
    if (direction == CameraMovement::BACKWARD)
        position -= forward_vector * velocity;
    if (direction == CameraMovement::LEFT)
        position -= right_vector * velocity;
    if (direction == CameraMovement::RIGHT)
        position += right_vector * velocity;
}

void Camera::processMouseMovement(float x_offset, float y_offset, bool constrain_pitch) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;

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