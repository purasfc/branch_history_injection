CC = gcc
CFLAGS = Wall

main : main.o common.o
	$(CC) main.o common.o -o main 

main.o : main.c common.h
	$(CC) -c main.c

common.o : common.c common.h
	$(CC) -c common.c

