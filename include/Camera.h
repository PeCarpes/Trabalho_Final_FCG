#pragma once

#include <glm/glm.hpp>

#include <Matrices.h>
#include <Callbacks.h>
#include <vector>

const float YAW = 0.0f; // obs: Yaw = phi
const float PITCH = 0.0f; //  e pitch = tetha
const float SPEED = 2.5f;
const float SENSITIVITY = 0.25f;
const float FOV = 80.0f;

class Camera
{
public:
    glm::vec4 position;
    glm::vec4 forward_vector;
    glm::vec4 up_vector;
    glm::vec4 right_vector;
    glm::vec4 world_up;

    // Ângulos de Euler (em graus)
    float yaw;
    float pitch;

    // Opções da Câmera
    float movement_speed;
    float mouse_sensitivity;
    float fov;


    Camera() = default; // Construtor padrão para serialização
    Camera(const glm::vec4 track_position, glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec4 getPosition() const;
    glm::vec4 getForwardVector() const;
    glm::vec4 getUpVector() const;
    glm::vec4 getRightVector() const;

    void lookAt(glm::vec4 target_position);

    void processMouseMovement(bool constrain_pitch = true);

private:
    // Calcula os vetores de orientação a partir dos ângulos de Euler
    void updateCameraVectors();
};
