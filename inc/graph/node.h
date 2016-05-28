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

	//-----------------------------------
	//--------- Access methods ----------
	//-----------------------------------
	void set_curvature(const Vec4& c);

	Vec4 get_pos() const { return this->pos; }

	int n_incident_faces() const { return ngbr.size(); }
	std::pair<int,int> get_face(int index) const;

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