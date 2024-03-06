#include <helper.hpp>

// helper function
bool projectionOverlap(const std::array<glm::vec3, 4>& a, const std::array<glm::vec3, 4>& b, const glm::vec3 axis)
{
    std::array<float, 4> projections_a, projections_b;
    float min_a, max_a, min_b, max_b;

    int i;

    // Project vertices onto axis
    for (i = 0; i < 4; i++)
    {
        projections_a[i] = glm::dot(a[i], axis);
        projections_b[i] = glm::dot(b[i], axis);
    }

    // Initalize the min and maxes
    min_a = projections_a[0];
    max_a = projections_a[0];
    min_b = projections_b[0];
    max_b = projections_b[0];

    for (i = 0; i < 4; i++)
    {
        if (projections_a[i] < min_a) min_a = projections_a[i];
        if (projections_a[i] > max_a) max_a = projections_a[i];
        if (projections_b[i] < min_b) min_b = projections_b[i];
        if (projections_b[i] > max_b) max_b = projections_b[i];
    }

    // LOG(DEBUG, "projectionOverlap(): min_a: " << min_a << " | max_a: " << max_a << " | min_b: " << min_b << " | max_b: " << max_b)

    constexpr float lenience = 0.05; 
    if (max_a <= min_b + lenience || max_b <= min_a + lenience)
    {
        // No overlap
        return false;
    }
    // Overlap
    return true;
}

// Get perpendicular edges
glm::vec3 getPerpendicularXZ(glm::vec3 vector)
{
    glm::vec3 returnVector;
    returnVector.x = -vector.z;
    returnVector.y = vector.y;
    returnVector.z = vector.x;
    return returnVector;
}

