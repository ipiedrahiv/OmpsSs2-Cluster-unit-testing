// Generated by /home/bsc28/bsc28600/work/20210906-impi/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 70 --nesting 4 --seed 13942 --watchdog 45 --auto --no-hash --enable-file .simplify/simplify-control.txt
//00000000000000000000000000000000000000000000000000000000000000001010000000000000
//0000000100


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>

void func51(int *b,char *all)
{
    b[0] = 0x0921351b;
    #pragma oss task weakconcurrent(b[0]) node(1) label("func52")
    { }
    
    #pragma oss task weakconcurrent(b[0])  node(1) label("func68")
    { }
}


int main(void)
{
	int *b = nanos6_lmalloc(sizeof(int));
	char *all = (char *)NULL;

	printf("&b[0] = %p\n", &b[0]);
	b[0] = 0x8c15d90d;

    #pragma oss task inout(b[0;1]) node(1) label("func51")
    func51(b,all);
    
	// #pragma oss task in(b[0;1]) node(0) label("check")
	// {
	// 	printf("in check: b[0] = 0x%08x\n", b[0]);
	// }
    #pragma oss taskwait
	sleep(1);
    
	int ref = 0x0921351b;	
	const char *eq = (b[0] == ref) ? "==" : "!=";
	printf("b[0]: 0x%08x %s 0x%08x\n", b[0], eq, ref);
	assert(b[0] == ref);

   nanos6_lfree(b, sizeof(int));

    printf("Test passed [/home/bsc28/bsc28600/work/20210906-impi/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 70 --nesting 4 --seed 13942 --watchdog 45 --auto --no-hash --enable-file .simplify/simplify-control.txt]\n");
	
    return(0);
}
	