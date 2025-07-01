
#include <SceneObject.h>

class Enemy : public SceneObject
{
public:
    Enemy(const ObjModel &model, GLuint programID, const std::string& name, glm::vec4 position, float speed = 1.0f);
    void update(float deltaTime);

    void move(float deltaTime, const glm::vec4 &target);

    void setSpeed(float newSpeed) { speed = newSpeed; }


    private:
    float speed;
};
