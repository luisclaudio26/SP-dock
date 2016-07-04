#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include "node.h"
#include "../util/unionfind.h"
#include "./patch.h"

class Graph
{
private:
	std::vector<Node> nodes;

public:

	//---------------------------------
	//--------- Access method ---------
	//---------------------------------
	unsigned int size() { return nodes.size(); }

	void push_node(double x, double y, double z);
	void push_triangular_face(int node, int adj1, int adj2);

	//TODO: why isn't cbegin() working!?
	std::vector<Node>::const_iterator get_nodes() { return nodes.begin(); }

	//-------------------------------
	//--------- Operations ----------
	//-------------------------------
	//These operations change the internal
	//state of the Graph
	void compute_curvatures();
	void classify_points();
	void segment_by_curvature(UnionFind& uf);
	void feature_points(const UnionFind& uf, std::vector<Patch>& feature);

	//--------------------------------
	//-------- Debugging ops ---------
	//--------------------------------
	std::string graph2str();
};

#endif