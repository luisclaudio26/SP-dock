#ifndef _PATCH_H_
#define _PATCH_H_

#include <vector>
#include <glm/glm.hpp>
#include "./node.h"
#include "../descriptor/descriptor.h"

class Patch
{
private:
	std::vector<int> nodes;
	glm::dvec3 normal;

public:
	Patch(const glm::dvec3& normal, const std::vector<int>& nodes);

	//-----------------------------------
	//----------- ACCESS STUFF ----------
	//-----------------------------------
	int patch_size() const { return nodes.size(); }

	//-----------------------------------
	//----------- OPERATIONS ------------
	//-----------------------------------
	//Temporarily void! Should return the descriptor
	Descriptor compute_descriptor(const std::vector<Node>& points) const;

	void paint_patch(std::vector<Node>& graph, const glm::vec3& color) const;
};

#endif