#include <Enemy.h>
#include <iostream>

Enemy::Enemy(const ObjModel &model, const std::string &name, Shader shader, const Camera &cam, glm::vec3 position, float speed)
    : SceneObject(model, name, shader, cam, true)
{
    setPosition(position);
    setSpeed(speed);
}

void Enemy::move(const glm::vec4 &target)
{

    float deltaTime = Callbacks::getDeltaTime();

    glm::vec4 direction = target - getPosition();
    direction = direction / length(direction);

    glm::vec4 newPosition = getPosition() + direction * speed * deltaTime;
    newPosition.y = getPosition().y; // Enemies don't move vertically
    setPosition(newPosition);

    // Corrige a rotação do inimigo para olhar na direção do movimento
    float rot = atan2(direction.x, direction.z);
    setRotationY(glm::degrees(rot));
}