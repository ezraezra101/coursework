# Ezra Davis
# HW 1 for Parallel Programming

# This makefile is loosely based on one I made a few years ago...

# The basic format for this ifeq comes from https://stackoverflow.com/questions/5618615/check-if-a-program-exists-from-a-makefile
CC := icc
ifeq ($(CC), gcc) # is CC overridden by a command line argument?
CC := gcc
CFLAGS :=
INTELXHOST :=
else

CFLAGS := -fno-alias -std=c99 -I/home/fas/cpsc424/ahs3/utils/timing /home/fas/cpsc424/ahs3/utils/timing/timing.o
INTELXHOST := -xHost
endif


executables := vector pi-O0 pi-O1 pi-O3 pi-O3-no-simd

all: $(executables)


pi-O0: pi.c
	$(CC) -g -O0 $(CFLAGS) -o $@ pi.c
pi-O1: pi.c
	$(CC) -g -O1 $(CFLAGS) -o $@ pi.c
pi-O3-no-simd: pi.c
	$(CC) -g -O3 -no-vec -no-simd $(CFLAGS) -o $@ pi.c
pi-O3: pi.c
	$(CC) -g -O3 $(INTELXHOST) $(CFLAGS) -o $@ pi.c

run-pi: pi-O0 pi-O1 pi-O3 pi-O3-no-simd
	./pi-O0
	./pi-O1
	./pi-O3-no-simd
	./pi-O3


vector: vector.c
	$(CC) -g -O3 $(INTELXHOST) $(CFLAGS) -o $@ vector.c

#Multipurpose simple make command
# %: %.c
# 	$(CC) $(CFLAGS) -o $@ $<

startup-session:
	srun --pty -p interactive -c 1 -t 6:00:00 --mem-per-cpu=6100mb bash

load-module:
	module load Langs/Intel/2015_update2

clean:
	-rm $(executables)
	rm -r *.dSYM

.PHONY: clean all run-pi startup-session load-module