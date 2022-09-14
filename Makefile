CC=gcc
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang
#CFLAGS=-O3

default: all

main:
	$(CILKCC) -o main main.c -fcilkplus

.PHONY: clean

all:  main

clean:
	rm -f  main
