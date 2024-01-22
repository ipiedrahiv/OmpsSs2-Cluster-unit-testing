#include "util.h"
#include <stdio.h>



int main(int argc, char *argv[])
{
	int u;
	#pragma oss task weakconcurrent(u) node(nanos6_cluster_no_offload) label("a")
	{
		#pragma oss task weakinout(u) node(nanos6_cluster_no_offload) label("b") 
		{
			#pragma oss task inout(u) node(nanos6_cluster_no_offload) label("d")
			{
			}
		}
	}

	#pragma oss taskwait

	return 0;
}
