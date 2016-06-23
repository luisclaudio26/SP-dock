#ifndef _LIN_ALG_H_
#define _LIN_ALG_G_

#include <glm/glm.hpp>
#include <cmath>
#include <vector>

const double EPS = 0.0000001f;
#define d_equals(a,b) (fabs(a-b) <= EPS ? true : false)

glm::dvec3 triangle_centroid(const glm::dvec3& p1, const glm::dvec3& p2, const glm::dvec3& p3);
glm::dvec3 cloud_centroid(const std::vector<glm::dvec3>& cloud);

#endif