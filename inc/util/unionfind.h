#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_

//Implement as a disjoint forest. The vector
//*parent stores the parent of each node.
//the root of a tree is such that it is its
//own parent.
class UnionFind
{
private:
	int* parent;

public:
	UnionFind(unsigned int n_elements);
	~UnionFind();

	int find(int a);
	void merge(int a, int b);
};

#endif