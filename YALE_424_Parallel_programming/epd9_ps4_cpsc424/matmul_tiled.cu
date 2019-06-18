#define FP float

#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <math.h>

#define BLOCK_SIZE blockDim.x // Note: Blocks should be square.

__global__ void gpu_matrixmult(FP *a,FP *b, FP *c, int n) {
  int m, e;

  int col = threadIdx.x + blockDim.x * blockIdx.x;
  int row = threadIdx.y + blockDim.y * blockIdx.y;

  extern __shared__ FP s[];
  FP *A_tile = s;
  FP *B_tile = &A_tile[blockDim.x * blockDim.y];

  FP c_value = 0;

  if(col < n && row < n) { // Make sure that we have a value of `c` to set.

    // TODO make sure this works
    // m is the column of A and row of B.
    for(m = 0; m < n; m+= blockDim.x) {

      A_tile[BLOCK_SIZE*threadIdx.x + threadIdx.y] = a[row * n + (m+theadIdx.y)];
      B_tile[BLOCK_SIZE*threadIdx.x + threadIdx.y] = b[(m+threadIdx.y) * n + col];
      __syncthreads();

      for(e=0; e<blockDim.x; ++e) {
        c_value += A_tile[BLOCK_SIZE*threadIdx.x + e]
                 * B_tile[BLOCK_SIZE*e + threadIdx.y];
      }

      __syncthreads();
    }

    c[row*n + col] = c_value;

  }

}


void cpu_matrixmult(FP *a,FP *b, FP *c, int n) {

  int index, indexa, indexb;
  FP cvalue;
  for(int col=0;col < n; col++)
    for(int row=0;row < n; row++) {
      indexb = col;
      index = row * n + col;
      cvalue = 0.;
      for (indexa = row*n; indexa < (row*n + n); indexa++, indexb+=n) 
	cvalue += a[indexa]*b[indexb];
      c[index] -= cvalue; //NOTE: This calculates the diff between CPU and GPU computations.
    }
}


int main(int argc, char *argv[]) {

  int i, j; // loop counters

  int gpucount = 0; // Count of available GPUs
  int gpunum = 0; // Device number to use
  int Grid_Dim = 1; //Grid dimension, x and y, square
  int Block_Dim = 1; //Block dimension, x and y, square

  int n; // matrix dimension
  FP *a,*b,*c;
  FP *dev_a, *dev_b, *dev_c;
  int size; // number of bytes in arrays
  int tile_size; // Number of bytes in a tile

  cudaEvent_t start, stop; // using cuda events to measure time
  float elapsed_time_ms; // which is applicable for asynchronous code also
  cudaError_t errorcode;

  // --------------------SET PARAMETERS AND DATA -----------------------

  errorcode = cudaGetDeviceCount(&gpucount);
  if (errorcode == cudaErrorNoDevice) {
    printf("No GPUs are visible\n");
    exit(-1);
  }
  else {
     printf("Device count = %d\n",gpucount);
  }

  if (argc!=4) {
    printf("Usage: matmul <matrix dim> <block dim> <grid dim>\n");
    exit (-1);
  }

  n = atoi(argv[1]);

  Block_Dim = atoi(argv[2]); // Square block
  if (Block_Dim*Block_Dim > 1024) {
    printf("Error, too many threads in block\n");
    exit (-1);
  }

  Grid_Dim = atoi(argv[3]); // Square grid
  if (Grid_Dim*Block_Dim < n) {
    printf("Error, number of threads in x/y dimensions less than number of array elements\n");
    exit (-1);
  }

  cudaSetDevice(gpunum);
  printf("Using device %d\n",gpunum);
  
  printf("Matrix Dimension = %d\n",n);
  printf("Block_Dim = %d, Grid_Dim = %d\n",Block_Dim,Grid_Dim);

  dim3 Grid(Grid_Dim, Grid_Dim); //Grid structure
  dim3 Block(Block_Dim, Block_Dim); //Block structure

  size = n * n * sizeof(FP); // number of bytes in total in arrays
  tile_size = Block_Dim * Block_Dim * sizeof(FP);

  a = (FP*) malloc(size); // dynamically allocated memory for arrays on host
  b = (FP*) malloc(size);
  c = (FP*) malloc(size); // results from GPU

  srand(12345);
  int p = n; //Used here only to illustrate proper initialization for non-square case
  for(i=0;i < n;i++)
    for(j=0;j < p;j++) {
      a[i * p + j] = (FP) rand() / (FP) RAND_MAX;
      //      a[i * p + j] = (FP) i+j; // may be helpful for debugging
    }

  for(i=0;i < p;i++)
    for(j=0;j < n;j++) {
      b[i * n + j] = (FP) rand() / (FP) RAND_MAX;
      //      b[i * n + j] = (FP) i+j; // may be helpful for debugging
    }

  // ------------- COMPUTATION DONE ON GPU ----------------------------

  cudaMalloc((void**)&dev_a, size); // allocate memory on device
  cudaMalloc((void**)&dev_b, size);
  cudaMalloc((void**)&dev_c, size);

  cudaMemcpy(dev_a, a , size ,cudaMemcpyHostToDevice);
  cudaMemcpy(dev_b, b , size ,cudaMemcpyHostToDevice);

  cudaEventCreate(&start); // instrument code to measure start time
  cudaEventCreate(&stop);
  
  cudaEventRecord(start, 0);
  // cudaEventSynchronize(start); // not needed

  gpu_matrixmult<<<Grid,Block, 2*tile_size>>>(dev_a,dev_b,dev_c,n);

  cudaEventRecord(stop, 0); // instrument code to measure end time
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&elapsed_time_ms, start, stop );

  cudaMemcpy(c,dev_c, size ,cudaMemcpyDeviceToHost);

  printf("Time to calculate results on GPU: %f ms.\n", elapsed_time_ms); // exec. time

  // ------------- COMPUTATION DONE ON HOST CPU ----------------------------
  // DEBUGGING USE ONLY (AND FOR LIMITED NUMBERS OF TIMING RUNS)

  cudaEventRecord(start, 0); // use same timing
  // cudaEventSynchronize(start); // not needed


  cpu_matrixmult(a,b,c, n); // do calculation on host (NOTE: This computes the diff with GPU result.)

  cudaEventRecord(stop, 0); // instrument code to measue end time
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&elapsed_time_ms, start, stop );

  printf("Time to calculate results on CPU: %f ms.\n", elapsed_time_ms); // exec. time

// ------------------- check device creates correct results -----------------

  double error, suma, sumb, sumc, ai, bi, ci;
  suma = 0.; sumb = 0; sumc = 0;
  for(i=0;i < n*n;i++) {
    ai = (double) a[i];
    bi = (double) b[i];
    ci = (double) c[i];
    suma += ai*ai;
    sumb += bi*bi;
    sumc += ci*ci;
  }
  suma = sqrt(suma);
  sumb = sqrt(sumb);
  sumc = sqrt(sumc);
  error =  sumc/(n*suma*sumb);
  printf("Scaled error between GPU and CPU: %e\n", error);

// -------------- clean up ---------------------------------------

  free(a);
  free(b);
  free(c);
  cudaFree(dev_a);
  cudaFree(dev_b);
  cudaFree(dev_c);

  cudaEventDestroy(start);
  cudaEventDestroy(stop);

  return 0;
}
