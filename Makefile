CC = gcc
CFLAGS = -Wall -g -O0

main : main.o common.o
	$(CC) main.o common.o -o main 

main.o : main.c common.h
	$(CC) -c main.c

common.o : common.c common.h
	$(CC) -c common.c

fr_checker : fr_checker.o
	$(CC) $(CFLAGS) fr_checker.o -o fr_checker

fr_checker.o : fr_checker.c targets.h
	$(CC) $(CFLAGS) -c fr_checker.c 

