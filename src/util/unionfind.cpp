#include "../../inc/util/unionfind.h"
#include <cstring>

UnionFind::UnionFind(unsigned int n_elements)
{
	this->n = n_elements;
	this->parent = new int[n_elements];
	for(int i = 0; i < n_elements; i++)
		this->parent[i] = i;
}

UnionFind::~UnionFind()
{
	delete[] parent;
}

int UnionFind::find(int a) const
{
	//TODO: error checking here
	if( parent[a] == a ) return a;
	else return find(parent[a]);
}

void UnionFind::merge(int a, int b)
{
	//no path compression or union-by-ranking or whatsoever
	int pa = find(a), pb = find(b);
	parent[pb] = pa;
}

void UnionFind::clusters(std::vector< std::vector<int> >& clusters) const
{
	int count = 0;
	int *hash = new int[this->n];
	memset(hash, -1, sizeof(int)*this->n);

	for(int i = 0; i < this->n; i++)
	{
		//get representative for the subset which
		//i belongs
		int s = find(i);

		//if this representative is not yet in the list,
		//i.e., its hash is -1, assign to it a fresh hash code
		//and push a new list to the list of lists
		if( hash[s] == -1 ) 
		{
			hash[s] = count++;
			clusters.push_back( std::vector<int>() );
		}

		//put i in the correct cluster
		clusters.at(hash[s]).push_back(i);
	}

	delete[] hash;
}
