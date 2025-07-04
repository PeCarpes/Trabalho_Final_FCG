
#include <SceneObject.h>

class Enemy : public SceneObject
{
public:
    Enemy(const ObjModel &model, const std::string& name, Shader shader, const Camera& cam, glm::vec3 position, float speed = 1.0f);

    void move(const glm::vec4 &target);
    void setSpeed(float newSpeed) { speed = newSpeed; }

    private:
    float speed;
};
