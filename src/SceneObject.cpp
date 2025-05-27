#include "SceneObject.h"

SceneObject::SceneObject(const char* name, const GLfloat *vertices, int num_vertices,
                         const GLuint *indices, int num_indices,
                         GLenum render_mode)
{
    this->name = name;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

const char* SceneObject::getName() const { return name; }


void SceneObject::draw() const
{
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
