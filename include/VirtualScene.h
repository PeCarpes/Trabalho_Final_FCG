#pragma once

#include <ObjModel.h>
#include <map>
#include <stdexcept>

class VirtualScene
{
private:
    static inline std::map<std::string, ObjModel> objects;

public:
    static void drawScene();
    void addObject(const ObjModel &object);

    ObjModel* operator[](const char* name);


};
