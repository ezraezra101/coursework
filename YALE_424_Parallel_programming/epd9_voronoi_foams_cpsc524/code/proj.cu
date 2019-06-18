#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <math.h>


#include "voronoi_query.cpp"

// Error checking macro from here: https://stackoverflow.com/questions/14038589/what-is-the-canonical-way-to-check-for-errors-using-the-cuda-runtime-api
#define CUDA_ERR_CHK(ans) { gpuAssert((ans), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t code, const char *file, int line) {
	if(code != cudaSuccess) {
		fprintf(stderr, "GPU error: %s %s %d\n", cudaGetErrorString(code), file, line);
		exit(code);
	}
}

__host__ __device__ FP rho(point q) { return (FP) 0.5; }

__global__ void gpu_foam(VOX *foam, dim3 shape, int z) {
	float step = 0.05;

	int x = threadIdx.x + blockDim.x * blockIdx.x;
	int y = threadIdx.y + blockDim.y * blockIdx.y;
	point q;
	q.x = step*x;
	q.y = step*y;
	q.z = step*z;

  if(x < shape.x && y < shape.y) {
  	VOX result = voronoi_query(q);

  	if(sizeof(VOX)*(x + y*shape.x) >= shape.x*shape.y * sizeof(VOX)) {
  		printf("How did you get here?\n"); // TODO remove
  	}

  	foam[x + y*shape.x] = result;
  }
}

int main(int argc, char *argv[]) {

	int i, j;
	dim3 shape;
	shape.x = 100;
	shape.y = 100;
	shape.z = 100;
	dim3 Grid(10, 10, 1); //Grid structure
	dim3 Block(10, 10, 1); //Block structure


	size_t size = shape.x * shape.y *sizeof(VOX);
	VOX  *dev_foam, *foam;
	foam = (VOX *) malloc(size);
	CUDA_ERR_CHK(cudaMalloc((void**)&dev_foam, size));

	for(int z = 0; z < shape.z; z++) {
		gpu_foam<<<Grid, Block>>>(dev_foam, shape, z);
		// CUDA_ERR_CHK(cudaMemset(dev_foam, 255, size)); // This works instead of gpu_foam<<...>>>

		CUDA_ERR_CHK(cudaGetLastError());
		CUDA_ERR_CHK(cudaMemcpy(foam, dev_foam, size, cudaMemcpyDeviceToHost));
		
		printf("Z layer %d:\n", z);
		for(i=0; i<shape.y; i++) {
			for(j=0; j<shape.x; j++) {
				printf("%c", foam[shape.x*i+j] == 0 ? ' ' : '#');
			}
			printf("\n");
		}
	}

	
	free(foam);
	CUDA_ERR_CHK(cudaFree(dev_foam));

	return 0;
}