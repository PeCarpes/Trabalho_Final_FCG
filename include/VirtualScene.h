#pragma once

#include <SceneObject.h>
#include <map>
#include <stdexcept>

class VirtualScene
{
private:
    static inline std::map<const char *, SceneObject> objects;

public:
    static void drawScene();

    void addObject(const char *name, const GLfloat *vertices, int num_vertices,
                   const GLuint *indices, int num_indices,
                   GLenum render_mode = GL_TRIANGLES);

    void addObject(const SceneObject &object);

    static SceneObject getObject(const char *name);
};
