#include "../include/VirtualScene.h"

void VirtualScene::addObject(const char *name, const GLfloat *vertices, int num_vertices,
                             const GLuint *indices, int num_indices,
                             GLenum render_mode)
{
    objects.insert({name, SceneObject(name, vertices, num_vertices, indices, num_indices, render_mode)});
}

void VirtualScene::addObject(const SceneObject &object)
{
    objects.insert({object.getName(), object});
}

SceneObject VirtualScene::getObject(const char *name)
{
    auto it = objects.find(name);  
    if (it != objects.end())
    {
        return it->second;
    }
    throw std::runtime_error("Object not found in the virtual scene.");
}

void VirtualScene::drawScene()
{
    for (const auto &pair : objects)
    {
        const SceneObject &object = pair.second;
        object.draw();
    }
}
