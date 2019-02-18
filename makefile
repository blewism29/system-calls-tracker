all: main.c
	gcc main.c syscall.c -o main -lm
	./main
