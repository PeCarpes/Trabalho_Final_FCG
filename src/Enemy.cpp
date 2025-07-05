#include <Enemy.h>
#include <iostream>

Enemy::Enemy(const ObjModel &model, const std::string &name, Shader shader, const Camera &cam, glm::vec3 position, float speed)
    : SceneObject(model, name, shader, cam, true)
{
    setPosition(position);
    setSpeed(speed);
}

void Enemy::move(SobjectMap objects, const glm::vec4 &target)
{
    float deltaTime = Callbacks::getDeltaTime();

    this->direction = target - getPosition();
    if (length(this->direction) > 0.0f)
    this->direction = this->direction / length(this->direction);
    this->direction.y = -1.0f;

    glm::vec3 collision_direction = checkCollisions(objects);
    this->direction.x *= (1.0f - collision_direction.x);
    this->direction.y *= (1.0f - collision_direction.y);
    this->direction.z *= (1.0f - collision_direction.z);

    setPosition(getPosition() + getNextDisplacement(this->direction));

    // Corrige a rotação do inimigo para olhar na direção do movimento
    float rot = atan2(this->direction.x, this->direction.z);
    setRotationY(glm::degrees(rot));
}

glm::vec4 Enemy::getNextDisplacement(glm::vec4 direction) const
{
    glm::vec4 next_displacement = direction * speed * (float) Callbacks::getDeltaTime();
    next_displacement.y = direction.y * (float) Callbacks::getDeltaTime(); // Enemies fall down
    next_displacement.w = 0.0f;
    
    return next_displacement;
}


glm::vec3 Enemy::checkCollisions(const SobjectMap &objects) const
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

    int i = 0;
    std::cout << std::endl;
    for (const auto &pair : objects) {
        SceneObject *obj = pair.second;
        if (!obj->collidable()) continue; // Skip non-collidable objects

        i++;
        std::cout << "Checking collision with object: " << obj->getName() << std::endl;

        glm::vec4 obj_bbox_min = obj->getBBoxMin();
        glm::vec4 obj_bbox_max = obj->getBBoxMax();

        obj_bbox_min -= glm::vec4(0.15f, 0.0f, 0.15f, 0.0f);
        obj_bbox_max += glm::vec4(0.15f, 0.0f, 0.15f, 0.0f);

        if (CheckCollisionPrisms(future_bbox_min_x, future_bbox_max_x, obj_bbox_min, obj_bbox_max)) {
            collision_direction.x = 1.0f;
        }
        if (CheckCollisionPrisms(future_bbox_min_y, future_bbox_max_y, obj_bbox_min, obj_bbox_max)) {
            collision_direction.y = 1.0f;
        }
        if (CheckCollisionPrisms(future_bbox_min_z, future_bbox_max_z, obj_bbox_min, obj_bbox_max)) {
            collision_direction.z = 1.0f;
        }

    }

    std::cout << "Collision direction: " << collision_direction.x << ", " << collision_direction.y << ", " << collision_direction.z << std::endl;

    return collision_direction;
}