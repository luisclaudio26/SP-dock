#include "../../inc/math/linalg.h"

Vec4 triangle_centroid(const Vec4& p1, const Vec4& p2, const Vec4& p3)
{
	Vec4 sum = (p1 + p2) + p3;
	return sum * (1.0/3.0);
}
