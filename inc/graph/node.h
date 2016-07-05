#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <string>
#include "convexity.h"

class Node
{
private:
	glm::dvec3 pos, curvature, normal;
	Convexity type;

	//We store the indices to the neighbours
	std::vector<std::pair<int, int> > ngbr;

public:
	Node();
	Node(const glm::dvec3& pos, const glm::dvec3& normal);

	//-----------------------------------
	//--------- Access methods ----------
	//-----------------------------------
	glm::dvec3 get_curvature() const { return this->curvature; }
	glm::dvec3 get_pos() const { return this->pos; }
	glm::dvec3 get_normal() const { return this->normal; }
	Convexity get_type() const { return this->type; }

	int n_incident_faces() const { return ngbr.size(); }
	std::pair<int,int> get_face(int index) const;

	void set_curvature(const glm::dvec3& c);
	void set_convexity(const Convexity& type) { this->type = type; }

	//-------------------------------
	//--------- Operations ----------
	//-------------------------------
	//These operations change the internal state of the node
	void push_triangular_face(int adj1, int adj2);

	//--------------------------------
	//-------- Debugging ops ---------
	//--------------------------------
	std::string node2str();
};

#endif