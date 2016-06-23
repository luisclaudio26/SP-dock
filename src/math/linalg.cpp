#include "../../inc/math/linalg.h"

glm::dvec3 triangle_centroid(const glm::dvec3& p1, const glm::dvec3& p2, const glm::dvec3& p3)
{
	glm::dvec3 sum = (p1 + p2) + p3;
	return sum / 3.0;
}

glm::dvec3 cloud_centroid(const std::vector<glm::dvec3>& cloud)
{
	glm::dvec3 sum = glm::dvec3(0,0,0);
	for(auto it = cloud.begin(); it != cloud.end(); ++it)
		sum += (*it);

	return sum / (double)cloud.size();
}