/* Send Multicast Datagram code example. */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN  1024

int main(int argc, char *argv[])
{
	int sock;
	char message_to_send[MAX_LEN];
	int send_len;
	char* multicast_ip;
	unsigned short multicast_port;
	unsigned char multicast_ttl=1;
	struct sockaddr_in multicast_addr;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s Multicast_IP Multicast_Port\n", argv[0]);
		exit(1);
	}

	multicast_ip = argv[1];       /* arg 1: multicast IP address */
	multicast_port     = atoi(argv[2]); /* arg 2: multicast port number */

	/* create a socket */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("Socket creation failed");
		exit(1);
	}

	/* set the TTL (time to live/hop count) for the send */
	if ((setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &multicast_ttl, sizeof(multicast_ttl))) < 0)
	{
		perror("setsockopt() failed");
		exit(1);
	}

	memset(&multicast_addr, 0, sizeof(multicast_addr));
	multicast_addr.sin_family      = AF_INET;
	multicast_addr.sin_addr.s_addr = inet_addr(multicast_ip);
	multicast_addr.sin_port        = htons(multicast_port);

	printf("Type the message below (Press Enter to send, ctrl-C to quit):\n");

	memset(message_to_send, 0, sizeof(message_to_send));

	while (fgets(message_to_send, MAX_LEN, stdin))
	{
		send_len = strlen(message_to_send);

		if ((sendto(sock, message_to_send, send_len, 0,
		(struct sockaddr *) &multicast_addr,
		sizeof(multicast_addr))) != send_len)
		{
			perror("Error in number of bytes");
			exit(1);
		}

		memset(message_to_send, 0, sizeof(message_to_send));
	}

	close(sock);

	exit(0);
}
