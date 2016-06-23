#include "../../inc/graph/graph.h"
#include "../../inc/math/linalg.h"
#include <sstream>

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
	
}

void Graph::feature_points(const UnionFind& uf, std::vector<unsigned int>& feature)
{
	
}

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