#include "../../inc/graph/graph.h"
#include "../../inc/math/linalg.h"
#include <sstream>

void Graph::compute_curvatures()
{
	for(auto n = nodes.begin(); n != nodes.end(); ++n)
	{
		Vec4 acc;

		for(int i = 0; i < n->n_incident_faces(); i++)
		{
			std::pair<int,int> cur_face = n->get_face(i);
			
			Vec4 P = n->get_pos();
			Vec4 Q1 = nodes[cur_face.first].get_pos(); 
			Vec4 Q2 = nodes[cur_face.second].get_pos();

			Vec4 U = triangle_centroid(P, Q1, Q2);

			//Vectors point from current Node to the adjacent ones
			//and to the centroid of the face
			//HINT: some optimization could be made here storing
			//all the vectors we already computed. Is this worth?
			Vec4 u = U - P, q1 = Q1 - P, q2 = Q2 - P;

			//This is the angle formed by the vectors which go from
			//the node to the adjacent ones
			double alpha = q1.dot(q2) * (1.0/ (q1.norm() * q2.norm()) ) ;
			alpha = acos(alpha);

			acc = acc + (u.normalize() * alpha);
		}

		n->set_curvature(acc);
	}
}

void Graph::segment_by_curvature(UnionFind& uf)
{
	
}

void Graph::feature_points(const UnionFind& uf, std::vector<unsigned int>& feature)
{
	
}

void Graph::push_node(double x, double y, double z)
{
	this->nodes.push_back( Node( Vec4(x,y,z,1.0) ) );
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