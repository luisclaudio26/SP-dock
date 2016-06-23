#include "../../inc/graph/node.h"
#include <iostream>
#include <sstream>

Node::Node()
{
	
}

Node::Node(const glm::dvec3& pos)
{
	this->pos = pos;
}

void Node::push_triangular_face(int adj1, int adj2)
{
	this->ngbr.push_back( std::pair<int,int>(adj1, adj2) );
}

std::pair<int,int> Node::get_face(int index) const
{
	try {
		//Skip if index is out of bounds
		if(index < 0 || index >= this->ngbr.size()) throw;

		return this->ngbr[index];
	} catch(int e) {
		std::cerr<<"Exception thrown while trying to retrieve a face adjacent to node: index out of bounds.\n";
	}
}


std::string Node::node2str()
{
	std::stringstream ss;

	ss<<"Node[ Pos = ("<<pos.x<<", "<<pos.y<<", "<<pos.z<<"), ";
	ss<<" Curvature = ("<<curvature.x<<", "<<curvature.y<<", "<<curvature.z<<") ["<<this->type<<"] -> adj: ";
	
	for(auto it = ngbr.begin(); it != ngbr.end(); ++it)
		ss<<"("<<it->first<<", "<<it->second<<"), ";
	
	ss<<"]";

	return ss.str();
}

void Node::set_curvature(const glm::dvec3& c)
{
	this->curvature = c;
}
