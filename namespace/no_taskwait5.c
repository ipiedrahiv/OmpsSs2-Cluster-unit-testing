#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>
#include <pthread.h>

int main(void)
{
	// It may seem that the wait clause would mean that it is safe
	// for func1 to access func0's stack after it completes. But one
	// of the benefits of wait is to allow the stack to be reclaimed
	// as soon as the body completes. So it is actually invalid and an
	// error should be raised.
	#pragma oss task wait label("func0")
	{
		int b[10];

		#pragma oss task out(b[0]) node(2) label("func1")
		{
			usleep(100);
			printf("done\n");
		}

		// #pragma oss taskwait
	}

	#pragma oss taskwait
	
    return(0);
}
	
