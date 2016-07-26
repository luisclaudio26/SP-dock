#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include "node.h"
#include "../util/unionfind.h"
#include "./patch.h"

typedef struct {
	int a, b, c;
} Face;

class Graph
{
private:
	std::vector<Node> nodes;
	std::vector<Face> faces; //Used for rendering only (so far)

public:

	//---------------------------------
	//--------- Access method ---------
	//---------------------------------
	unsigned int size() const { return nodes.size(); }

	void push_node(double x, double y, double z, double nx, double ny, double nz);
	void push_face(int a, int b, int c);

	//TODO: why isn't cbegin() working!?
	std::vector<Node>& get_nodes() { return nodes; }
	const std::vector<Face>& get_faces() const { return faces; }

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