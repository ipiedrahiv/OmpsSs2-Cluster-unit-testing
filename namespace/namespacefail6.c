// Generated by ./nasty.py --nodes 3 --tasks 18 --nesting 5 --seed 144

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>


void task_a(int *b)
{
}

void task_b(int *b)
{
    #pragma oss task in(b[493;7]) weakout(b[416;3]) node(2) label("a")
    task_a(b);

	#pragma oss taskwait
	printf("1\n");
}

void task_d(int *b)
{
   #pragma oss task out(b[487;45]) out(b[415;6]) node(1) label("c")
   task_b(b);

   #pragma oss taskwait
   printf("done func0 taskwait\n");
}


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(1000 * sizeof(int));

    #pragma oss task out(b[347;461]) node(2) label("d")
    task_d(b);
    
    #pragma oss taskwait

	nanos6_dfree(a, 1000 * sizeof(int));
	nanos6_lfree(b, 1000 * sizeof(int));

    printf("Test passed [./nasty.py --nodes 3 --tasks 18 --nesting 5 --seed 144]\n");
	
    return(0);
}
	