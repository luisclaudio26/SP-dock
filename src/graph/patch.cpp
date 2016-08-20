#include "../../inc/graph/patch.h"
#include "../../inc/math/linalg.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_blas.h>

//-------------------------------------------------------------------
//-------------------------- INTERNAL -------------------------------
//-------------------------------------------------------------------
void build_vector_of_points(const std::vector<Node>& nodes, const std::vector<int>& patch, std::vector<glm::dvec3>& out)
{
	for(auto it = patch.begin(); it != patch.end(); ++it)
		out.push_back( nodes[*it].get_pos() );
}

void principal_component_analysis(const std::vector<glm::dvec3>& points, glm::dmat3& out_eigen_vec, glm::dvec3& out_eigen_val)
{
	//3 rows, n columns
	int nrows = 3, ncolumns = points.size();
	gsl_matrix* data = gsl_matrix_alloc(nrows, ncolumns);

	//Build matrix where each column is one of the points in region
	for(int i = 0; i < ncolumns; i++)
	{	
		//Set column i with px, py and pz
		gsl_matrix_set(data, 0, i, points[i][0]);
		gsl_matrix_set(data, 1, i, points[i][1]);
		gsl_matrix_set(data, 2, i, points[i][2]);
	}

	//transpose matrix
	gsl_matrix* data_t = gsl_matrix_alloc(ncolumns, nrows);
	gsl_matrix_transpose_memcpy(data_t, data);

	//multiply matrices
	gsl_matrix* covar = gsl_matrix_calloc(3, 3);
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, data, data_t, 0.0, covar);
	
	//eigen decompose covariance matrix
	gsl_eigen_symmv_workspace* eigen_aux = gsl_eigen_symmv_alloc(3);
	gsl_vector* eigen_val = gsl_vector_alloc(3);
	gsl_matrix* eigen_vec = gsl_matrix_alloc(3, 3);

	gsl_eigen_symmv(covar, eigen_val, eigen_vec, eigen_aux);

	//sort eigenvectors by eigenvalues
	gsl_eigen_symmv_sort(eigen_val, eigen_vec, GSL_EIGEN_SORT_VAL_DESC);

	//build final matrix
	memcpy( glm::value_ptr(out_eigen_val), eigen_val->data, 3*sizeof(double) );	
	memcpy( glm::value_ptr(out_eigen_vec), eigen_vec->data, 9*sizeof(double) );
	
	//transpose maps from origin to space defined by the three eigenvectors
	out_eigen_vec = glm::transpose(out_eigen_vec);

	//delete pointers
	gsl_matrix_free(data); gsl_matrix_free(data_t); gsl_matrix_free(covar);
	gsl_matrix_free(eigen_vec); gsl_vector_free(eigen_val);
	gsl_eigen_symmv_free(eigen_aux);
}

//-----------------------------------------------------------------------
//-------------------------- FROM PATCH.H -------------------------------
//-----------------------------------------------------------------------
Patch::Patch(const glm::dvec3& normal, const std::vector<int>& nodes)
{
	this->normal = normal;

	//capture patch by move semantics
	this->nodes = std::move(nodes);
}

void Patch::paint_patch(std::vector<Node>& graph, const glm::vec3& color) const
{
	for(auto n = this->nodes.begin(); n != this->nodes.end(); ++n)
		graph[*n].set_color(color);
}

Descriptor Patch::compute_descriptor(const std::vector<Node>& points) const
{
	//build vector with point positions
	std::vector<glm::dvec3> p;
	build_vector_of_points(points, this->nodes, p);

	//compute patches centroid; remember PCA must be done when
	//mean of all points is zero
	glm::dvec3 centroid = cloud_centroid(p);

	//translate centroid to origin
	for(auto it = p.begin(); it != p.end(); ++it)
		*it = *it - centroid;

	//PCA of translated cloud point
	glm::dmat3 eigen_vec; glm::dvec3 eigen_val;
	principal_component_analysis(p, eigen_vec, eigen_val);

	//send all points to the eigenvectors basis
	for(auto it = p.begin(); it != p.end(); ++it)
		*it = eigen_vec * (*it);

	//first, totally naïve descriptor: just store "curvature"
	//as the relative variance in the direction of the least
	//eigenvector, i.e., the least eigenvalue divided by total.
	//Signal is important to know whether patch is convex or concave.
	double total = 0.0; for(int i = 0; i < 3; i++) total += eigen_val[i];

	double curvature = eigen_val[2] / total;
	int type = glm::dot( glm::row(eigen_vec, 2), this->normal) < 0 ? 1 : -1;

	return (Descriptor){curvature, type};
}
