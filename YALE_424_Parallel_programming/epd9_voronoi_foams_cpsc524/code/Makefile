# This Makefile assumes the following module files are loaded:
#
# Langs/Intel/15
# GPU/Cuda/8.0
#
# This Makefile will only work if executed on a GPU node.
#

CUDAPATH = /gpfs/apps/hpc/GPU/Cuda/8.0
# TODO comment out
#CUDAPATH = /Developer/NVIDIA/CUDA-9.1

NVCC = $(CUDAPATH)/bin/nvcc

NVCCFLAGS = -I$(CUDAPATH)/include -O3

DEBUG = -g -G
LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lm # TODO uncomment out

# Compiler-specific flags (by default, we always use sm_35)
GENCODE_SM35 = -gencode=arch=compute_35,code=\"sm_35,compute_35\"
GENCODE = $(GENCODE_SM35)

.SUFFIXES : .cu .ptx
.PHONY : clean setup

BINARIES = proj

proj: proj.o voronoi_query.cpp
	$(NVCC) $(GENCODE) $(LFLAGS) $(DEBUG) -o $@ $<

test_voronoi_query: test_voronoi_query.cpp voronoi_query.cpp
	$(NVCC) $(GENCODE) $(LFLAGS) -o $@ voronoi_query.cpp test_voronoi_query.cpp

.cu.o:
	$(NVCC) $(GENCODE) $(NVCCFLAGS) $(DEBUG) -o $@ -c $<

.cpp.o:
	$(NVCC) $(GENCODE) $(NVCCFLAGS) $(DEBUG) -o $@ -c $<

clean:	
	rm -f *.o $(BINARIES)

setup:
	module load Langs/Intel/15 GPU/Cuda/8.0
	srun --pty -c 1 --mem-per-cpu=6100 -p cpsc424_gpu -t 2:00:00 --gres-flags=enforce-binding --gres=gpu:1 bash
