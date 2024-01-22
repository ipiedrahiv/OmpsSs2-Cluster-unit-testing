/* * Copyright (C) 2019  Jimmy Aguilar Mena
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "util.h"
#include <stdio.h>

void init_AB(double *A, double *B, const size_t dim, size_t ts)
{
	const size_t numNodes = nanos6_get_num_cluster_nodes();
	assert_that(dim >= ts);
	assert_that(dim / ts >= numNodes);

	const size_t rowsPerNode = dim / numNodes;

	for (size_t i = 0; i < dim; i += rowsPerNode) { // loop nodes

		const int nodeid = i / rowsPerNode;

		#pragma oss task weakout(A[i * dim; rowsPerNode * dim])	\
			weakout(B[i; rowsPerNode])							\
			node(nodeid) wait label("init_AB_weak")
		{
			for (size_t j = i; j < i + rowsPerNode; j += ts) { // loop tasks

				#pragma oss task out(A[j * dim; ts * dim]) out(B[j; ts])									\
					node(nanos6_cluster_no_offload) label("init_AB_strong")
				{
					for (size_t k = j; k < j + ts; ++k) {
						double cum = 0.0, sum = 0.0;
						for (size_t l = 0; l < dim; ++l) {
							A[k * dim + l] = 0;
						}
					}
				}
			}
		}
	}
}

void init_x(double *x, const size_t dim, size_t ts)
{
	const size_t numNodes = nanos6_get_num_cluster_nodes();
	assert_that(dim >= ts);

	const size_t rowsPerNode = dim / numNodes;

	for (size_t i = 0; i < dim; i += rowsPerNode) { // loop nodes

		int nodeid = i / rowsPerNode;

		#pragma oss task weakout(x[i; rowsPerNode])		\
			node(nodeid) wait label("init_vec_weak")
		{
			for (size_t j = i; j < i + rowsPerNode; j += ts) { // loop tasks

				#pragma oss task out(x[j; ts])						\
					node(nanos6_cluster_no_offload) label("init_vec_strong")
				{
					for (size_t k = j; k < j + ts; ++k) {
						x[k] = 0.0;
					}
				}
			}
		}
	}
}

void jacobi_base(const double * A, double Bi,
                 const double *xin, double *xouti, size_t dim
) {
	xouti[0] = Bi;

	for (size_t j = 0; j < dim; ++j) {
		xouti[0] += A[j] * xin[j];
	}
}

void jacobi_tasks(const double *A, const double *B, double *xin, double *xout,
                  size_t ts, size_t dim, size_t it
) {

	const size_t numNodes = nanos6_get_num_cluster_nodes();
	assert_that(ts <= dim);

	const size_t rowsPerNode = dim / numNodes;
	assert_that(ts <= rowsPerNode);

	printf("Iteration %d\n", it);
	printf("A: %p\n", A);
	printf("B: %p\n", B);
	printf("xin: %p\n", xin);
	printf("xout: %p\n", xout);

	for (int i = dim - rowsPerNode; i >= 0; i -= rowsPerNode) {

		const int nodeid = i / rowsPerNode;

		#pragma oss task weakin(A[i * dim; rowsPerNode * dim])			\
			weakin(xin[0; dim])											\
			weakin(B[i; rowsPerNode])									\
			weakout(xout[i; rowsPerNode])								\
			node(nodeid) label("weakjacobi")
		{
			int rowsPerNuma = rowsPerNode / 2;
			for (int w = i; w < i + rowsPerNode; w += rowsPerNuma) {

				#pragma oss task for in(A[w * dim; rowsPerNuma * dim])	\
					in(xin[0; dim])										\
					in(B[w; rowsPerNuma])								\
					out(xout[w; rowsPerNuma])							\
					node(nanos6_cluster_no_offload) label("strongjacobi")
				for (size_t j = w; j < w + rowsPerNuma; j ++) {
					printf("node %d w %d j %d\n", nodeid, w, j);
					jacobi_base(&A[j * dim], B[j], xin, &xout[j], dim);
				}
			}

			#pragma oss task inout(xout[i; rowsPerNode])
			{
				printf("it %d Node %d\n", it, nodeid);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	const int ROWS = 16; //768;
	const int TS = 2;
	const int ITS = 2;

	printf("# Initializing data\n");

	double *A = nanos6_dmalloc(ROWS * ROWS * sizeof(double),
	                           nanos6_equpart_distribution, 0, NULL);

	double *B = nanos6_dmalloc(ROWS * sizeof(double),
	                           nanos6_equpart_distribution, 0, NULL);

	double *x1 = nanos6_dmalloc(ROWS * sizeof(double),
	                            nanos6_equpart_distribution, 0, NULL);

	double *x2 = nanos6_dmalloc(ROWS * sizeof(double),
	                            nanos6_equpart_distribution, 0, NULL);

	init_AB(A, B, ROWS, TS);
	init_x(x1, ROWS, TS);
	init_x(x2, ROWS, TS);

	#pragma oss taskwait
	printf("After first taskwait\n");

	jacobi_tasks(A, B, x1, x2, TS, ROWS, 0);
	jacobi_tasks(A, B, x2, x1, TS, ROWS, 1);

	#pragma oss taskwait
	printf("After second taskwait\n");

	nanos6_dfree(A, ROWS * ROWS * sizeof(double));
	nanos6_dfree(B, ROWS * sizeof(double));
	nanos6_dfree(x1, ROWS * sizeof(double));
	nanos6_dfree(x2, ROWS * sizeof(double));

	return 0;
}
