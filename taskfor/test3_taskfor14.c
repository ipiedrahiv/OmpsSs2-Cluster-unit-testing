/*
 * Copyright (C) 2021  Jimmy Aguilar Mena
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	 If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
	const int ROWS = 16384;

	int N = 40;
	double (*A) = nanos6_dmalloc(N * sizeof(double), nanos6_equpart_distribution, 0, NULL);

	assert(A != NULL);

	#pragma oss task for in(A[0;N]) node(nanos6_cluster_no_offload) label("A") priority(1)
	for (int w=0; w<N; w++) { }

	#pragma oss task for in(A[0;N]) node(nanos6_cluster_no_offload) label("B") priority(2)
	for (int w=0; w<N; w++) { }

	#pragma oss task for in(A[0;N]) node(nanos6_cluster_no_offload) label("C") priority(4)
	for (int w=0; w<N; w++) { }

	#pragma oss task for in(A[0;N]) node(nanos6_cluster_no_offload) label("D") priority(5)
	for (int w=0; w<N; w++) { }

	#pragma oss task for in(A[0;N]) node(nanos6_cluster_no_offload) label("E") priority(6)
	for (int w=0; w<N; w++) { }

	#pragma oss task for in(A[0;N]) node(nanos6_cluster_no_offload) label("F") priority(7)
	for (int w=0; w<N; w++) { }

	#pragma oss taskwait

	return 0;
}
