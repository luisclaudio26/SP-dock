#ifndef _DOCKER_H_
#define _DOCKER_H_

#include <vector>
#include <utility>
#include "../descriptor/descriptor.h"
#include "../graph/patch.h"

typedef std::vector<std::pair<int,int> > 			MatchingGroup;
typedef std::vector<std::pair<Patch, Descriptor> >	SurfaceDescriptors;

class Docker
{
private:
	static Docker* docker_ptr;

public:
	static Docker* instance() {
		if(!Docker::docker_ptr)
			Docker::docker_ptr = new Docker();
		return Docker::docker_ptr;
	}

	//------------------------------
	//------ Main operations -------
	//------------------------------
	void build_matching_groups(const SurfaceDescriptors& desc_target,
								const SurfaceDescriptors& desc_ligand,
								std::vector<MatchingGroup>& groups_out) const;
};

#endif