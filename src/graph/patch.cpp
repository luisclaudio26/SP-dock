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
static void build_vector_of_points(const std::vector<Node>& nodes, const std::vector<int>& patch, std::vector<glm::dvec3>& out)
{
	for(auto it = patch.begin(); it != patch.end(); ++it)
		out.push_back( nodes[*it].get_pos() );
}

static void least_evec_eval(const glm::dmat3 evec, 
							const glm::dvec3 eval, 
							glm::dvec3& least_evec,
							double& least_eval)
{
	int least_i = 0;
	for(int i = 0; i < 3; i++)
		if( eval[i] < eval[least_i] ) least_i = i;

	least_eval = least_i;
	least_evec = glm::column(evec, least_i);
}

static void principal_component_analysis(const std::vector<glm::dvec3>& points, 
									glm::dmat3& out_eigen_vec, 
									glm::dvec3& out_eigen_val)
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

	//eigendecompose covariance matrix
	gsl_eigen_symmv_workspace* eigen_aux = gsl_eigen_symmv_alloc(3);
	gsl_vector* eigen_val = gsl_vector_alloc(3);
	gsl_matrix* eigen_vec = gsl_matrix_alloc(3, 3);

	gsl_eigen_symmv(covar, eigen_val, eigen_vec, eigen_aux);

	//build final matrix -> TODO: do we need to transpose/invert?
	memcpy( glm::value_ptr(out_eigen_val), eigen_val->data, 3*sizeof(double) );	
	memcpy( glm::value_ptr(out_eigen_vec), eigen_vec->data, 9*sizeof(double) );

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

//TODO: So far, PCA is still useless, but we'll use it when aligning daisies
//so to compute DRINK descriptor.
Descriptor Patch::compute_descriptor(const std::vector<Node>& points)
{
	//build vector with point positions
	std::vector<glm::dvec3> p;
	build_vector_of_points(points, this->nodes, p);

	//compute patches centroid; remember PCA must be done when
	//mean of all points is zero
	this->centroid = cloud_centroid(p);

	//translate centroid to origin
	for(auto it = p.begin(); it != p.end(); ++it)
		*it = *it - centroid;

	//PCA of translated cloud point
	glm::dmat3 eigen_vec; glm::dvec3 eigen_val;
	glm::dvec3 least_evec; double least_eval;
	
	principal_component_analysis(p, eigen_vec, eigen_val);
	least_evec_eval(eigen_vec, eigen_val, least_evec, least_eval);

	//first, totally naïve descriptor: just store "curvature"
	//as the relative variance in the direction of the least
	//eigenvector, i.e., the least eigenvalue divided by total.
	//Signal is important to know whether patch is convex or concave.
	double total = 0.0; for(int i = 0; i < 3; i++) total += eigen_val[i];

	double curvature = least_eval / total;

	//Least eigenvalue doesn't necessarily point outside the
	//patch! (see Sorkine's book, pg. 55). This means we really need to
	//carry curvature information so to correctly compute descriptors!
	Convexity type = glm::dot( this->normal, this->curvature) > 0 ? CONCAVE : CONVEX;

	return (Descriptor){curvature, type};
}

glm::dvec3 Patch::get_pos() const { return this->centroid; }
glm::dvec3 Patch::get_normal() const { return this->normal; }
glm::dvec3 Patch::get_curvature() const { return this->curvature; }

void Patch::set_curvature(const glm::dvec3& c) { this->curvature = c; }