#include "Camera.h"
#include "Callbacks.h"
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
    float field_of_view = fov;
    return Matrix_Perspective(field_of_view, aspectRatio, -0.1f, -100.0f);
}

void Camera::lookAt(glm::vec4 target_position) {
    forward_vector = glm::normalize(target_position - position);

    // Mantém a câmera consistente ao alternar para o modo livre
    pitch = glm::degrees(asin(forward_vector.y));
    yaw = glm::degrees(atan2(forward_vector.z, forward_vector.x));
    
    updateCameraVectors();
}

glm::vec4 Camera::getPosition() const {
    return position;
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


void Camera::processKeyboard(float delta_time) {
    float velocity = movement_speed * delta_time;

    int w_pressed = Callbacks::getKeyState(GLFW_KEY_W);
    int s_pressed = Callbacks::getKeyState(GLFW_KEY_S);
    int a_pressed = Callbacks::getKeyState(GLFW_KEY_A);
    int d_pressed = Callbacks::getKeyState(GLFW_KEY_D);

    glm::vec4 mov_vector = forward_vector;
    mov_vector.y = 0.0f;
    mov_vector = normalize(mov_vector);

    if (w_pressed == GLFW_PRESS || w_pressed == GLFW_REPEAT)
        position += mov_vector * velocity;
    if (s_pressed == GLFW_PRESS || s_pressed == GLFW_REPEAT)
        position -= mov_vector * velocity;
    if (a_pressed == GLFW_PRESS || a_pressed == GLFW_REPEAT)
        position -= right_vector * velocity;
    if (d_pressed == GLFW_PRESS || d_pressed == GLFW_REPEAT)
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