#include <iostream>
#include <string>
#include <vector>
#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/util/unionfind.h"
#include "./inc/visualization/render.h"

//We need to embed some visualization system
//to this so to know whether the point classification,
//docking, etc. make sense. 
//	-> Embed a simple renderer system with OpenGL,
//		load the mesh triangles and render everything

int main(int argc, char** args)
{
	std::string vertfile("./data/test1.vert");
	std::string facefile("./data/test1.face");

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