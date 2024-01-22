#include "util.h"
#include <stdio.h>


int main(int argc, char **argv)
{
  int nodes = nanos6_get_num_cluster_nodes();
  char *ALL_MEMORY = NULL;
  int N = 10;
  int **A = nanos6_lmalloc(N*sizeof(int*)); 
  
  for(int k = 0; k < N; ++k)
  {
    #pragma oss task out(A[k]) weakinout(ALL_MEMORY[1;-2]) node(k%nodes) label("T1")
    {
      int M = 10;
      A[k] = (int *)nanos6_lmalloc(M * sizeof(int));
      A[k][0] = 123;
    }
  }
  
  #pragma oss task in(A[0]) weakin(ALL_MEMORY[1;-2]) node(1) label("T2")
  {
  	#pragma oss task in(A[0]) in(A[0][0;10]) node(nanos6_cluster_no_offload) label("T2a")
    assert_that(A[0][0] == 123);
  }

  #pragma oss taskwait
  printf("Test passed\n");
  return 0;
}
