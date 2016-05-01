#include "../../inc/graph/node.h"

#include <sstream>

Node::Node()
{
	
}

Node::Node(const Vec4& pos)
{
	this->pos = pos;
}

void Node::push_triangular_face(int adj1, int adj2)
{
	this->ngbr.push_back( std::pair<int,int>(adj1, adj2) );
}

std::string Node::node2str()
{
	std::stringstream ss;

	ss<<"Node[ Pos = ("<<pos.x()<<", "<<pos.y()<<", "<<pos.z()<<"), ";
	for(auto it = ngbr.begin(); it != ngbr.end(); ++it)
		ss<<"("<<it->first<<", "<<it->second<<"), ";
	ss<<"]";

	return ss.str();
}
