#include "juliaMcast.h"
#include <inttypes.h>
#include "msock.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define SLEEP_1_SECOND 1000000
#define LOOP_COUNT 100

static int pktCount = 0;

void DieWithError(char *msg)
{
	printf("%s\n", msg);
	exit(-1);
}

/* this function is run by the second thread */
void *beacon(void *x_void_ptr)
{
	       
	advertisePresence("223.0.0.1", "2222", 100, 1000000);	
	return x_void_ptr;
}

int advertise() 
{
	printf("Frankie's monster is alive....\n");
	int x = 0, y = 0;

	/* show the initial values of x and y */
	printf("x: %d, y: %d\n", x, y);

	/* this variable is our reference to the second thread */
	pthread_t advertise_thread;

	/* create a second thread which executes advertise(&x) */
	if(pthread_create(&advertise_thread, NULL, beacon, &x)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
	/* increment y to 100 in the first thread */
	while(++y < LOOP_COUNT) {
		usleep(SLEEP_1_SECOND);
		printf("Aloha: y =%d\n", y);
		fflush(stdout);
	}

	printf("y increment finished\n");

	/* wait for the second thread to finish */
	if(pthread_join(advertise_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

	/* show the results - x is now 100 thanks to the second thread */
	printf("x: %d, y: %d\n", x, y);
	
	
		
	return 1;
}

int testlib() 
{
	printf("It's alive 2....\n");\
	return 1;
}

int discover()
{
	printf("In neighbor discovery C-function\n");
	fflush(stdout);
	discoverNeighbors("223.0.0.1", "2222");
	return 0;
}

int discoverNeighbors(char *multicastIP, char *mulitcastPort)
{
	SOCKET		sock;
	char*		recvBuf;
	int			recvBufLen = 2240;
	int 		bytes=0;
	time_t 	timer;
	
	recvBuf = 	malloc(recvBufLen*sizeof(char));

	// Creat the receive socket
	sock = mcast_recv_socket(multicastIP, mulitcastPort, recvBufLen);
	
	if(sock < 0)
		DieWithError("mcast_recv_socket() failed");
	else printf("receive mulicast socket created\n");

	
    /* Receive a single datagram from the server */
    if ((bytes = recvfrom(sock, recvBuf, recvBufLen, 0, NULL, 0)) < 0)
		DieWithError("recvfrom() failed");
                
	int this_p = ntohl(*(int*)recvBuf);                
	time(&timer);  /* get time stamp to print with recieved data */	
	printf("Time Received: %.*s : packet %d, %d bytes\n",
	 (int)strlen(ctime(&timer)) - 1, ctime(&timer), this_p, bytes);

	exit(EXIT_SUCCESS);
}

int advertisePresence(char *multicastIP, char *multicastPort, int loopCount, int usleepTime) 
{
	SOCKET 	sock;
	struct addrinfo *multicastAddr;
	int    	sendStringLen;          /* Length of string to multicast */
	int     multicastTTL=1;         /* Arg: TTL of multicast packets */
	int 	i = 0;
				
	char *advertisementStr="Aloha Beacon\n";
	fprintf(stdout, advertisementStr);
	fflush(stdout);
	
	sendStringLen = strlen(advertisementStr);	
	sock = mcast_send_socket(multicastIP, multicastPort, multicastTTL, &multicastAddr);
		
	if(sock == -1 ) {
		printf("\nmcast_send_socket() failed\n");	
		DieWithError("Didn't even get to advertise function!\n");
	}
	
	for (i = 0; i < loopCount; i++) {
		if ( sendto(sock, advertisementStr, sendStringLen, 0,
			multicastAddr->ai_addr, multicastAddr->ai_addrlen) != sendStringLen )
				DieWithError("sendto() sent a different number of bytes than expected");        
		printf("sent beacon packet %d\n", pktCount++);
		usleep(usleepTime);
	}
	return 0;
}

