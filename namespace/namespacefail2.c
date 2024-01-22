// Generated by ./nasty.py --nodes 2 --tasks 7 --nesting 3 --seed 154

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>




void func2(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
}
void func3(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
}
void func1(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
}
void func5(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
}
void func6(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
}
void func4(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    #pragma oss task in(a[961;1]) in(a[420;1]) weakin(a[960;1]) node(0) label("func5")
    func5(a);
}
void func0(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 1);

    #pragma oss task weakinout(a[928;19]) weakinout(a[355;40]) node(0) label("func1")
    func1(a);

    #pragma oss task weakin(a[935;12]) in(a[960;2]) weakin(a[398;23]) node(0) label("func4")
    func4(a);

}


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(1000 * sizeof(int));


    #pragma oss taskwait

    #pragma oss task weakout(a[287;152]) weakout(a[906;58]) node(1) label("func0")
    func0(a);
    
    #pragma oss taskwait
    

	nanos6_dfree(a, 1000 * sizeof(int));
	nanos6_lfree(b, 1000 * sizeof(int));

    
    printf("Test passed [./nasty.py --nodes 2 --tasks 7 --nesting 3 --seed 154]\n");
	
    return(0);
}
	