#include "util.h"
#include <stdio.h>


int main(int argc, char **argv)
{
  int nodes = nanos6_get_num_cluster_nodes();
  char *ALL_MEMORY = NULL;
  int N = 60;
  int M = 10;
  int **A = nanos6_lmalloc(N*sizeof(int*)); 
  printf("A: %p\n", A);
  
  for(int k = 0; k < N; ++k)
  {
      A[k] = nanos6_lmalloc(M * sizeof(int));
		  A[k][0] = 100+k;
  }
	  // #pragma oss taskwait
  
	for(int k = 0; k < N; ++k)
	{
	  // #pragma oss task in(A[k]) weakin(ALL_MEMORY[1;-2]) node(1) label("T2")
	  #pragma oss task in(A[k]) in(A[0;N]) node(1) label("T2")
	  {
	  }
	}

  #pragma oss taskwait
  printf("Test passed\n");
  return 0;
}
