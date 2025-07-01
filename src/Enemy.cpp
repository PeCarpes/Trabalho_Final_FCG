#include <Enemy.h>

Enemy::Enemy(const ObjModel &model, GLuint programID, const std::string& name, glm::vec4 position, float speed)
    : SceneObject(model, programID, name) 
{
    setPosition(position);
    setSpeed(speed);
}


void Enemy::move(float deltaTime, const glm::vec4 &target){

    glm::vec4 direction = target - getPosition();
    direction = direction/norm(direction);
    glm::vec4 newPosition = getPosition() + direction * speed * deltaTime;
    setPosition(newPosition);

    // Corrige a rotação do inimigo para olhar na direção do movimento
    float rot = atan2(direction.x, direction.z);
    setRotationY(glm::degrees(rot));

}