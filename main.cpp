#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/util/unionfind.h"
#include "./inc/visualization/render.h"

bool under_threshold(const Patch& p)
{
	return p.nodes.size() < 8;
}

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


	//remove everyone below a certain threshold
	std::cout<<"Patches before: "<<feature_points.size()<<std::endl;
	
	feature_points.erase( std::remove_if(feature_points.begin(), feature_points.end(), under_threshold), 
							feature_points.end() );
	

	std::cout<<"Patches after: "<<feature_points.size()<<std::endl;

	//paint remaining patches
	int c = 0;
	for(auto p = feature_points.begin(); p != feature_points.end(); ++p)
	{
		for(auto it = p->nodes.begin(); it != p->nodes.end(); ++it)
		{
			float v = 1.0f;
			switch(c)
			{
				case 0: mesh_graph.get_nodes()[*it].set_color( glm::vec3(0.0f, v, 0.0f) ); break;
				case 1:	mesh_graph.get_nodes()[*it].set_color( glm::vec3(v, 0.0f, 0.0f) ); break;
				case 2:	mesh_graph.get_nodes()[*it].set_color( glm::vec3(0.0f, 0.0f, v) ); break;
			}
		}
	
		c++; if(c>2) c = 0;	
	}	

	//visualize mesh
	Render::instance()->draw_mesh( mesh_graph );

	//From here on, describe point, group-based matching,
	//ICP, scoring, etc.
	//std::cout<<mesh_graph.graph2str()<<std::endl;

	return 0;
}