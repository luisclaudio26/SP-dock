#include <iostream>
#include <string>
#include <vector>
#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/util/unionfind.h"

int main(int argc, char** args)
{
	std::string vertfile("./data/test1.vert");
	std::string facefile("./data/test1.face");

	//Load molecular surface as generated from MSMS program
	Graph mesh_graph;
	FileIO::instance()->mesh_from_file(vertfile, facefile, mesh_graph);

	//Compute curvature for each point in mesh
	mesh_graph.compute_curvatures();

	//Get contiguous regions of convex/concave/flat points
	UnionFind uf( mesh_graph.size() );
	mesh_graph.segment_by_curvature(uf);

	//Extract feature points by expanding points until we
	//reach the borders
	std::vector<unsigned int> feature_points;
	mesh_graph.feature_points(uf, feature_points);

	//From here on, describe point, group-based matching,
	//ICP, scoring, etc.

	return 0;
}