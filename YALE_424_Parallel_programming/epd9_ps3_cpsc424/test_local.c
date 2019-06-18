#include "matmul_parallel_3.c"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int world_size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double *A,*B,*C;
  int N = 4;
  if(rank == ROOT_P) {
  	A = calloc(N*(N+1)/2, sizeof(double));
  	B = calloc(N*(N+1)/2, sizeof(double));
  	C = calloc(N*N, sizeof(double));

  	int i;
	  for(i=0; i<N*(N+1)/2; i++) {
	  	A[i]=i+1;
	  	B[i]=i+1;
	  }
  } else {
  	A = NULL;
  	B = NULL;
  	C = NULL;
  }

  matmul(N,A,B,C);

  if(rank == ROOT_P) {
  	printf("Used %d processes to multiply matrix\n", world_size);
  	int i,j;
  	// Make sure that it calculates correctly:
  	double Ccorrect[4*4] = {1,2,4,7,  2,13,23,38,  4,23,77,122,  7,38,122,294};
  	int equal = 1;
  	for(i=0;i<N*N;i++) {
			equal &= Ccorrect[i] == C[i];
  	}
	  if(!equal) {
	  	printf("Did not pass equality check\n");
	  	for(i=0;i<N*N;i++) {
	  		printf("%f,", (float)C[i]);
	  		if(0==(i+1)%N) printf("\n");
	  	}
	  }
  }


  MPI_Finalize();
  return 0;
}