// Generated by /home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 55 --nesting 2 --seed 16 --watchdog 25 --enable-file .simplify/simplify-control.txt
//0000000000000000000000000100000000000000000000010000100000000000


#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>


void func20(int *b)
{
}
void func44(int *b)
{
}
void func40(int *a,int *b)
{
    #pragma oss task commutative(b[987;1]) node(2) label("func44")
    func44(b);
    
}


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(1000 * sizeof(int));
	char *all = (char *)NULL;

    #pragma oss task weakin(b[987;1]) node(2) label("func20")
    func20(b);
    
    #pragma oss task weakinout(b[987;1]) node(2) label("func40")
    func40(a,b);
    
    #pragma oss taskwait

   nanos6_dfree(a, 1000 * sizeof(int));
   nanos6_lfree(b, 1000 * sizeof(int));

    printf("Test passed [/home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 55 --nesting 2 --seed 16 --watchdog 25 --enable-file .simplify/simplify-control.txt]\n");
	
    return(0);
}
	
