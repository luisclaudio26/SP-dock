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

	//We store the indices to the neighbours
	std::vector<std::pair<int, int> > ngbr;

public:
	Node();
	Node(const Vec4& pos);

	void push_triangular_face(int adj1, int adj2);

	std::string node2str();
};

#endif