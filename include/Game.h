
#pragma once

#include <VirtualScene.h>
#include <Player.h>
#include <Camera.h>
#include <Shader.h>
#include <Callbacks.h>
#include <Texture.h>
#include <Enemy.h>
#include <ObjModel.h>
#include <Matrices.h>
#include <utils.h>
#include <miniaudio.h>

#include <glm/glm.hpp>

#include <map>

enum class GameState
{
    IN_MENU,
    IN_GAME,
    GAME_OVER,
    GAME_WON,
};

enum class CameraMode {
    FIRST_PERSON,
    FOLLOW_PROJECTILE
};

typedef std::map<std::string, ObjModel *> ObjModelMap;
typedef std::map<std::string, SceneObject *> SceneObjectMap;
typedef std::map<std::string, Enemy *> EnemyMap;
typedef std::map<std::string, Projectile *> ProjectileMap;
typedef std::map<std::string, Texture3D *> TextureMap;
typedef std::map<std::string, ma_sound *> SoundMap;

class Game
{
    const glm::vec4 starting_position = glm::vec4(4.5f, 2.25f, 4.5f, 1.0f); // Starting position of the player

    int num_enemies = 0;
    int num_projectiles = 0;

    VirtualScene virtual_scene;
    Player player;
    Camera camera;
    Shader shader;
    ma_engine sound_engine; // Sound engine for audio playback

    TextureMap textures;
    ObjModelMap obj_models;
    SceneObjectMap objects;
    EnemyMap enemies;
    ProjectileMap projectiles;
    SoundMap sounds;

    float projectile_orbit_yaw = 0.0f;
    float projectile_orbit_pitch = 20.0f;
    float projectile_orbit_radius = 1.5f;

    bool wave1_spawned = false;
    bool wave2_spawned = false;
    bool wave3_spawned = false;

    GameState game_state = GameState::IN_MENU;
    CameraMode camera_mode = CameraMode::FIRST_PERSON;

    bool is_asking_to_look_at_bullet = false;
    bool can_look_at_bullet = false;
    bool is_in_god_mode = false;

public:
    Game();

    void initializeShader();
    void initializeCamera();
    void initializeEnemies();
    void initializePlayer(const std::string &weapon_model_name, const std::string &projectile_objmodel_name,
                          const std::string &projectile_texture_name);
    void initializeSoundEngine();

    void addProjectile(Projectile *projectile);
    void addEnemy(const glm::vec4 &position);
    void addSceneObject(const std::string &name, const std::string &obj_model_name = "", const std::string &texture_name = "", int object_id = -1, bool use_view_matrix = true, bool collidable = false);
    void addTexture(const std::string &name, const std::string &file_path);
    void addObjModel(const std::string &name, const std::string &file_path);
    void addSound(const std::string &name, const std::string &file_path);
    void moveEnemies(void);
    void moveProjectiles(void);
    void movePlayer(void);

    void allowPlayerToFly(void);

    void draw(void);

    void setObjectScale(const std::string &name, const glm::vec3 &scale);
    void setObjectTextureScale(const std::string &name, const glm::vec2 &scale);
    void setObjectPosition(const std::string &name, const glm::vec4 &position);
    void setObjectRotation(const std::string &name, const glm::vec3 &rotation);

    void setBoolAskCamera(bool ask) { is_asking_to_look_at_bullet = ask; }
    void setCanLookAtBullet(bool can_look) { can_look_at_bullet = can_look; }
    void setGameState(GameState state) { game_state = state; }

    void updateCamera();
    void updateCameraMode(void);
    void updateGameState(void);
    void checkAndSpawnWaves();
    void useShader();
    void resetLevel();

    void manageEnemyShooting(void);
    void managePlayerShooting(void);

    void deleteMarkedObjects(void);

    void enterGodMode(void);
    void manageGodMode(void);

    const Texture3D &getTexture(const std::string &name);
    const Shader &getShader(void);

    Camera *getCamera(void) { return &camera; }
    CameraMode getCameraMode(void) const { return camera_mode; }
    Player *getPlayer(void) { return &player; }
    VirtualScene *getVirtualScene(void) { return &virtual_scene; }
    SceneObjectMap *getObjects(void) { return &objects; }
    EnemyMap *getEnemies(void) { return &enemies; }
    ProjectileMap *getProjectiles(void) { return &projectiles; }
    GameState getGameState(void) const { return game_state; }
    
};