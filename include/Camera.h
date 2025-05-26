#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../include/Matrices.h"

enum class CameraProjectionType {
    ORTHOGRAPHIC,
    PERSPECTIVE
};

class Camera {
    private:
        glm::vec4 position;
        glm::vec4 up_vector;
        glm::vec4 right_vector;
        glm::vec4 view_vector;

        float FOV_angle_deg;

        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        CameraProjectionType projectionType = CameraProjectionType::PERSPECTIVE;
    public:
        Camera(glm::vec4 position, glm::vec4 up, glm::vec4 right, glm::vec4 forward, CameraProjectionType type = CameraProjectionType::PERSPECTIVE)
            : position(position), up_vector(up), right_vector(right), view_vector(forward), projectionType(type) {}

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float aspectRatio) const;

        void setPosition(const glm::vec4& newPosition);
        void setUp(const glm::vec4& newUp);
        void setRight(const glm::vec4& newRight);
        void setForward(const glm::vec4& newForward);

};

