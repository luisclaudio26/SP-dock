#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

	return 0;
}