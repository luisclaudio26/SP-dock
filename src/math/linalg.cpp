#include "../../inc/math/linalg.h"

glm::dvec3 triangle_centroid(const glm::dvec3& p1, const glm::dvec3& p2, const glm::dvec3& p3)
{
	glm::dvec3 sum = (p1 + p2) + p3;
	return sum / 3.0;
}
