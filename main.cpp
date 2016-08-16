#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/util/unionfind.h"
#include "./inc/visualization/render.h"

int main(int argc, char** args)
{
	std::string fname(args[1]);
	std::string vertfile(fname + ".vert");
	std::string facefile(fname + ".face");


	int patch_threshold = 8;
	if(argc > 2) patch_threshold = atoi( args[argc-1] );


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
	mesh_graph.feature_points(uf, feature_points, patch_threshold);

	//visualize mesh
	Render::instance()->draw_mesh( mesh_graph );

	return 0;
}