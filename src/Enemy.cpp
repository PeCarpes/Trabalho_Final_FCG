#include <Enemy.h>
#include <iostream>

Enemy::Enemy(const ObjModel *model, const std::string &name, Shader shader, const Camera &cam, glm::vec3 position, float speed)
    : SceneObject(model, name, shader, cam, true, false)
{
    setPosition(position);
    setSpeed(speed);
}

bool Enemy::canShoot(void) const
{
    return shooting_cooldown >= shooting_speed;
}

void Enemy::setProjectileModel(ObjModel *model)
{
    this->projectile_model = model;
}

void Enemy::manageShooting(glm::vec4 target, VirtualScene &virtual_scene,
                           glm::vec4 target_bbox_min, glm::vec4 target_bbox_max,
                           const Camera &cam, Shader shader,
                           std::map<std::string, SceneObject *> objects,
                           std::map<std::string, Projectile *> &projectiles,
                           std::map<std::string, Texture3D*> textures,
                           std::map<std::string, ma_sound *> sounds,
                           int *num_projectiles)
{
    bool target_in_sight = targetInSight(target_bbox_min, target_bbox_max, objects);
    if (canShoot() && target_in_sight)
    {
        std::string projectile_name = "enemy_projectile_" + std::to_string(*num_projectiles);
        glm::vec4 starting_pos = getPosition();
        starting_pos.y = target.y;
        glm::vec4 direction = target - starting_pos;
        Projectile *new_proj = new Projectile(projectile_model, projectile_name, shader, cam, true, starting_pos, direction);
        new_proj->setTexture(textures["projectile_texture"]);
        new_proj->setHeight(0.05f);
        (*num_projectiles)++;

        virtual_scene.addObject(new_proj);
        projectiles[projectile_name] = new_proj;

        if(norm(direction) > 1.0f)
            ma_sound_set_volume(sounds["shooting_sound"], 0.5f/norm(direction));
            
        ma_sound_start(sounds["shooting_sound"]);

        this->shooting_cooldown = 0.0f; // Reset cooldown after shooting
    }
    else if (target_in_sight)
    {
        shooting_cooldown += Callbacks::getDeltaTime() * Callbacks::getTimeModifier();
    }
}

void Enemy::move(std::map<std::string, SceneObject *> objects, const glm::vec4 &target)
{

    this->direction = target - getPosition();

    if(norm(this->direction) > 2.0f){

            if (norm(this->direction) > 0.0f)
            this->direction = this->direction / norm(this->direction);
        this->direction.y = -1.0f;

        glm::vec3 collision_direction = checkCollisions(objects);
        this->direction.x *= (1.0f - collision_direction.x);
        this->direction.y *= (1.0f - collision_direction.y);
        this->direction.z *= (1.0f - collision_direction.z);
        
        setPosition(getPosition() + getNextDisplacement(this->direction));
    }

    // Corrige a rotação do inimigo para olhar na direção do movimento
    float rot = atan2(this->direction.x, this->direction.z);
    setRotationY(glm::degrees(rot));
}

glm::vec4 Enemy::getNextDisplacement(glm::vec4 direction) const
{
    glm::vec4 next_displacement = direction * speed * (float)(Callbacks::getDeltaTime() * Callbacks::getTimeModifier());
    next_displacement.y = direction.y * (float)(Callbacks::getDeltaTime() * Callbacks::getTimeModifier()); // Enemies fall down
    next_displacement.w = 0.0f;

    return next_displacement;
}

void Enemy::checkCollisionsWithProjectiles(std::map<std::string, Projectile *> &projectiles)
{
    for (const auto &pair : projectiles)
    {
        Projectile *proj = pair.second;
        if (proj == nullptr)
            continue;

        if (proj->isHostile())
            continue;

        if (CheckCollisionPrismSphere(getBBoxMin(), getBBoxMax(),
                                      proj->getPosition(), proj->getRadius()))
        {
            this->markForDeletion();
            proj->markForDeletion(); // Mark the projectile for deletion as well
        }
    }
}

glm::vec3 Enemy::checkCollisions(const std::map<std::string, SceneObject *> &objects) const
{
    Enemy e = *this;

    glm::vec4 displacement = e.getNextDisplacement(direction);
    glm::vec3 collision_direction(0.0f, 0.0f, 0.0f);

    glm::vec4 displacement_x = glm::vec4(displacement.x, 0.0f, 0.0f, 1.0f);
    glm::vec4 displacement_y = glm::vec4(0.0f, displacement.y, 0.0f, 1.0f);
    glm::vec4 displacement_z = glm::vec4(0.0f, 0.0f, displacement.z, 1.0f);

    glm::vec4 future_bbox_min_x = e.getBBoxMin() + displacement_x;
    glm::vec4 future_bbox_max_x = e.getBBoxMax() + displacement_x;
    glm::vec4 future_bbox_min_y = e.getBBoxMin() + displacement_y;
    glm::vec4 future_bbox_max_y = e.getBBoxMax() + displacement_y;
    glm::vec4 future_bbox_min_z = e.getBBoxMin() + displacement_z;
    glm::vec4 future_bbox_max_z = e.getBBoxMax() + displacement_z;

    for (const auto &pair : objects)
    {
        SceneObject *obj = pair.second;
        if (obj == nullptr)
            continue;

        if (!obj->collidable())
            continue; // Skip non-collidable objects

        glm::vec4 obj_bbox_min = obj->getBBoxMin();
        glm::vec4 obj_bbox_max = obj->getBBoxMax();

        obj_bbox_min -= glm::vec4(0.15f, 0.0f, 0.15f, 0.0f);
        obj_bbox_max += glm::vec4(0.15f, 0.0f, 0.15f, 0.0f);

        if (CheckCollisionPrisms(future_bbox_min_x, future_bbox_max_x, obj_bbox_min, obj_bbox_max))
        {
            collision_direction.x = 1.0f;
        }
        if (CheckCollisionPrisms(future_bbox_min_y, future_bbox_max_y, obj_bbox_min, obj_bbox_max))
        {
            collision_direction.y = 1.0f;
        }
        if (CheckCollisionPrisms(future_bbox_min_z, future_bbox_max_z, obj_bbox_min, obj_bbox_max))
        {
            collision_direction.z = 1.0f;
        }
    }

    return collision_direction;
}

bool Enemy::targetInSight(const glm::vec4 bboxMin, const glm::vec4 bboxMax,
                          std::map<std::string, SceneObject *> objects) const
{
    glm::vec4 direction = (bboxMin + bboxMax) / 2.0f - getPosition();
    float path_length = norm((bboxMin + bboxMax) / 2.0f - getPosition());

    if (norm(direction) > 0.0f)
        direction = direction / norm(direction);
    else
    {
        return true;
    }

    const float epsilon = 0.1f; // Tolerance for direction comparison
    glm::vec4 position = getPosition();

    float traveled_distance = 0.0f;
    glm::vec4 current_point = position;
    while (traveled_distance < path_length)
    {
        for (const auto &pair : objects)
        {
            SceneObject *obj = pair.second;
            if (obj == nullptr)
                continue;

            if (!obj->collidable())
                continue; // Skip non-collidable objects

            glm::vec4 obj_bbox_min = obj->getBBoxMin();
            glm::vec4 obj_bbox_max = obj->getBBoxMax();

            if (CheckCollisionPointPrism(current_point, obj_bbox_min, obj_bbox_max))
            {
                return false; // Collision detected with an object
            }
        }
        traveled_distance += norm(direction * epsilon); // Move a small step in the direction
        current_point += direction * epsilon;
    }
    return true;
}
