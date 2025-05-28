#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <Matrices.h>

class SceneObject
{
private:
    const char *name = "Unnamed Object";

    GLuint GpuProgramID = 0; // ID do programa de GPU associado a este objeto

    // object's topology
    GLfloat *vertices = nullptr;      
    // object's geometry
    GLuint *indices = nullptr;
    // rendering mode (e.g., GL_TRIANGLES, GL_LINES)
    GLenum render_mode = GL_TRIANGLES;
    
    int num_vertices = 0;
    int num_indices = 0;

    // object's position in global space
    glm::vec4 position_global = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    // object's rotation
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

public:
    // Construtor com número de vértices e índices
    SceneObject(const char *name, GLuint GpuProgramID, const GLfloat *vertices, int num_vertices,
                const GLuint *indices, int num_indices,
                GLenum render_mode);

    const char *getName() const;

    void setPosition(const glm::vec3 translation);
    void rotate(const glm::vec3 rotation);

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
