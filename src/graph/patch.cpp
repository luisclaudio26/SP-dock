#include "../../inc/graph/patch.h"

Patch::Patch(const std::vector<int>& nodes)
{
	//capture patch by move semantics
	this->nodes = std::move(nodes);
}

void Patch::paint_patch(std::vector<Node>& graph, const glm::vec3& color) const
{
	for(auto n = this->nodes.begin(); n != this->nodes.end(); ++n)
		graph[*n].set_color(color);
}