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
    glm::vec4 upVector;

    glm::mat4 rotationMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 translationMatrix;

    std::string name;

protected:
GLuint GpuProgramID;

public:
ObjModel objModel;
    void setPosition(const glm::vec4 &newPosition);
    void addPosition(const glm::vec4 &deltaPosition);
    void setUpVector(const glm::vec4 &newUp);
    void setRotationMatrix(const glm::mat4 &newRotation);
    void setScaleMatrix(const glm::mat4 &newScale);
    void setTranslationMatrix(const glm::mat4 &newTranslation);

    std::string getName() const;
    glm::vec4 getPosition() const;

    void draw() const;

    SceneObject(const ObjModel &model, GLuint programID, const std::string& name);
};