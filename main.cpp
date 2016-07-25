#include <iostream>
#include <string>
#include <vector>
#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/util/unionfind.h"
#include "./inc/visualization/render.h"

int main(int argc, char** args)
{
	std::string fname(args[1]);

	std::string vertfile(fname + ".vert");
	std::string facefile(fname + ".face");

	//Load molecular surface as generated from MSMS program
	Graph mesh_graph;
	FileIO::instance()->mesh_from_file(vertfile, facefile, mesh_graph);

	//Compute curvature for each point in mesh
	mesh_graph.compute_curvatures();

	//classify each point as convex/concave/flat
	mesh_graph.classify_points();

	//Get contiguous regions of convex/concave/flat points
	UnionFind uf( mesh_graph.size() );
	mesh_graph.segment_by_curvature(uf);

	//Extract feature points by expanding points until we
	//reach the borders
	std::vector<Patch> feature_points;
	mesh_graph.feature_points(uf, feature_points);

	//visualize mesh
	Render::instance()->draw_mesh( mesh_graph );

	//From here on, describe point, group-based matching,
	//ICP, scoring, etc.
	//std::cout<<mesh_graph.graph2str()<<std::endl;


	return 0;
}