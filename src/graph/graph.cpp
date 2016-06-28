#include "../../inc/graph/graph.h"
#include "../../inc/math/linalg.h"
#include "../../inc/util/unionfind.h"
#include <sstream>
#include <cstring>
#include <set>
#include <queue>

#include <iostream>
using std::cout;
using std::endl;

//-------------------------------------------------
//------------------- INTERNAL --------------------
//-------------------------------------------------
static void cluster_nodes_by_type(int current, bool visited[], const std::vector<Node>& nodes, UnionFind& UF)
{
	//if already visited, skip
	if(visited[current]) return;

	//retrieve current node
	const Node& cur = nodes.at(current);

	//mark as visited
	visited[current] = true;

	//loop through all adjacent nodes and union with every
	//one with the same convexity
	for(int i = 0; i < cur.n_incident_faces(); i++)
	{
		//get adjacent nodes in this face
		const std::pair<int,int> f = cur.get_face(i);
		const Node &f1 = nodes[f.first], &f2 = nodes[f.second];

		//merge if convexity is the same
		if( cur.get_type() == f1.get_type() )
			UF.merge(current, f.first);

		if( cur.get_type() == f2.get_type() )
			UF.merge(current, f.second);

		//recursively cluster
		cluster_nodes_by_type(f.first, visited, nodes, UF);
		cluster_nodes_by_type(f.second, visited, nodes, UF);
	}
}

static void get_contour_from_cluster(const std::vector<Node>& nodes, const std::vector<int>& cluster, std::set<int>& contour)
{
	for(auto p = cluster.begin(); p != cluster.end(); ++p)
	{
		//check whether every neighbour of this point has
		//the same convexity, i.e., is on the same region
		//(can we prove that two adjacent points have the
		//same convexity if and only if they lie on the
		//same cluster?)
		const Node &P = nodes.at(*p);
		bool in_contour = false;

		//loop through each face incident to P and check
		//whether they lie on the same region or not
		for(int i = 0; i < P.n_incident_faces(); i++)
		{
			const Node &n1 = nodes[P.get_face(i).first];
			const Node &n2 = nodes[P.get_face(i).second];

			if(n1.get_type() != P.get_type()) in_contour = true;
			if(n2.get_type() != P.get_type()) in_contour = true;
		}

		if(in_contour) contour.insert(*p);
	}
}

static int expand_node_in_breadth(const std::vector<Node>& nodes, const std::set<int>& contour, int node)
{
	bool *visited = new bool[contour.size()]; 	//TODO: not sure whether this is correct
	std::queue<std::pair<int,int> > Q;			//Pairs have form <NODE,DISTANCE> 
	Q.push( std::pair<int,int>(node, 0) );

	bool border_reached = false;

	while(!Q.empty())
	{
		//retrieve node info
		std::pair<int,int> cur = Q.front(); Q.pop();
		int dist = cur.second; int id = cur.first;

		//mark as visited
		visited[id] = true;

		//if N is on the contour, stop and return distance
		if( contour.count(id) > 0 ) return dist;

		//if not, push all neighbours to queue
		const Node& N = nodes[ id ];
		for(int i = 0; i < N.n_incident_faces(); i++)
		{
			int n1 = N.get_face(i).first;
			int n2 = N.get_face(i).second;

			if( !visited[n1] )
				Q.push( std::pair<int,int>(n1, dist+1) );
			
			if( !visited[n2] )
				Q.push( std::pair<int,int>(n2, dist+1) );
		}
	}

	//if we reach this point, something went wrong (our
	//in-breadth expansion did not reach the border ).
	return -1;
}

//-----------------------------------------------------
//------------------- FROM GRAPH.H --------------------
//-----------------------------------------------------
void Graph::push_node(double x, double y, double z)
{
	this->nodes.push_back( Node( glm::dvec3(x,y,z) ) );
}

void Graph::push_triangular_face(int node, int adj1, int adj2)
{
	this->nodes[node].push_triangular_face(adj1, adj2);
}

std::string Graph::graph2str()
{
	std::stringstream ss;

	ss<<"Graph[ ";
	for(auto it = nodes.begin(); it != nodes.end(); ++it)
		ss<<it->node2str()<<", \n";
	ss<<"]";
	
	return ss.str();
}

