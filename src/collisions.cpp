#include <collisions.h>

bool CheckCollisionPrisms(const glm::vec4 &a_min, const glm::vec4 &a_max,
                          const glm::vec4 &b_min, const glm::vec4 &b_max)
{
    return (a_min.x <= b_max.x && a_max.x >= b_min.x &&
            a_min.y <= b_max.y && a_max.y >= b_min.y &&
            a_min.z <= b_max.z && a_max.z >= b_min.z);
}

bool CheckCollisionPointPrism(const glm::vec4 &point,
                              const glm::vec4 &prism_min, const glm::vec4 &prism_max)
{
    return (point.x >= prism_min.x && point.x <= prism_max.x &&
            point.y >= prism_min.y && point.y <= prism_max.y &&
            point.z >= prism_min.z && point.z <= prism_max.z);
}

bool CheckCollisionPrismSphere(const glm::vec4 &prism_min, const glm::vec4 &prism_max,
                               const glm::vec4 &sphere_center, float sphere_radius)
{
    glm::vec4 closest_point = glm::clamp(sphere_center, prism_min, prism_max);
    float distance_squared = glm::length(closest_point - sphere_center);
    distance_squared *= distance_squared;
    return distance_squared <= (sphere_radius * sphere_radius);
}
