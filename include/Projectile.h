#pragma once

#include <VirtualScene.h>
#include <collisions.h>

class Projectile : public SceneObject
{
public:
    Projectile(const ObjModel &model,
               const std::string &name,
               Shader shader,
               const Camera &cam,
               bool is_hostile,
               glm::vec4 position,
               glm::vec4 direction);
    void move();
    void checkCollisions();

private:
    Shader shader;
    SobjectMap virtual_scene;
    const float radius = 0.1f;

    const float speed = 1.0f;
    bool is_hostile;
    glm::vec4 direction;
    SobjectMap objects;
};