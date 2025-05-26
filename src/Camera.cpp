#include "../include/Camera.h"

void Camera::setPosition(const glm::vec4 &newPosition) { position = newPosition; }
void Camera::setUp(const glm::vec4 &newUp) { up_vector = newUp; }
void Camera::setRight(const glm::vec4 &newRight) { right_vector = newRight; }
void Camera::setForward(const glm::vec4 &newForward) { view_vector = newForward; }

glm::mat4 Camera::getViewMatrix() const
{
    return Matrix_Camera_View(this->position, this->view_vector, this->up_vector);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    if(projectionType == CameraProjectionType::PERSPECTIVE)
    {
        return Matrix_Perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    }
    else
    {
        return Matrix_Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    }
}



