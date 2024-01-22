#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>


void func13(int *b)
{
}
void func7(int *b)
{
    #pragma oss task inout(b[500;70]) node(2) label("A1")
    func13(b);
}
void func21(int *b)
{
}


int main(void)
{
	int *b = nanos6_lmalloc(1000 * sizeof(int));

    #pragma oss task weakinout(b[500;70]) node(0) label("A")
    func7(b);
    
    #pragma oss task weakin(b[500;70]) node(2) label("B")
    func21(b);
    
    #pragma oss taskwait
    nanos6_lfree(b, 1000 * sizeof(int));

    printf("Test passed\n");
	
    return(0);
}
	
