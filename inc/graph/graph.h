#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include "node.h"
#include "../util/unionfind.h"

class Graph
{
private:
	std::vector<Node> nodes;

public:

	//---------------------------------
	//--------- Access method ---------
	//---------------------------------
	unsigned int size() {
		return nodes.size();
	}

	//-------------------------------
	//--------- Operations ----------
	//-------------------------------
	//These operations change the internal
	//state of the Graph
	void compute_curvatures();
	void segment_by_curvature(UnionFind& uf);
	void feature_points(const UnionFind& uf, std::vector<unsigned int>& feature);
};

#endif