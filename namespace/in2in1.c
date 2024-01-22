#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int *a = nanos6_lmalloc(10*sizeof(int));
	a[0] = 1;

	for(int j=0; j<20; j++) {
		#pragma oss task weakin(a[0;10]) node(1) label("A")
		{ }

		#pragma oss task weakin(a[0;10]) node(2) label("B")
		{ }
	}

	#pragma oss taskwait

    return 0;
}
