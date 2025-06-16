#pragma once

#include <SceneObject.h>
#include <map>
#include <stdexcept>
#include <memory>

class VirtualScene
{
private:
    static inline std::map<std::string, SceneObject *> objects;

public:
    static void drawScene();
    void addObject(SceneObject *object);

    SceneObject *operator[](const char *name);
};
