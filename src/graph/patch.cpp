#include "../../inc/graph/patch.h"

Patch::Patch(const std::vector<int>& nodes)
{
	//capture patch by move semantics
	this->nodes = std::move(nodes);
}