#pragma once
#include <glm/glm.hpp>
#include <array>

//
// This header is to give access to frequently used helper functions
//

// Collision detection SAT
bool projectionOverlap(const std::array<glm::vec3, 4>& a, const std::array<glm::vec3, 4>& b, const glm::vec3 axis);
glm::vec3 getPerpendicularXZ(glm::vec3 vector);

