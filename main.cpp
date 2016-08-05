#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

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
	high_resolution_clock::time_point t_start = high_resolution_clock::now();
	mesh_graph.compute_curvatures();

	//classify each point as convex/concave/flat
	high_resolution_clock::time_point t_classify = high_resolution_clock::now();
	mesh_graph.classify_points();

	//Get contiguous regions of convex/concave/flat points
	high_resolution_clock::time_point t_segment = high_resolution_clock::now();
	UnionFind uf( mesh_graph.size() );
	mesh_graph.segment_by_curvature(uf);

	//Extract feature points by expanding points until we
	//reach the borders
	high_resolution_clock::time_point t_featurepoints = high_resolution_clock::now();
	std::vector<Patch> feature_points;
	mesh_graph.feature_points(uf, feature_points);

	//remove everyone below a certain threshold	
	int points_before = feature_points.size();

	high_resolution_clock::time_point t_remove = high_resolution_clock::now();

	int one_point_only = 0;
	for(auto p = feature_points.begin(); p != feature_points.end(); ++p)
		if( p->nodes.size() == 1 ) one_point_only++;

	feature_points.erase( std::remove_if(feature_points.begin(), feature_points.end(), under_threshold), 
							feature_points.end() );
	
	int points_after = feature_points.size();

	//finish main processing
	high_resolution_clock::time_point t_finish = high_resolution_clock::now();

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

	//print info
	milliseconds curvatures_t = duration_cast<milliseconds>(t_classify - t_start);
	milliseconds classify_t = duration_cast<milliseconds>(t_segment - t_classify);
	milliseconds segment_t = duration_cast<milliseconds>(t_featurepoints - t_segment);
	milliseconds featurepoints_t = duration_cast<milliseconds>(t_remove - t_featurepoints);
	milliseconds removal_t = duration_cast<milliseconds>(t_finish - t_remove);

	/*
	std::cout<<"-------------------------------------------------------------------\n"<<std::endl;
	std::cout<<"File: "<<fname<<std::endl<<std::endl;
	std::cout<<"Curvature computation time : "<<curvatures_t.count()<<" ms\n";
	std::cout<<"Classification time : "<<classify_t.count()<<" ms\n";
	std::cout<<"Segmentation time : "<<segment_t.count()<<" ms\n";
	std::cout<<"Feature point extraction time : "<<featurepoints_t.count()<<" ms\n";
	std::cout<<"Spurious point removal time : "<<removal_t.count()<<" ms\n\n";

	std::cout<<"Patches before : "<<points_before<<", after filtering : "<<points_after<<std::endl;
	std::cout<<"Number of patches with one point only : "<<one_point_only<<std::endl;
	std::cout<<(points_before-points_after)*100.0f/points_before<<"\% of patches with < 10 points"<<std::endl;
	std::cout<<(one_point_only)*100.0/points_before<<"\% of patches with one poine only\n"<<std::endl; */

	std::cout<<fname<<" ";
	std::cout<<featurepoints_t.count()<<" ";
	std::cout<<points_before<<" "<<points_after<<" ";
	std::cout<<(points_before-points_after)/(double)points_before<<" ";
	std::cout<<(one_point_only)/(double)points_before<<" ";
	std::cout<<std::endl;

	//visualize mesh
	//Render::instance()->draw_mesh( mesh_graph );

	return 0;
}