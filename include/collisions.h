#pragma once

#include <glm/glm.hpp>
#include <Matrices.h>

bool CheckCollisionPrisms(const glm::vec4 &a_min, const glm::vec4 &a_max,
                          const glm::vec4 &b_min, const glm::vec4 &b_max);

bool CheckCollisionPointPrism(const glm::vec4 &point,
                              const glm::vec4 &prism_min, const glm::vec4 &prism_max);

bool CheckCollisionPrismSphere(const glm::vec4 &prism_min, const glm::vec4 &prism_max,
                                  const glm::vec4 &sphere_center, float sphere_radius);