void Graph::compute_curvatures()
{
	for(auto n = nodes.begin(); n != nodes.end(); ++n)
	{
		glm::dvec3 acc;

		for(int i = 0; i < n->n_incident_faces(); i++)
		{
			std::pair<int,int> cur_face = n->get_face(i);
			
			glm::dvec3 P = n->get_pos();
			glm::dvec3 Q1 = nodes[cur_face.first].get_pos(); 
			glm::dvec3 Q2 = nodes[cur_face.second].get_pos();

			glm::dvec3 U = triangle_centroid(P, Q1, Q2);

			//Vectors point from current Node to the adjacent ones
			//and to the centroid of the face
			//HINT: some optimization could be made here storing
			//all the vectors we already computed. Is this worth?
			glm::dvec3 u = U - P, q1 = Q1 - P, q2 = Q2 - P;

			//This is the angle formed by the vectors which go from
			//the node to the adjacent ones
			double alpha = glm::dot(q1, q2) / (glm::length(q1) * glm::length(q2)) ;
			alpha = acos(alpha);

			acc = acc + (alpha * (u / glm::length(u)) );
		}

		n->set_curvature(acc);
	}
}

void Graph::classify_points()
{
	//TODO: THIS METHOD WON'T WORK AS WE DO IT NOW
	//unless centroid is inside mesh. Bruno's suggestion
	//was to discretize using voxels and then test
	//whether the normal vector points towards the center
	//of the implicit mesh
	std::vector<glm::dvec3> point_cloud;
	for(auto it = this->nodes.begin(); it != this->nodes.end(); ++it)
		point_cloud.push_back( it->get_pos() );

	glm::dvec3 centroid = cloud_centroid(point_cloud);
	for(auto it = this->nodes.begin(); it != this->nodes.end(); ++it)
	{
		double dot_centroid_curv = glm::dot(centroid - it->get_pos(), it->get_curvature());

		if( d_equals(dot_centroid_curv, 0.0) )
			it->set_convexity( FLAT );
		else if( dot_centroid_curv > 0)
			it->set_convexity( CONVEX );
		else
			it->set_convexity( CONCAVE );
	}
}

void Graph::segment_by_curvature(UnionFind& uf)
{
	bool *visited = new bool[this->nodes.size()];
	memset( visited, 0, sizeof(bool)*this->nodes.size() );

	//recursively cluster nodes
	cluster_nodes_by_type(0, visited, this->nodes, uf);

	delete[] visited;
}

void Graph::feature_points(const UnionFind& uf, std::vector<unsigned int>& feature)
{
	//cluster points by convexity
	std::vector< std::vector<int> > clusters;
	uf.clusters(clusters);

	//get feature points from each cluster
	for(auto region = clusters.begin(); region != clusters.end(); ++region)
	{
		//get contour for this cluster, i.e., the set of points on the border
		std::set<int> contour;
		get_contour_from_cluster(this->nodes, *region, contour);

		//rank points according to distance from border: expand in breadth
		//and stop when the first contour point is found. Store it with the
		//distance info.
		for(auto p = region->begin(); p != region->end(); ++p)
		{
			int dist_p = expand_node_in_breadth(this->nodes, contour, *p);
		}
	}


	/* Step 0: generate clusters of points
	 * Step 1: select a continuous region of surface points with same type
	 * Step 2: rank all region points according to their distance from
	 *			from the region contour and select those with the maximum
	 *			distance as seed points. <<<<< VERY IMPORTANT
	 * Step 3: expand each seed point uniformly to all directions along the surface
	 * 			until the region contour is reached.

	We must create a structure to hold each feature point, which is in fact the seed
	with all the points in neighbourhood. Feature point will be used by the descriptor

	how to define which points lie on the countour?
		-> points fully inside the region are such that ALL of its immediate neighbours
			do lie in the same region. The points of the contour are the ones which
			are not fully inside the region.

	how to rank the points according to distance?
		-> expand in breadth and stop when the first contour point is find. Store it
			with the distance info
	*/
}

