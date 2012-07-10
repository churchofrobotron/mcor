
#ifndef MCOR_SENDER_H
#define MCOR_SENDER_H

#include "emu.h"

#define MCOR_SERVER "127.0.0.1"
#define MCOR_PORT 2084

void mcor_send(const char *data, int length);
void mcor_send_cleanup();
void mcor_send_init(const char *ip_address, int port);

void execute_mcor_init(running_machine &machine, int ref, int params, const char **param);
void execute_mcor_send(running_machine &machine, int ref, int params, const char **param);
void execute_mcor_dumpscores(running_machine &machine, int ref, int params, const char **param);

#endif
