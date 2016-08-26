#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "./inc/graph/graph.h"
#include "./inc/descriptor/descriptor.h"
#include "./inc/io/fileio.h"
#include "./inc/visualization/render.h"

const int K = 5;
const int G_THRESH = 10;

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

	//create matching groups. This is a list of GROUPS, each GROUP
	//being a list of pairs <int,int>, where pair.first is the patch
	//in target and pair.second is the patch in ligand.
	std::vector<std::vector<std::pair<int,int> > > matching_groups;

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
				double dist = fabs(t_patch.second.curv - l_patch.second.curv) / std::max(t_patch.second.curv, l_patch.second.curv);
				similarity_list.push_back( std::make_pair(dist, l) );
			}
		}

		//sort similarity list according to distance. This will
		//give us a list of the patches in the LIGAND which are
		//most similar to t_patch (which is the current TARGET 
		//patch we're treating.
		std::sort(similarity_list.begin(), similarity_list.end());

		//get the K patches most similar to t_patch
		similarity_list.erase( similarity_list.begin() + K, similarity_list.end() );

		//try to put pairs in groups
		for(auto lig = similarity_list.begin(); lig != similarity_list.end(); ++lig)
		{
			bool added = false;

			//build the current pair we're treating
			std::pair<int,int> cur_pair = std::make_pair(t, lig->second);

			for(auto grp = matching_groups.begin(); grp != matching_groups.end(); ++grp)
			{
				bool grouping_crit = true;

				//check distances
				for(auto pair = grp->begin(); pair != grp->end(); ++pair)
				{
					if( rand() % 20 > G_THRESH ) grouping_crit = false;
					if( rand() % 20 > G_THRESH ) grouping_crit = false;
				}

				//if group criterion holds, push cur_pair to group
				if(grouping_crit)
				{
					grp->push_back( cur_pair );
					added = true;
				}
			}

			//if cur_pair was not added to any group, create a new group
			if(!added)
			{
				std::vector<std::pair<int,int> > new_group;
				new_group.push_back( cur_pair );
				matching_groups.push_back( new_group );
			}
		}
	}

	std::cout<<"Groups :"<<std::endl;
	for(auto grp = matching_groups.begin(); grp != matching_groups.end(); ++grp)
	{
		std::cout<<"---------------------------------------\n";
		for(auto pair = grp->begin(); pair != grp->end(); ++pair)
			std::cout<<"("<<pair->first<<", "<<pair->second<<"), ";
		std::cout<<std::endl<<std::endl;
	}

	return 0;
}