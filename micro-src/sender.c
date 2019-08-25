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
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include "../cacheutils.h"
#define SUM_KEY 22
const int length = 4*256*1024;//8MB shm
#define PORT 8080 


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
	CPU_SET(2, &mask);
	int result = sched_setaffinity(0, sizeof(mask), &mask);

	int val = atoi(argv[1]);
	//SHARED MEMORY INIT
	int shm_id = shmget(SUM_KEY, length*sizeof(size_t), 0777 | IPC_CREAT);
	size_t* shm = shmat(shm_id, 0, 0);
	*(long long int*)shm = 0;
	for (int i = 0; i < length; i++){
		shm[i] = (long long int)i;
	}

	// for(int line = 0; line < 64; line++){
		
	// 	for (int page = 0; page < length/512; page++)
	// 	{
	 int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

	int flag=0;
	int pid, status;
	int page = 0;
	int code[1000] = {0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,0,0,0,1,1,0,0,0,0,1,1,1,0,1,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,1,1,0,1,1,0,1,
		1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,1,0,0,0,1,1,1,0,1,1,
		0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,1,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,0,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,1,0,0,1,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,0,0,1,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,0,0,1,1,1,1,0,1,1,0,1,0,1,0,0,1,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,1,1,1,0,1,1,0,0,0,0,1,1,0,0,1,0,0,0,0,1,1,0,0,1,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,0,0,1,1,0,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1,1,1,0,0,1,1,0,0,1,0,1,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,1,0,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1,1,0,1,0,0,1,1,1,1,1,0,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,0,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,0,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,1,0,0,1,0,1,1,0,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,0,1,1,0,0,1,0,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,1,1,0,0,1,0,1,1,0,1,1,1,0,0,1,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,0,1,0,0,1,1,1,0,0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,1,1,1,1,0,1,0,1,1,1,0,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,1,0,1,0,0,0,1,1,0,1,1,0,1,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,1,1,1,0,1,0,0,1,1,1,0,1,1,1,1,0,0,0,1,1,0,1,0};

	clock_t begin = clock();
	for (int z = 0; z < 1000; ++z)
    {	
		// for (int i = 0; i < 18*8; i += 8){
		// 	flush(&shm[i]);
		// }
		if(code[z] == 1) //EXEC ATTACK
		{
				int temper;
				temper = z*512 + shm[8*0];
				temper = z*512 + shm[8*1];
				temper = z*512 + shm[8*2];
		}
		else{
				int temper;				
				temper = z*512 + shm[8*2];
				temper = z*512 + shm[8*1];
				temper = z*512 + shm[8*0];

		}
		send(sock , hello , strlen(hello) , 0 ); 
		valread = read( sock , buffer, 1024); 
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time: %f  |  ",time_spent );		

	shmctl(shm_id, IPC_RMID, NULL);
	shmdt(shm);
	return 0;
}
/*
▶ taskset 0x1 ./new_rdtsc_no_avg 110 1 0
*/