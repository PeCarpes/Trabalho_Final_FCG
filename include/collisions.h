#pragma once

#include <glm/glm.hpp>

bool CheckCollisionPrisms(const glm::vec4 &a_min, const glm::vec4 &a_max,
                          const glm::vec4 &b_min, const glm::vec4 &b_max);

bool CheckCollisionPointPrism(const glm::vec4 &point,
                              const glm::vec4 &prism_min, const glm::vec4 &prism_max);


