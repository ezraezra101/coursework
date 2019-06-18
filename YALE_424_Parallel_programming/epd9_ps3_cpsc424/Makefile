# NOTE: This Makefile presumes that the following command has been executed:

#                module load Langs/Intel/15 MPI/OpenMPI/2.1.1-intel15

TIMINGDIR = /home/fas/cpsc424/ahs3/utils/timing
# MPICC is a wrapper for other compilers that just adds the MPI library.
CC = mpicc
CFLAGS = -g -O3 -xHost -fno-alias -std=c99 -I$(TIMINGDIR)

all: serial parallel2 parallel3 parallel4

serial:	serial.o matmul.o $(TIMINGDIR)/timing.o
	$(CC) -o $@ $(CFLAGS) $^

parallel2:	parallel.o matmul_parallel_2.o $(TIMINGDIR)/timing.o
	$(CC) -o $@ $(CFLAGS) $^

parallel3:	parallel.o matmul_parallel_3.o $(TIMINGDIR)/timing.o
	$(CC) -o $@ $(CFLAGS) $^

parallel4:	parallel.o matmul_parallel_4.o $(TIMINGDIR)/timing.o
	$(CC) -o $@ $(CFLAGS) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean all

clean:
	rm -f serial parallel2 parallel3 parallel4 *.o
