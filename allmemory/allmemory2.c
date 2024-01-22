/*
 * Copyright (C) 2022  Paul Capenter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "util.h"
#include <stdio.h>


int main(int argc, char **argv)
{
  int nodes = nanos6_get_num_cluster_nodes();
  char *ALL_MEMORY = NULL;
  int N = 10;
  int **A = nanos6_lmalloc(N*sizeof(int*)); 
  
  int k = 0;
	#pragma oss task out(A[k]) weakinout(ALL_MEMORY[1;-2]) node(1) label("T1")
	{
	  int M = 10;
	  A[k] = (int *)nanos6_lmalloc(M * sizeof(int));
	  A[k][0] = 0;
	}

	k=1;
	#pragma oss task out(A[k]) weakinout(ALL_MEMORY[1;-2]) node(1) label("T2")
	{
	  int M = 10;
	  A[k] = (int *)nanos6_lmalloc(M * sizeof(int));
	  A[k][0] = 0;
	}
  
  #pragma oss taskwait
  printf("Task passed\n");
  return 0;
}
