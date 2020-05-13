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
#include <math.h>
#include "../cacheutils.h"
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

	size_t* shm;
	shm = (size_t*) calloc(length, sizeof(size_t));

	for (int iter = 1; iter < pow(2,12); iter=iter*2)
	{
		// iter = 2;
		printf("(%d) ",iter );
		float hit_cnt[64]={0}, hit_time[64]={0};
		
		for (int z = 0; z < 1000; ++z)
		{
			for(int line = 0; line < 64; line++){
				
				for (int page = 0; page < iter; page++)
				{
				for (int i = 0; i < length/8; i+=8){
					flush(&shm[i]);
				}
					int flag=0;
					int pid, status;


					int temper, l1, l2, l3;
					l1 = page*512 + 8*0;
					l2 = page*512 + 8*1;
					l3 = page*512 + 8*2;
					temper = shm[l1];
					temper = shm[l2];
					temper = shm[l3];

					// }
				}
				for (int page = 0; page < 1; page++)
				{
					// victim access
					int tr = page*512 + 8*3;
					int temp = shm[tr]; //TRIGGER PREFETCH

					uint64_t a, d, e, f;
					size_t temp12;
					int temp_cnt = 512*(page)+line*8;
					asm volatile("mfence");
					asm volatile("rdtsc": "=a"(a), "=d"(d));
					temp12 = shm[temp_cnt];
					asm volatile("mfence");
					asm volatile("rdtsc": "=a"(e), "=d"(f));

					if((e-a)<threshold){
						hit_cnt[line]++;
					}
					// hit_time[line] += (e-a);
						
				}
				// page = 0;
			}
		}	
		int total_lines=0;
		for (int i = 4; i < 64; ++i) // total number of lines prefetched in page no. 1
		{
			total_lines += hit_cnt[i];
		}
		printf("%d\n", total_lines);
	}
	free(shm);
	// shmctl(shm_id, IPC_RMID, NULL);
	// shmdt(shm);
	return 0;
}