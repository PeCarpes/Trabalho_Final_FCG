#pragma once

#include <VirtualScene.h>
#include <collisions.h>
#include <Texture.h>

class Projectile : public SceneObject
{
public:
    Projectile(const ObjModel *model,
               const std::string &name,
               Shader shader,
               const Camera &cam,
               bool is_hostile,
               glm::vec4 position,
               glm::vec4 direction);
    void move(std::map<std::string, SceneObject *> objects);
    void checkCollisions(std::map<std::string, SceneObject *> objects);
    bool isHostile() const { return is_hostile; }
    float getRadius() const { return radius; }

    glm::vec4 getDirection() const { return direction; }

private:
    Shader shader;
    std::map<std::string, SceneObject *> virtual_scene;
    const float radius = 0.1f;

    const float speed = 4.0f;
    bool is_hostile;          // Used to differentiate between player and enemy projectiles
    glm::vec4 direction;
};