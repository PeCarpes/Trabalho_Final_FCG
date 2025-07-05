#pragma once

#include <SceneObject.h>
#include <map>
#include <stdexcept>
#include <memory>

typedef std::map<std::string, SceneObject *> SobjectMap;

class VirtualScene
{
private:
    static inline SobjectMap objects;
    static int num_objects;

    public:
    static void drawScene();
    void addObject(SceneObject *object);
    void deleteMarkedObjects();
    const std::map<std::string, SceneObject *>& getObjects() const { return objects; }
    
    SceneObject *operator[](const char *name);
};
