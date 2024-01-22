// Generated by /home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 200 --nesting 2 --seed 6 --watchdog 15 --enable-file .simplify/simplify-control.txt
//10110111101100100000000000000000000000000000000000100000000000000000111110001100
//10011101011110110011011000000000000000001110101111001111000000000000000000110110
//011101011011101011110111000000000000000000001101110010011011111000


#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>
#include <pthread.h>

int watchdog_done = 0;

void *watchdog_func(void *x)
{
	for(int j=0;j<15;j++) {
		sleep(1);
		if (watchdog_done) {
			return NULL;
		}
	}
	printf("Watchdog timer expired (15 seconds)\n");
}

void func106(int *a)
{
}
void func107(int *a)
{
}
void func109(int *a)
{
}
void func110()
{
}
void func111()
{
}
void func112(int *a)
{
}
void func115()
{
}
void func116()
{
}
void func117()
{
}
void func118()
{
}
void func105(int *a)
{
    #pragma oss task out(a[209;71]) weakout(a[282;2]) node(0) label("func107")
    func107(a);
    
    #pragma oss task out(a[156;16]) node(2) wait label("func109")
    func109(a);
    
    //#pragma oss task  if(0) label("func110")
    //func110();
    
    #pragma oss task  node(2) label("func111")
    func111();
    
    #pragma oss task weakout(a[177;76]) if(0) label("func112")
    func112(a);
    
    #pragma oss task  node(2) label("func115")
    func115();
    
    #pragma oss task  node(0) wait label("func116")
    func116();
    
    #pragma oss task  node(0) label("func117")
    func117();
    
    #pragma oss task  if(0) label("func118")
    func118();
    
}
void func136(int *b)
{
}
void func137(int *a)
{
}
void func138(int *a)
{
}
void func140(int *b)
{
}
void func141(int *b)
{
}
void func142(int *b)
{
}
void func144(int *b)
{
}
void func146(int *a)
{
}
void func147(int *a,int *b)
{
}
void func149(int *b)
{
}
void func135(int *a,int *b)
{
    #pragma oss task inout(b[824;8]) node(2) label("func136")
    func136(b);
    
    #pragma oss task weakout(a[575;1]) node(1) label("func137")
    func137(a);
    
    #pragma oss task out(a[570;1]) if(0) label("func138")
    func138(a);
    
    #pragma oss task in(b[638;1]) node(2) label("func140")
    func140(b);
    
    #pragma oss task weakinout(b[840;2]) inout(b[638;1]) node(0) wait label("func141")
    func141(b);
    
    #pragma oss task weakin(b[638;1]) weakin(b[829;14]) out(b[639;1]) node(2) label("func142")
    func142(b);
    
    #pragma oss task in(b[638;1]) weakin(b[842;5]) node(1) wait label("func144")
    func144(b);
    
    #pragma oss task out(a[566;11]) weakout(a[577;2]) node(0) label("func146")
    func146(a);
    
    #pragma oss task out(a[575;1]) inout(b[638;1]) node(0) wait label("func147")
    func147(a,b);
    
    #pragma oss task weakinout(b[839;15]) node(1) wait label("func149")
    func149(b);
    
}
void func152()
{
}
void func154()
{
}
void func155()
{
}
void func156()
{
}
void func157()
{
}
void func159()
{
}
void func161()
{
}
void func150()
{
    #pragma oss taskwait noflush
    
    #pragma oss task  node(2) wait label("func152")
    func152();
    
    #pragma oss task  node(1) label("func154")
    func154();
    
    #pragma oss task  node(1) label("func155")
    func155();
    
    #pragma oss task  node(0) wait label("func156")
    func156();
    
    #pragma oss task  node(0) label("func157")
    func157();
    
    #pragma oss task  node(2) label("func159")
    func159();
    
    #pragma oss task  node(1) label("func161")
    func161();
    
}

void func180(int *a,int *b)
{
}


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(1000 * sizeof(int));

	printf("a:%p\nb:%p\n", a,b);

	pthread_t watchdog;
	if (pthread_create(&watchdog, NULL, watchdog_func, NULL)) {
		return 1;
	}
    
    #pragma oss task weakout(a[68;219]) node(0) wait label("func105")
    func105(a);
    
    #pragma oss task out(a[566;16]) weakinout(b[824;33]) out(b[638;2]) node(2) label("func135")
    func135(a,b);
    
    #pragma oss task  node(0) wait label("func150")
    func150();
    
    #pragma oss task weakin(b[839;18]) weakout(a[929;4]) node(2) wait label("func180")
    func180(a,b);
    
    #pragma oss taskwait

   nanos6_dfree(a, 1000 * sizeof(int));
   nanos6_lfree(b, 1000 * sizeof(int));
     watchdog_done = 1;
    pthread_join(watchdog, NULL);

    printf("Test passed [/home/bsc28/bsc28600/work/20201023-experimental/ompss-2-testgen/ompss-2-testgen --nodes 3 --tasks 200 --nesting 2 --seed 6 --watchdog 15 --enable-file .simplify/simplify-control.txt]\n");
	
    return(0);
}
	