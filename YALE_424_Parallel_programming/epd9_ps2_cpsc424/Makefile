
CC := icc
ifeq ($(CC), gcc-7) # is CC overridden by a command line argument?
CC := g++-7
GCC_OPTIONS := -Wall -pedantic
FAST_OPTIONS := -march=native -O3 -m64

OPTIONS := $(GCC_OPTIONS) $(FAST_OPTIONS) -fopenmp
else

CFLAGS := -fno-alias -std=c99 -O3 -g
INTELXHOST := -xHost
OPTIONS := $(CFLAGS) $(INTELXHOST) -fopenmp
endif

.cpp:
	$(CC) $(OPTIONS) $@.cpp -o $@

all: mandelbrot

run: mandelbrot
	./mandelbrot
