/*
 * Copyright (C) 2019  Jimmy Aguilar Mena
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
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <nanos6/debug.h>


double *alloc_init(const size_t rows, size_t cols, size_t TS)
{
	assert_that(rows >= TS);

	const size_t size = cols * rows;

	double *ret =
		(double *) nanos6_dmalloc(size * sizeof(double),
								  nanos6_equpart_distribution, 0, NULL);
	assert_that (ret);
	printf("Allocated at %p\n", ret);

	return ret;
}

void free_matrix(double *mat, size_t size)
{
	nanos6_dfree(mat, size * sizeof(double));
}



void matvec_tasks(const double *A, double **B, double *C,
                  size_t ts, size_t dim, size_t colsBC, size_t it, size_t numNodes)
{
	//printf("# matvec_tasks_node FETCHTASK %d\n", FETCHTASK);
	assert_that(ts <= dim);

	const size_t rowsPerNode = dim / numNodes;
	assert_that(ts <= rowsPerNode);

	size_t i = 0;

		#pragma oss task weakin(A[i * dim; rowsPerNode * dim])		\
			node(1) label("weakmatvec1")
		{
				#pragma oss task in(A[i * dim; ts * dim])		\
					node(1) label("strongmatvec1")
				{
					(void)ts;
					sleep(1);
				}
		}

		#pragma oss task weakin(A[i * dim; rowsPerNode * dim])		\
			node(1) label("weakmatvec2")
		{
				#pragma oss task in(A[i * dim; ts * dim])		\
					node(1) label("strongmatvec2")
				{
					(void)ts;
				}
		}
}


int main(int argc, char* argv[])
{
	const int ROWS = 16384;
	const int TS = 8192;
	const int ITS = 2;
	const size_t numNodes = 2;

	const size_t colsBC = 1;

	int TSinit = ROWS / numNodes;
	double *A = alloc_init(ROWS, ROWS, TSinit);      // this initialized by blocks ts x rows
	printf("A: %p\n", A);
	double **B = (double **)malloc(numNodes * sizeof(double *)); 
	for(int j=0; j<numNodes; j++) {
		B[j] = alloc_init(ROWS, colsBC, TSinit);    // this splits the array in ts
	}
	double *C = alloc_init(ROWS, colsBC, ROWS);  // This one initializes all the arrays

	matvec_tasks(A, B, C, TS, ROWS, colsBC, 0, numNodes);

	#pragma oss taskwait

	printf("# Finished algorithm...\n");

	free_matrix(A, ROWS * ROWS);
	free_matrix(B[0], ROWS * colsBC);
	free_matrix(C, ROWS * colsBC);

	return 0;
}
