#pragma once

#include <glm/glm.hpp>
#include <Callbacks.h>
#include <VirtualScene.h>
#include <Camera.h>
#include <collisions.h>
#include <Projectile.h>

class Player{

    private:
        const glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f); // Up vector in global coordinates
        glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Position in global coordinates
        glm::vec4 vel = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);      // Velocity vector (speed direction)
        glm::vec4 forward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);  // Forward direction vector
        glm::vec4 right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);    // Right direction vector
        float speed = 1.0f;                                     // Speed of the player
        SceneObject* player_obj = nullptr;                      // SceneObject representing the player
        SceneObject* weapon_obj = nullptr;                      // SceneObject representing the weapon
        
        ObjModel* projectile_model = nullptr;                   // Model for the projectile
        int num_projectiles = 0;                                // Used to name the projectiles uniquely
        std::vector<Projectile*> projectiles;                   // List of projectiles fired by the player
        float shooting_speed = 0.5f;                            // Time between shots in seconds
        float shooting_cooldown = 0.0f;                         // Cooldown timer for shooting
        bool can_shoot(void) const;                             // Check if the player can shoot based on cooldown

        glm::vec3 CheckCollisions(SobjectMap objects);
        void updateObject(void);
        void updateForwardVector(const glm::vec4 &newForward);
        void updateDirection(void);

    
        public:
        void move_projectiles();
        void move(Camera cam, SobjectMap objects);
        void manage_shooting(VirtualScene& virtual_scene, const Camera& cam, Shader shader);

        glm::vec4* getPositionPtr(void) { return &position; }

        glm::vec4 getPosition(void) const { return position; }
        glm::vec4 getForward(void) const { return forward; }
        glm::vec4 getVel(void) const { return vel; }
        glm::vec4 getNextDisplacement(void) const;
        void initializeWeapon(SceneObject* weapon);
        void initializeProjectiles(ObjModel* model);
        void setModel(SceneObject* obj) { player_obj = obj; }
        void fly();

        Player(SceneObject* obj, glm::vec4 pos);


        
};
