#pragma once

#include <glad/glad.h>
#include <string>

class SceneObject
{
private:
    const char* name = nullptr;

public:
    // Construtor com número de vértices e índices
    SceneObject(const char* name, const GLfloat *vertices, int num_vertices,
                const GLuint *indices, int num_indices,
                GLenum render_mode);

    const char* getName() const;

    // Desenhar o objeto
    void draw() const;

    // Liberar buffers
    void cleanup();

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei index_count = 0;
    GLenum rendering_mode = GL_TRIANGLES;
};
