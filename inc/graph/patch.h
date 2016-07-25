#ifndef _PATCH_H_
#define _PATCH_H_

#include <vector>

class Patch
{
private:
	

public:

	//temporarily make it public
	std::vector<int> nodes;

	Patch(const std::vector<int>& nodes);

	std::vector<int> get_nodes() const {
		return this->nodes;
	}
	
	//Don't if this is correct
	void operator=(const Patch& p)
	{
		this->nodes = p.get_nodes();
	}
};

#endif