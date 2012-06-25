
CC = gcc
CLFAGS =

all: sender 

clean:
	rm *.o sender

sender: mcor_sender.c mcor_send.o
	$(CC) -o mcor_sender mcor_sender.c mcor_send.o

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
