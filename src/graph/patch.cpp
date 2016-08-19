#include "../../inc/graph/patch.h"
#include "../../inc/math/linalg.h"

#include <cstdio>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_blas.h>

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

void Patch::compute_descriptor(const std::vector<Node>& points) const
{
	//3 rows, n columns
	int nrows = 3, ncolumns = this->nodes.size();
	gsl_matrix* data = gsl_matrix_alloc(nrows, ncolumns);

	//build vector with point positions
	std::vector<glm::dvec3> p;
	for(auto it = nodes.begin(); it != nodes.end(); ++it)
		p.push_back( points[*it].get_pos() );

	//compute patches centroid; remember PCA must be done when
	//mean of all points is zero
	glm::dvec3 centroid = cloud_centroid(p);

	//Build matrix where each column is one of the points in region
	for(int i = 0; i < p.size(); i++)
	{	
		glm::dvec3 translated_point = p[i] - centroid;

		//Set column i with px, py and pz
		gsl_matrix_set(data, 0, i, translated_point[0]);
		gsl_matrix_set(data, 1, i, translated_point[1]);
		gsl_matrix_set(data, 2, i, translated_point[2]);
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

	//delete pointers
	gsl_matrix_free(data); gsl_matrix_free(data_t); gsl_matrix_free(covar);
	gsl_matrix_free(eigen_vec); gsl_vector_free(eigen_val);
	gsl_eigen_symmv_free(eigen_aux);
}
