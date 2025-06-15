#pragma once

#include <SceneObject.h>
#include <map>
#include <stdexcept>
#include <memory>

class VirtualScene
{
private:
    static inline std::map<std::string, std::shared_ptr<SceneObject>> objects;

public:
    static void drawScene();
    void addObject(std::shared_ptr<SceneObject> object);

    SceneObject* operator[](const char* name);

};
