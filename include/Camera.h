#pragma once

#include <glm/glm.hpp>

#include "../include/Matrices.h"
#include <vector>


const float YAW         = -90.0f; // obs: Yaw = phi
const float PITCH       =  0.0f;  //  e pitch = tetha
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV         =  45.0f;

class Camera {
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

        Camera(glm::vec4 position = glm::vec4(0.0f, 0.0f, 3.0f, 1.0f), glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix(float aspectRatio);

        void lookAt(glm::vec4 target_position);

        void processKeyboard(float delta_time);
        void processMouseMovement(float x_offset, float y_offset, bool constrain_pitch = true);

    private:
        // Calcula os vetores de orientação a partir dos ângulos de Euler
        void updateCameraVectors();
};

