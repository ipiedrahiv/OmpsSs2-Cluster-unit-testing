// Generated by /home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 70 --nesting 4 --seed 18 --watchdog 25 --enable-file .simplify/simplify-control.txt
//00101100010100000000000000000000000000000000000000000000000000000000000000000000
//0000000000000000000000000

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>

void func2(int *b)
{
}
void func6(int *b)
{
}


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(1000 * sizeof(int));

	printf("hello\n");

    #pragma oss task out(b[235;1]) node(1) label("func2")
    func2(b);
    
    #pragma oss task weakin(b[235;1]) node(1) wait label("func6")
    func6(b);
    
    #pragma oss taskwait
    

   nanos6_dfree(a, 1000 * sizeof(int));
   nanos6_lfree(b, 1000 * sizeof(int));

    printf("Test passed [/home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 70 --nesting 4 --seed 18 --watchdog 25 --enable-file .simplify/simplify-control.txt]\n");
	
    return(0);
}
	
