#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <utility>
#include "convexity.h"
#include "../math/vec4.h"

class Node
{
private:
	Vec4 pos, curvature;
	Convexity type;

	std::vector<std::pair<Node,Node> > ngbr;

public:
};

#endif