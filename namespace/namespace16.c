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


	#pragma oss task weakinout(a[0;10]) node(1) label("1-weak")
	{
		// First offload task 1.1 to node 2
		#pragma oss task weakinout(a[0;10]) node(2) label("1.1-weak")
		{
			#pragma oss task inout(a[0;10]) node(2) label("1.1-strong")
			{
				assert_that(a[0] == 1);
				a[0] = 2;
			}
		}
		// Wait 2 seconds. In the meantime task 2-weak will offload
		// task 2.1 to node 2, which will go in the same namespace,
		// but it will not be propagated directly. This is OK. When
		// task 1.2-weak is offloaded to node 2 there must be no
		// remote propagation from task 2.1-weak to task 1.2-weak.
		sleep(2);

		#pragma oss task weakinout(a[0;10]) node(2) label("1.2-weak")
		{
			#pragma oss task inout(a[0;10]) node(2) label("1.2-strong")
			{
				assert_that(a[0] == 2);
				a[0] = 3;
			}
		}
	}

	#pragma oss task weakinout(a[0;10]) node(1) label("2-weak")
	{
		sleep(1);
		#pragma oss task weakinout(a[0;10]) node(2) label("2.1-weak")
		{
			#pragma oss task inout(a[0;10]) node(2) label("2.1-strong")
			{
				assert_that(a[0] == 3);
				sleep(1);
				a[0] = 4;
			}
		}
	}

	#pragma oss taskwait
	assert_that(a[0] == 4);
	printf("Finished main!\n");

    return 0;
}
