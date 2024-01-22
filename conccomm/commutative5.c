// Generated by /home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 1000 --nesting 4 --seed 5 --watchdog 45 --enable-file .simplify/simplify-control.txt
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000010000
//00000000000000000000000000000000000000000000000000000000000000000000000000000000
//00000000000000000000000000000000000000000000000000000000000000000000000000100000
//00011010110110000000000000000000000000000000000000000000000000000000000000000000
//0000000000000000000000000000


#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>
#include <pthread.h>


void func917(int *b)
{
}
void func918(int *b)
{
}
void func920(int *b)
{
}
void func921(int *b)
{
}
void func915(int *b)
{
    // #pragma oss taskwait noflush
    
    #pragma oss task weakin(b[370;1]) in(b[371;1]) node(2) wait label("func917")
    func917(b);
    
    #pragma oss task in(b[371;1]) in(b[370;1]) if(0) label("func918")
    func918(b);
    
    #pragma oss task weakin(b[371;1]) weakin(b[370;1]) node(1) wait label("func920")
    func920(b);
    
	// The problem is b[370]
    #pragma oss task in(b[370;1]) node(1) label("func921")
    func921(b);
    
}
void func907(int *b)
{
    // #pragma oss taskwait
    
    #pragma oss task in(b[371;1]) in(b[370;1]) node(0) wait label("func915")
    func915(b);
}
void func777(int *a,int *b)
{
    #pragma oss task weakin(b[369;2]) weakin(b[371;1]) node(0) wait label("func907")
    func907(b);
}


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(1000 * sizeof(int));
	printf("%p %p\n", a, b);

    #pragma oss task weakin(b[368;4]) weakout(a[4;22]) node(1) label("func777")
    func777(a,b);
    
    #pragma oss taskwait

   nanos6_dfree(a, 1000 * sizeof(int));
   nanos6_lfree(b, 1000 * sizeof(int));

    printf("Test passed [/home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 1000 --nesting 4 --seed 5 --watchdog 45 --enable-file .simplify/simplify-control.txt]\n");
	
    return(0);
}
	