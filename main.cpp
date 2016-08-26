#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "./inc/docker/docker.h"
#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/visualization/render.h"


int main(int argc, char** args)
{
	std::string fname(args[1]);
	std::string vertfile(fname + ".vert");
	std::string facefile(fname + ".face");

	//Process arguments
	int patch_threshold = 8;
	if(argc > 2) patch_threshold = atoi( args[argc-1] );

	//preprocess input molecules
	Graph target; SurfaceDescriptors desc_target;
	FileIO::instance()->mesh_from_file(vertfile, facefile, target);
	target.preprocess_mesh(desc_target, patch_threshold);

	Graph ligand; SurfaceDescriptors desc_ligand;
	FileIO::instance()->mesh_from_file(vertfile, facefile, ligand);
	ligand.preprocess_mesh(desc_ligand, patch_threshold);

	//build matching groups
	std::vector<MatchingGroup> matching_groups;
	Docker::instance()->build_matching_groups(desc_target, desc_ligand, matching_groups);

	return 0;
}