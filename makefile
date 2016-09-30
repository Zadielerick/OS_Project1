CC=gcc
CFLAGS= -I

project1: Project1.c
	$(CC) -o Project1 Project1.c 

clean:
	rm Project1
