
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcor_send.h"

int main(int argc, char **argv){
	int port = strtol(argv[2], NULL, 10);
	printf("Creating sender for %s:%d\n", argv[1], port);
	mcor_send_init(argv[1], port);
	char *message = argv[3];
	mcor_send(message, strlen(message));
}
