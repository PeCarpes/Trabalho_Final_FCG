#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Matrices.h>

#include <ObjModel.h>

#include <stack>

class SceneObject
{
private:
    glm::vec4 position;
    glm::vec3 rotation; // Stored in RADIANS
    glm::vec3 scale;

    glm::vec4 upVector;

    std::string name;

protected:
    GLuint GpuProgramID;

public:
    ObjModel objModel;
    void setPosition(const glm::vec3 &newPosition);
    void addPosition(const glm::vec3 &deltaPosition);
    void setRotationX(float degrees);
    void setRotationY(float degrees);
    void setRotationZ(float degrees);
    void setRotation(glm::vec3 newRotation);
    void addRotation(glm::vec3 deltaRotation);
    void setScaleX(float factor);
    void setScaleY(float factor);
    void setScaleZ(float factor);
    void setScale(const glm::vec3 &newScale);

    void setUpVector(const glm::vec4 &newUp);

    std::string getName() const;
    glm::vec4 getPosition() const;

    void draw() const;

    SceneObject(const ObjModel &model, GLuint programID, const std::string &name);
};