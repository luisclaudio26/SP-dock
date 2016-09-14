#ifndef _PATCH_H_
#define _PATCH_H_

#include <vector>
#include <glm/glm.hpp>
#include "./node.h"
#include "../descriptor/descriptor.h"

class Patch
{
private:
	
	glm::dvec3 normal;

	glm::dvec3 centroid;
public:
	//Temporarily public
	std::vector<int> nodes;

	Patch(const glm::dvec3& normal, const std::vector<int>& nodes);

	//-----------------------------------
	//----------- ACCESS STUFF ----------
	//-----------------------------------
	int patch_size() const { return nodes.size(); }

	//-----------------------------------
	//----------- OPERATIONS ------------
	//-----------------------------------
	Descriptor compute_descriptor(const std::vector<Node>& points);
	glm::dvec3 get_pos() const;
	glm::dvec3 get_normal() const;

	void paint_patch(std::vector<Node>& graph, const glm::vec3& color) const;
};

#endif