#include "../../inc/graph/node.h"

#include <sstream>

Node::Node()
{
	
}

Node::Node(const Vec4& pos)
{
	this->pos = pos;
}

std::string Node::node2str()
{
	std::stringstream ss;

	ss<<"Node[ Pos = ("<<pos.x()<<", "<<pos.y()<<", "<<pos.z()<<") ]";

	return ss.str();
}
