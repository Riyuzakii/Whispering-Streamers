#define _GNU_SOURCE
#include <stdio.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h> 
#include "../cacheutils.h"
// #define SUM_KEY 22
const int length = 4*256*1024;//8MB shm

void cache_flush(){
	const int flush = 5*1024*1024; // Allocate 20M. Set much larger then L2
    char *c = (char *)malloc(flush);
    for (int i = 0; i < 0x7; ++i)
    {
    	for (int j = 0; j < flush; j++)
		{
			c[j] = j/2;
		}	
    }
}
int main(int argc, char const *argv[])
{
	// SET PROC TO CPU0
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	int result = sched_setaffinity(0, sizeof(mask), &mask);

	// INPUT ARGS & FLAGS
	int threshold = atoi(argv[1]);
	int trigger = atoi(argv[2]);
	// int cf_flag = atoi(argv[3]);

	//SHARED MEMORY INIT
	size_t* shm;
	shm = (size_t*) calloc(length, sizeof(size_t));
	printf("%ld\n",sizeof(size_t) );
	for (int iter = 0; iter < 30; iter++)
	{
		printf("(%d) ",iter );
		int hit_cnt[64]={0};
		
		for(int line = 0; line < 64; line++){
			
			for (int page = 0; page < length/512-1; page++)
			{
				int flag=0;
				int pid, status;

				for (int i = 0; i < length/8; i+=8){
					flush(&shm[i]);
				}

				int temper, l1, l2, l3;
				l1 = page*512 + 8*0;
				l2 = page*512 + 8*1;
				l3 = page*512 + 8*3;
				temper = shm[l1];
				temper = shm[l2];
				temper = shm[l3];

				// victim access
				int tr = page*512 + 8*(trigger+iter);
				int temp = shm[tr]; //TRIGGER PREFETCH

				uint64_t a, d, e, f;
				size_t temp12;
				int temp_cnt = 512*(page+1)+line*8;
				asm volatile("mfence");
				asm volatile("rdtsc": "=a"(a), "=d"(d));
				temp12 = shm[temp_cnt];
				asm volatile("mfence");
				asm volatile("rdtsc": "=a"(e), "=d"(f));

				if((e-a)<threshold)
					hit_cnt[line]++;
			}
		}
		for (int i = 0; i < 64; ++i)
		{
			printf("%d ",hit_cnt[i] );
		}
		printf("\n");
	}
	return 0;
}