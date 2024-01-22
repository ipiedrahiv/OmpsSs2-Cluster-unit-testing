#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>


int main(void)
{
	int n = nanos6_get_num_cluster_nodes();
	int *b = nanos6_lmalloc(n * sizeof(int));

	// Initialize part of b on each node
	for(int k=0; k<n; k++) {
		#pragma oss task out(b[k;1]) node(k)
		{
			b[k] = k;
		}
	}

	#pragma oss task weakinout(b[0;n]) node(0) wait
	{
	}
	
	// Read whole of b on node 1: how many satisfiability messages
	#pragma oss task in(b[0;n]) node(1)
	{
		for(int k=0;k<n;k++) {
			assert_that(b[k] == k);
		}
	}


    #pragma oss taskwait
    
   nanos6_lfree(b, n * sizeof(int));

    return(0);
}
	
