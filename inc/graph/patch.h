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

	//-----------------------------------
	//----------- ACCESS STUFF ----------
	//-----------------------------------
	int patch_size() const { return nodes.size(); }

	//-----------------------------------
	//----------- OPERATIONS ------------
	//-----------------------------------
	//Temporarily void! Should return the descriptor
	void compute_descriptor(const std::vector<Node>& points) const;

	void paint_patch(std::vector<Node>& graph, const glm::vec3& color) const;


	
};

#endif