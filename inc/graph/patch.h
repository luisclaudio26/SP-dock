#ifndef _PATCH_H_
#define _PATCH_H_

#include <vector>

class Patch
{
private:
	std::vector<int> nodes;

public:
	Patch(const std::vector<int>& nodes);

	//TODO: overload operator=(const Patch&&)!
};

#endif