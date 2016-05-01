#include "../../inc/graph/graph.h"

#include <sstream>

void Graph::compute_curvatures()
{
	
}

void Graph::segment_by_curvature(UnionFind& uf)
{
	
}

void Graph::feature_points(const UnionFind& uf, std::vector<unsigned int>& feature)
{
	
}

void Graph::push_node(double x, double y, double z)
{
	this->nodes.push_back( Node( Vec4(x,y,z,1.0) ) );
}

std::string Graph::graph2str()
{
	std::stringstream ss;

	ss<<"Graph[ ";
	for(auto it = nodes.begin(); it != nodes.end(); ++it)
		ss<<it->node2str()<<", \n";
	ss<<"]";
	
	return ss.str();
}