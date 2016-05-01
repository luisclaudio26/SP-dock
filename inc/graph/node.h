#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <utility>
#include "convexity.h"
#include "../math/vec4.h"

#include <string>

class Node
{
private:
	Vec4 pos, curvature;
	Convexity type;

	std::vector<std::pair<Node,Node> > ngbr;

public:
	Node();
	Node(const Vec4& pos);

	std::string node2str();
};

#endif