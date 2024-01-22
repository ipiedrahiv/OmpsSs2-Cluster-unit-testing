#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>
#include <pthread.h>

int main(void)
{
	int b[10];

	// The "taskwait on" means that in principle this program is valid. But
	// currently the runtime raises the error "main: subtask accesses stack
	// after task completion (add taskwait)".

	#pragma oss task out(b[0]) node(2) label("func0")
	{
		usleep(100);
		printf("done\n");
	}

	#pragma oss taskwait on in(b[0])
	// #pragma oss taskwait
	
    return(0);
}
