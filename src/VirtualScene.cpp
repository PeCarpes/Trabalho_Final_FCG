#include <../include/VirtualScene.h>
#include <iostream>

void VirtualScene::addObject(SceneObject *object)
{
    objects.insert({object->getName(), object});
}

SceneObject* VirtualScene::operator[](const char* name) {
    auto it = objects.find(name);
    if (it != objects.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Object not found in the scene.");
    }
}

void VirtualScene::drawScene()
{
    int i = 0;
    for (const auto &pair : objects)
    {
        const SceneObject *object = pair.second;
        object->draw();
        i++;
    }
    std::cout << i << std::endl;
}
