#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_

#include <vector>

//Implement as a disjoint forest. The vector
//*parent stores the parent of each node.
//the root of a tree is such that it is its
//own parent.
class UnionFind
{
private:
	int* parent;
	int n;

public:
	UnionFind(unsigned int n_elements);
	~UnionFind();

	//classical operations
	int find(int a);
	void merge(int a, int b);

	//extra-operation: return a list of lists,
	//each one with a cluster (a disjoint set)
	void clusters(std::vector< std::vector<int> >& clusters);
};

#endif