#include <../include/SceneObject.h>

SceneObject::SceneObject(const ObjModel &model, GLuint programID)
    : objModel(model), GpuProgramID(programID),
      position(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
      upVector(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)),
      rotationMatrix(Matrix_Identity()),
      scaleMatrix(Matrix_Identity()),
      translationMatrix(Matrix_Identity()) {}

void SceneObject::setPosition(const glm::vec4 &newPosition)
{
    position = newPosition;
}

void SceneObject::addPosition(const glm::vec4 &deltaPosition)
{
    position += deltaPosition;
}

void SceneObject::setUpVector(const glm::vec4 &newUp)
{
    upVector = newUp;
}

void SceneObject::setRotationMatrix(const glm::mat4 &newRotation)
{
    rotationMatrix = newRotation;
}

void SceneObject::setScaleMatrix(const glm::mat4 &newScale)
{
    scaleMatrix = newScale;
}

void SceneObject::setTranslationMatrix(const glm::mat4 &newTranslation)
{
    translationMatrix = newTranslation;
}

glm::vec4 SceneObject::getPosition() const
{
    return position;
}

std::string SceneObject::getName() const
{
    return objModel.getName();
}

void SceneObject::draw() const
{
    glm::mat4 model = Matrix_Identity();
    model = model * translationMatrix * rotationMatrix * scaleMatrix;
    
    PrintMatrix(translationMatrix);

    GLuint model_loc = glGetUniformLocation(GpuProgramID, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    
    objModel.draw();

}
