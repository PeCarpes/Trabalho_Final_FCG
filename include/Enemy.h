
#pragma once
#include <VirtualScene.h>
#include <collisions.h>
#include <Projectile.h>
#include <miniaudio.h>

class Enemy : public SceneObject
{
public:
    Enemy(const ObjModel *model, const std::string &name, Shader shader, const Camera &cam, glm::vec3 position, float speed = 1.0f);

    void move(std::map<std::string, SceneObject *> objects, const glm::vec4 &target);
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void manageShooting(glm::vec4 target, VirtualScene &virtual_scene,
                        glm::vec4 target_bbox_min, glm::vec4 target_bbox_max,
                        const Camera &cam, Shader shader,
                        std::map<std::string, SceneObject *> objects,
                        std::map<std::string, Projectile *> &projectiles,
                        std::map<std::string, Texture3D*> textures,
                        std::map<std::string, ma_sound *> sounds,
                        int *num_projectiles);

    void setProjectileModel(ObjModel *model);
    void checkCollisionsWithProjectiles(std::map<std::string, Projectile *> &projectiles);

private:
    float speed;
    ObjModel *projectile_model = nullptr; // Model for the projectile
    float shooting_speed = 1.0f;          // Time between shots in seconds
    float shooting_cooldown = 0.0f;       // Cooldown timer for shooting
    glm::vec4 direction;                  // Direction vector for movement

    bool canShoot(void) const; // Check if the enemy can shoot based on cooldown
    glm::vec4 getNextDisplacement(glm::vec4 direction) const;
    glm::vec3 checkCollisions(const std::map<std::string, SceneObject *> &objects) const;
    bool targetInSight(const glm::vec4 bboxMin, const glm::vec4 bboxMax,
                       std::map<std::string, SceneObject *> objects) const;

};
