#include "../include/VirtualScene.h"

void VirtualScene::addObject(const SceneObject &object)
{
    objects.insert({object.getName(), object});
}

SceneObject* VirtualScene::operator[](const char* name) {
    auto it = objects.find(name);
    if (it != objects.end())
    {
        return &(it->second);
    }
    else
    {
        throw std::runtime_error("Object not found in the scene.");
    }
}


void VirtualScene::drawScene()
{
    for (const auto &pair : objects)
    {
        const SceneObject &object = pair.second;
        object.draw();
    }
}
