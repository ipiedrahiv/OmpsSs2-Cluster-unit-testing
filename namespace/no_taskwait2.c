#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>
#include <pthread.h>

int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);

    #pragma oss task out(a[0]) node(2) wait label("func0")
	{
		usleep(100);
		// printf("done\n");
	}

	// #pragma oss taskwait
	nanos6_dfree(a, 1000 * sizeof(int));
	
    return(0);
}
	
