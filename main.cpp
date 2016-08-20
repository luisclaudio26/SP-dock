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
	ligand.preprocess_mesh(desc_ligand, patch_threshold);

	//1) Calcule a dissimilaridade entre todos os pares
	//de patch do target e do ligand
	for(int t = 0; t < desc_target.size(); ++t)
	{
		const std::pair<Patch,Descriptor> &t_patch = desc_target[t];

		//This list stores pairs <D,I>, where I is the index of a
		//LIGAND patch and D is the dissimilary distance between
		//the current TARGET patch and I. It is inverted so we
		//can use lexicographical order for sorting
		std::vector< std::pair<double,int> > similarity_list;

		for(int l = 0; l < desc_ligand.size(); ++l)
		{	
			const std::pair<Patch,Descriptor> &l_patch = desc_ligand[l];
			if(t_patch.second.type * l_patch.second.type == -1)
			{
				double dist = fabs(t_patch.second.curv - l_patch.second.curv);
				similarity_list.push_back( std::make_pair(dist, l) );
			}
		}

		//sort similarity list according to distance. This will
		//give us a list of the patches in the LIGAND which are
		//most similar to t_patch (which is the current TARGET 
		//patch we're treating.
		std::sort(similarity_list.begin(), similarity_list.end());

		std::cout<<"Ranked distances from "<<t<<" : "<<std::endl;
		for(auto it = similarity_list.begin(); it != similarity_list.end(); ++it)
			std::cout<<"	to "<<it->second<<" : "<<it->first<<std::endl;
	}

	return 0;
}