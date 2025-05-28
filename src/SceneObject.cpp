#include "SceneObject.h"

SceneObject::SceneObject(const char *name, GLuint GpuProgramID, const GLfloat *vertices, int num_vertices,
                         const GLuint *indices, int num_indices,
                         GLenum render_mode)
{
    this->name = name;
    this->GpuProgramID = GpuProgramID;
    rendering_mode = render_mode;
    index_count = static_cast<GLsizei>(num_indices);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Agora multiplicamos por 3, pois cada vértice tem 3 floats (x, y, z)
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    // Posição (atributo 0): 3 floats por vértice, espaçados por 3*sizeof(float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

const char *SceneObject::getName() const { return name; }

void SceneObject::setPosition(const glm::vec3 translation)
{
    position_global = glm::vec4(translation, 0.0f);
}

void SceneObject::rotate(const glm::vec3 rotation)
{
    this->rotation += rotation;
}

void SceneObject::draw() const
{
    glm::mat4 model = Matrix_Identity();
    model = model * Matrix_Translate(position_global.x, position_global.y, position_global.z);

    if (rotation.x != 0)
    {
        model = model * Matrix_Rotate(rotation.x, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    }
    if (rotation.y != 0)
    {
        model = model * Matrix_Rotate(rotation.y, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    }
    if (rotation.z != 0)
    {
        model = model * Matrix_Rotate(rotation.z, glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    }

    GLint model_location = glGetUniformLocation(this->GpuProgramID, "model"); // Assuming shader program is bound
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(vao);
    glDrawElements(rendering_mode, index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SceneObject::cleanup()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
