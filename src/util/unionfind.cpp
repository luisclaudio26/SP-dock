#include "../../inc/util/unionfind.h"

UnionFind::UnionFind(unsigned int n_elements)
{
	parent = new int[n_elements];
	for(int i = 0; i < n_elements; i++)
		parent[i] = i;
}

UnionFind::~UnionFind()
{
	delete[] parent;
}

int UnionFind::find(int a)
{
	if( parent[a] == a ) return a;
	else return find(parent[a]);
}

void UnionFind::merge(int a, int b)
{
	//no path compression or union-by-ranking or whatsoever
	int pa = find(a), pb = find(b);
	parent[pb] = pa;
}
