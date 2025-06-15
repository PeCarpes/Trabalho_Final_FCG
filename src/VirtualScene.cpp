#include <../include/VirtualScene.h>

void VirtualScene::addObject(std::shared_ptr<SceneObject> object)
{
    objects[object->getName()] = object;
}

SceneObject* VirtualScene::operator[](const char* name) {
    return objects.at(name).get();
}

void VirtualScene::drawScene()
{
    for (const auto &pair : objects)
    {
        const auto &object_ptr = pair.second;
        object_ptr->draw();
    }
}
