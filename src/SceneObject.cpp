#include <../include/SceneObject.h>

#include <iostream>

SceneObject::SceneObject(const ObjModel &model, const std::string &name, Shader shader, const Camera &cam, bool useViewMatrix)
    : objModel(model),
      position(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
      upVector(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)),
      rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
      scale(glm::vec3(1.0f, 1.0f, 1.0f)),
      useViewMatrix(useViewMatrix), // Use view matrix by default
      name(name),
      texture_id(0),
      cam(cam),
      shader(shader)
{
    GpuProgramID = shader.GetGpuProgramID();
}

void SceneObject::setID(int newID)
{
    object_id = newID;
}

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

float SceneObject::height() const
{
    // Assuming the height is the difference between the max and min Y coordinates of the bounding box
    return objModel.height();
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
    if (!useViewMatrix)
    {
        shader.ClearZBuffer();
        shader.SetUniform("view", Matrix_Identity());
    }
    else
    {
        shader.SetUniform("view", cam.getViewMatrix());
    }

    // Importante: http://forum.lwjgl.org/index.php?topic=6967.0
    // "OpenGL GLSL compiler agressively optimizes/removes unused uniforms"
    // Se ocorrer mensagem de erro  "var is not found in shader", é porque o shader não está usando a variável var.

    // shader.SetUniform("texture_id", (int) texture_id);
    // shader.SetUniform("object_id", (int) object_id);
    shader.SetUniform("model", model);
    shader.SetUniform("projection", cam.getProjectionMatrix());

    objModel.draw();
}
