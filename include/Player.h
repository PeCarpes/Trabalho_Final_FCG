#pragma once

#include <glm/glm.hpp>
#include <Callbacks.h>
#include <VirtualScene.h>
#include <Camera.h>
#include <collisions.h>
#include <Projectile.h>
#include <miniaudio.h>

typedef std::map<std::string, ma_sound *> SoundMap;

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
        Texture3D* projectile_texture = nullptr;                // Texture for the projectile
        SoundMap* sounds = nullptr;

        std::string last_projectile_name;

        int num_projectiles = 0;                                // Used to name the projectiles uniquely
        float shooting_speed = 0.5f;                            // Time between shots in seconds
        float shooting_cooldown = 0.0f;                         // Cooldown timer for shooting
        
        // These values are used to define the size of the player in the scene
        // They are used to create a bounding box for collision detection
        const float width = 0.35f;
        const float height = 1.0f;
        const float depth = 0.5f;

        bool can_shoot(void) const;                             // Check if the player can shoot based on cooldown
        bool is_walking = false;
        bool was_hit = false;
        bool is_running = false;

        bool CheckCollisions(std::map<std::string, SceneObject*> objects);
        void updateObject(void);
        void updateForwardVector(const glm::vec4 &newForward);
        void updateDirection(void);
        void checkIfRunning(void);

        public:

        void updateTimeModifier(void);
        void updateShootingCooldown(void);

        void setSoundMap(SoundMap* sound_map) { sounds = sound_map; }

        void checkCollisionsWithProjectiles(std::map<std::string, Projectile *> &projectiles);
        void setProjectileTexture(Texture3D* texture) { projectile_texture = texture; }
        void move(Camera cam, std::map<std::string, SceneObject*> objects);
        void manageShooting(VirtualScene& virtual_scene, const Camera& cam, Shader shader, 
                            std::map<std::string, SceneObject*> objects, 
                            std::map<std::string, Projectile*> &projectiles);
        void initializeWeapon(SceneObject* weapon);
        void initializeProjectiles(ObjModel* model);
        void fly();
        
        void setPosition(const glm::vec4& pos) { position = pos; }
        void setModel(SceneObject* obj) { player_obj = obj; }

        glm::vec4* getPositionPtr(void) { return &position; }
        glm::vec4 getPosition(void) const { return position; }
        glm::vec4 getForward(void) const { return forward; }
        glm::vec4 getVel(void) const { return vel; }
        glm::vec4 getNextDisplacement(void) const;
        glm::vec4 getBBoxMin(void);
        glm::vec4 getBBoxMax(void);
        std::string getLast_projectile_name(void) const { return last_projectile_name; }

        bool wasHit(void) const { return was_hit; }
        void resetHit(void) { was_hit = false; }

        Player(SceneObject* obj, glm::vec4 pos);
        Player() = default; // Default constructor for serialization purposes


        
};
