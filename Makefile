
CC = gcc

sender: mcor_send.c mcor_send.h
	$(CC) -o mcor_sender mcor_send.c
