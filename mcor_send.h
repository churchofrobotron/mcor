
#ifndef MCOR_SENDER_H
#define MCOR_SENDER_H

#define MCOR_SERVER "127.0.0.1"
#define MCOR_PORT 2084

void mcor_send(const char *data, int length);
void mcor_send_cleanup();
void mcor_send_init(const char *ip_address, int port);

#endif
