#include <../include/SceneObject.h>

SceneObject::SceneObject(const ObjModel &model, GLuint programID, const std::string& name, bool useViewMatrix)
    : objModel(model), GpuProgramID(programID),
      position(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
      upVector(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)),
      rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
      scale(glm::vec3(1.0f, 1.0f, 1.0f)),
      useViewMatrix(useViewMatrix), // Use view matrix by default
      name(name) {}

void SceneObject::setPosition(const glm::vec3 &newPosition)
{
    position = glm::vec4(newPosition, 1.0f);
}

void SceneObject::addPosition(const glm::vec3 &deltaPosition)
{
    position += glm::vec4(deltaPosition, 0.0f);
}

void SceneObject::setUpVector(const glm::vec4 &newUp)
{
    upVector = newUp;
}

glm::vec4 SceneObject::getPosition() const
{
    return position;
}

std::string SceneObject::getName() const
{
    return name;
}

void SceneObject::setRotationX(float degrees)
{
    rotation.x = glm::radians(degrees);
}
void SceneObject::setRotationY(float degrees)
{
    rotation.y = glm::radians(degrees);
}
void SceneObject::setRotationZ(float degrees)
{
    rotation.z = glm::radians(degrees);
}
void SceneObject::setRotation(glm::vec3 newRotation)
{
    rotation = glm::radians(newRotation);
}
void SceneObject::addRotation(glm::vec3 deltaRotation)
{
    rotation += glm::radians(deltaRotation);
}

void SceneObject::setScaleX(float factor)
{
    scale.x = factor;
}

void SceneObject::setScaleY(float factor)
{
    scale.y = factor;
}

void SceneObject::setScaleZ(float factor)
{
    scale.z = factor;
}

void SceneObject::setScale(const glm::vec3 &newScale)
{
    scale = newScale;
}

void SceneObject::draw() const
{
    glm::mat4 model = Matrix_Identity();
    
    model = model * Matrix_Translate(position.x, position.y, position.z);
    model = model * Matrix_Scale(scale.x, scale.y, scale.z);
    model = model * Matrix_Rotate(rotation.x, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    model = model * Matrix_Rotate(rotation.y, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    model = model * Matrix_Rotate(rotation.z, glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

    // If useViewMatrix is false, we clear the depth buffer and set the view matrix to identity.
    // This is useful for rendering objects like a gun in an FPS game, to draw the viewmodel space.
    // https://www.reddit.com/r/GraphicsProgramming/comments/3692ch/rendering_the_gun_in_an_fps_opengl_c/
    if(!useViewMatrix) 
    {
        glClear(GL_DEPTH_BUFFER_BIT); // avoid depth testing for this object
        glm::mat4 view = Matrix_Identity();
        GLuint view_loc = glGetUniformLocation(GpuProgramID, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    }

    GLuint model_loc = glGetUniformLocation(GpuProgramID, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    objModel.draw();
}
