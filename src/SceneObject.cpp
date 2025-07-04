#include <../include/SceneObject.h>

#include <iostream>

SceneObject::SceneObject(const ObjModel &model, const std::string &name, Shader shader, const Camera &cam, bool useViewMatrix, bool collidable)
    : objModel(model),
      position(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
      upVector(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)),
      rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
      scale(glm::vec3(1.0f, 1.0f, 1.0f)),
      useViewMatrix(useViewMatrix), // Use view matrix by default
      isCollidable(collidable),
      name(name),
      texture_id(0),
      cam(cam),
      shader(shader)
{
    bobbing_curve = Bezier();
    bobbing_cps = 1.0f;
    GpuProgramID = shader.GetGpuProgramID();
}

void SceneObject::setID(int newID)
{
    object_id = newID;
    printf("Object ID set to %d for object %s\n", object_id, name.c_str());
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

void SceneObject::bob(void)
{
    setPosition(glm::vec3(bobbing_curve.evaluate(glfwGetTime() * bobbing_cps))); // Set position to the current evaluation of the curve
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
    return getBBoxMax().y - getBBoxMin().y;
}

void SceneObject::setHeight(float height)
{
    // Set all dimensions to match the height
    float currentHeight = this->height();
    if (currentHeight > 0.0f)
    {
        float scaleFactor = height / currentHeight;
        scale *= scaleFactor;
    }
}

glm::vec4 SceneObject::getBBoxMin() const
{
    glm::vec4 bboxMin = glm::vec4(objModel.getBboxMin(), 1.0f);

    bboxMin = Matrix_Translate(position.x, position.y, position.z) *
            Matrix_Scale(scale.x, scale.y, scale.z) * 
            bboxMin;

    return bboxMin;

}

glm::vec4 SceneObject::getBBoxMax() const
{
   glm::vec4 bboxMax = glm::vec4(objModel.getBboxMax(), 1.0f);

    bboxMax = Matrix_Translate(position.x, position.y, position.z) *
            Matrix_Scale(scale.x, scale.y, scale.z) *
            bboxMax;

    return bboxMax;

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

    glUseProgram(GpuProgramID);

    shader.SetUniform("bbox_min", objModel.getBboxMin());
    shader.SetUniform("bbox_max", objModel.getBboxMax());    
    shader.SetUniform("object_id", (int) object_id);
    shader.SetUniform("model", model);
    shader.SetUniform("projection", cam.getProjectionMatrix());

    // prepara a textura ativa para carregar a textura do objeto
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Diz ao shader para usar a textura da unidade 0
    glUniform1i(glGetUniformLocation(this->GpuProgramID, "TextureImage"), 0);

    objModel.draw();
}
