
#pragma once
#include <VirtualScene.h>
#include <collisions.h>

class Enemy : public SceneObject
{
public:
    Enemy(const ObjModel &model, const std::string& name, Shader shader, const Camera& cam, glm::vec3 position, float speed = 1.0f);

    void move(SobjectMap objects, const glm::vec4 &target);
    void setSpeed(float newSpeed) { speed = newSpeed; }

    private:
    float speed;
    glm::vec4 direction; // Direction vector for movement

    glm::vec4 getNextDisplacement(glm::vec4 direction) const;
    glm::vec3 checkCollisions(const SobjectMap &objects) const;


};
