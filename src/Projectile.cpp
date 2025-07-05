#include <Projectile.h>
#include <iostream>

Projectile::Projectile(const ObjModel &model,
               const std::string &name,
               Shader shader,
               const Camera &cam,
               bool is_hostile,
               glm::vec4 position,
               glm::vec4 direction)
   : SceneObject(model, name, shader, cam, true, false)
{
    this->is_hostile = is_hostile;
    this->direction = direction;
    this->direction = glm::normalize(this->direction); // Normalize the direction vector
    setPosition(position);
}


void Projectile::checkCollisions(SobjectMap objects)
{
    glm::vec4 pos = getPosition();

    for (const auto &pair : objects)
    {
        SceneObject *object = pair.second;
        if (object->collidable())
        {
            glm::vec4 bbox_min = object->getBBoxMin();
            glm::vec4 bbox_max = object->getBBoxMax();
    
            if (CheckCollisionPrismSphere(bbox_min, bbox_max, pos, radius))
            {
                this->markForDeletion();
                return;
            }
        }
    }
}

void Projectile::move(SobjectMap objects)
{ 

    if(this->markedForDeletion())
    {
        setPosition(glm::vec4(0.0f, -10000.0f, 0.0f, 1.0f));
        return; // Do not move if the projectile is marked for deletion
    }

    // Makes the projectile face the right way
    float rot = atan2(this->direction.x, this->direction.z);
    setRotationY(glm::degrees(rot));
    float deltaTime = Callbacks::getDeltaTime();
    
    glm::vec4 displacement = direction * speed * deltaTime;
    setPosition(getPosition() + displacement);
}