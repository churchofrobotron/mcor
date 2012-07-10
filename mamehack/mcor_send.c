#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "mcor_send.h"
#include "machine/nvram.h"
#include "debug/debugcon.h"
#include "debug/debugcmd.h"

int mini_printf(running_machine &machine, char *buffer, const char *format, int params, UINT64 *param);
//int debug_command_parameter_number(running_machine &machine, const char *param, UINT64 *result);

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
	//printf("Sending data of length %d\n", length);
	int rc = sendto(mcor_sock, data, length, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (rc == -1){
		perror("sendto() failed!");
		exit(1);
	}
}

void execute_mcor_init(running_machine &machine, int ref, int params, const char **param)
{
   mcor_send_init(MCOR_SERVER, MCOR_PORT);
}

void execute_mcor_send(running_machine &machine, int ref, int params, const char **param)
{
	UINT64 values[MAX_COMMAND_PARAMS];
	char buffer[1024];
	int i;

	/* validate the other parameters */
	for (i = 1; i < params; i++)
		if (!debug_command_parameter_number(machine, param[i], &values[i]))
			return;

	/* then do a printf */
	if (mini_printf(machine, buffer, param[0], params - 1, &values[1]))
   {
      mcor_send(buffer, strlen(buffer));
   }
}

void execute_mcor_dumpscores(running_machine& machine, int ref, int params, const char** param)
{
   nvram_save(machine);
}
