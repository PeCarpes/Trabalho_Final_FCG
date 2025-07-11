#pragma once

#include <SceneObject.h>
#include <map>
#include <stdexcept>
#include <memory>

class VirtualScene
{
private:
    static inline std::map<std::string, SceneObject *> objects;
    static int num_objects;

    public:
    static void drawScene();
    void addObject(SceneObject *object);
    void deleteMarkedObjects();
    const std::map<std::string, SceneObject *>& getObjects() const { return objects; }

    SceneObject *operator[](const char *name);
};
