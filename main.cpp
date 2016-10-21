#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <glm/gtx/string_cast.hpp>

#include "./inc/docker/docker.h"
#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/visualization/render.h"
#include "./inc/parameters.h"

int main(int argc, char** args)
{
	std::string fname(args[1]);
	std::string vertfile(fname + ".vert");
	std::string facefile(fname + ".face");

	//Process arguments
	if(argc > 2) Parameters::PATCH_SIZE_THRESH = atoi( args[2] );
	if(argc > 3) Parameters::N_BEST_PAIRS = atoi( args[3] );
	if(argc > 4) Parameters::G_THRESH = atoi( args[4] );

	//preprocess input molecules
	Graph target; SurfaceDescriptors desc_target;
	FileIO::instance()->mesh_from_file(vertfile, facefile, target);
	target.preprocess_mesh(desc_target);

	Graph ligand; SurfaceDescriptors desc_ligand;
	FileIO::instance()->mesh_from_file(vertfile, facefile, ligand);
	ligand.preprocess_mesh(desc_ligand);

	//build matching groups
	std::vector<MatchingGroup> matching_groups;
	Docker::instance()->build_matching_groups(desc_target, desc_ligand, matching_groups);

	//Render::instance()->draw_mesh( target );

	//build transformations matrices that align matching groups
	std::vector<glm::dmat4> mg_transformation;
	Docker::instance()->transformations_from_matching_groups(matching_groups, 
															target, desc_target, 
															ligand, desc_ligand, 
															mg_transformation);

	//docking phase: align cloud points according to calculated transformations
	ligand.set_base_color( glm::vec3(0.0, 0.7, 0.7) );
	target.set_base_color( glm::vec3(0.7, 0.7, 0.7) );

	for(auto trans = mg_transformation.begin(); trans != mg_transformation.end(); ++trans)
	{
		ligand.transform_cloud(*trans);
		std::cout<<glm::to_string(*trans)<<std::endl<<std::endl;
		Render::instance()->draw_meshes(ligand, target);
	}

	return 0;
}