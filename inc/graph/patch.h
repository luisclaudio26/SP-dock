#ifndef _PATCH_H_
#define _PATCH_H_

#include <vector>
#include <glm/glm.hpp>
#include "./node.h"

class Patch
{
private:
	std::vector<int> nodes;

public:
	Patch(const std::vector<int>& nodes);

	void paint_patch(std::vector<Node>& graph, const glm::vec3& color) const;

	int patch_size() const { return nodes.size(); }
	
};

#endif