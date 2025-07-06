#include <../include/VirtualScene.h>
#include <iostream>

int VirtualScene::num_objects = 0; // talvez remover isso?

void VirtualScene::addObject(SceneObject *object)
{
    objects.insert({object->getName(), object});
}

void VirtualScene::deleteMarkedObjects()
{

    for (const auto &pair : objects)
    {
        SceneObject *object = pair.second;
        if (object->markedForDeletion())
        {
            objects.erase(object->getName()); // Remove the object from the map
            delete object;                    // Delete the object
        }

    }
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
}
