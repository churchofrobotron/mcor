#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "mcor_send.h"

int mcor_sock;
struct sockaddr_in server_addr;

void mcor_send_init(const char *ip_address, int port){
	if ((mcor_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
		perror("socket");
	}
	memset((char *) &server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	printf("Initializing sender for %s:%d...\n", ip_address, port);
	if (inet_aton(ip_address, &server_addr.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

}

void mcor_send_cleanup(){
	close(mcor_sock);
}

void mcor_send(const char *data, int length){

	int slen = sizeof(server_addr);

	printf("Sending data of length %d\n", length);
	int rc = sendto(mcor_sock, data, length, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (rc == -1){
		perror("sendto() failed!");
		exit(1);
	}
}

int main(int argc, char **argv){
	int port = strtol(argv[2], NULL, 10);
	printf("Creating sender for %s:%d\n", argv[1], port);
	mcor_send_init(argv[1], port);
	char *message = argv[3];
	mcor_send(message, strlen(message));
}
