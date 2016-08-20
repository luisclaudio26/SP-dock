#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "./inc/graph/graph.h"
#include "./inc/descriptor/descriptor.h"
#include "./inc/io/fileio.h"
#include "./inc/visualization/render.h"

int main(int argc, char** args)
{
	std::string fname(args[1]);
	std::string vertfile(fname + ".vert");
	std::string facefile(fname + ".face");


	int patch_threshold = 8;
	if(argc > 2) patch_threshold = atoi( args[argc-1] );


	Graph target; std::vector< std::pair<Patch,Descriptor> > desc_target;
	FileIO::instance()->mesh_from_file(vertfile, facefile, target);
	target.preprocess_mesh(desc_target, patch_threshold);

	Graph ligand; std::vector< std::pair<Patch,Descriptor> > desc_ligand;
	FileIO::instance()->mesh_from_file(vertfile, facefile, ligand);
	ligand.preprocess_mesh(desc_target, patch_threshold);

	Render::instance()->draw_mesh(ligand);

	return 0;
}