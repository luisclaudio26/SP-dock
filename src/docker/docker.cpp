#include "../../inc/docker/docker.h"
#include <algorithm>
#include <iostream>

Docker* Docker::docker_ptr = 0;

//------------------------------------------------------
//--------------------- INTERNAL -----------------------
//------------------------------------------------------
static const int K = 5;
static const int G_THRESH = 2;

static int geodesic_distance(int lhs_patch_ind, int rhs_patch_ind, const SurfaceDescriptors& desc)
{
	const Patch& lhs_patch = desc[lhs_patch_ind].first;
	const Patch& rhs_patch = desc[rhs_patch_ind].first;

	//just compute Euclidean distance between two random points in patches
	glm::dvec3 n1 = lhs_patch.get_pos();
	glm::dvec3 n2 = rhs_patch.get_pos();

	return glm::length(n1-n2);
}

//-----------------------------------------------------------
//--------------------- FROM DOCKER.H -----------------------
//-----------------------------------------------------------
void Docker::build_matching_groups(const SurfaceDescriptors& desc_target, 
									const SurfaceDescriptors& desc_ligand, 
									std::vector<MatchingGroup>& groups_out) const
{
	//loop over all patches in target surface, get the
	//most similar and complementary patches from ligand,
	//then try to group it. 
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

			for(auto grp = groups_out.begin(); grp != groups_out.end(); ++grp)
			{
				bool grouping_crit = true;

				//check distances
				for(auto pair = grp->begin(); pair != grp->end(); ++pair)
				{
					if( geodesic_distance(cur_pair.first, pair->first, desc_target) > G_THRESH ) grouping_crit = false;
					if( geodesic_distance(cur_pair.second, pair->second, desc_ligand) > G_THRESH ) grouping_crit = false;
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
				MatchingGroup new_group;
				new_group.push_back( cur_pair );
				groups_out.push_back( new_group );
			}
		}
	}

	//maybe we can sort it and get only the best groups (with more pairs)
	std::cout<<"Groups :"<<std::endl;
	for(auto grp = groups_out.begin(); grp != groups_out.end(); ++grp)
	{
		std::cout<<"---------------------------------------\n";
		for(auto pair = grp->begin(); pair != grp->end(); ++pair)
			std::cout<<"("<<pair->first<<", "<<pair->second<<"), ";
		std::cout<<std::endl<<std::endl;
	}

	return;
}