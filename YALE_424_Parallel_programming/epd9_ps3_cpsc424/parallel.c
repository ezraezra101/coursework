#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PARALLEL

#ifdef PARALLEL
#include <mpi.h>
#endif

double matmul(int, double*, double*, double*);


//const int trials = 4; // Sadly, this is nonstandard... :-(
#define TRIALS 4

// This array contains the sizes of the test cases
int sizes[TRIALS+1]={1000,2000,4000,8000,7633};

// This array contains the file names for the true answers
char files[TRIALS+1][50]={"/home/fas/cpsc424/ahs3/assignment3/C-1000.dat",\
                   "/home/fas/cpsc424/ahs3/assignment3/C-2000.dat",\
                   "/home/fas/cpsc424/ahs3/assignment3/C-4000.dat",\
                   "/home/fas/cpsc424/ahs3/assignment3/C-8000.dat",\
                   "/home/fas/cpsc424/ahs3/assignment3/C-7633.dat"};


int main(int argc, char **argv) {

  /*
    This is the parallel main program for CPSC424/524 Assignment #3.

    Author: Andrew Sherman, Yale University
    Edited by Ezra Davis

    Date: 4/11/2018

  */

  int world_size=1, rank=0;

#ifdef PARALLEL
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

  int starting_trial = 0, ending_trial = TRIALS;
  if(argc > 1) {
    if(argv[1][0] == 'b') {
      // Skip the earlier trials
      starting_trial = 3;
    } else if(argv[1][0] == '5') {
      // Perform task 5:
      starting_trial = TRIALS;
      ending_trial = TRIALS+1;
    }
  }

  if(0 != rank) {
    int i;
    for(i=starting_trial; i<ending_trial;i++) {
#ifdef PARALLEL
      MPI_Barrier(MPI_COMM_WORLD);
#endif
      matmul(sizes[i], NULL, NULL, NULL);
    }
  } else {
    int N, i, run;
    double *A, *B, *C, *Ctrue;
    long sizeAB, sizeC;

    double wctime, Fnorm;

    FILE *fptr;

    // Print a table heading
    printf("Matrix multiplication times (with %d processes):\n   N      TIME (secs)    F-norm of Error\n -----   -------------  -----------------\n", world_size);

    // Now run the four test cases
    for (run=starting_trial; run<ending_trial; run++) {
      N = sizes[run];

      sizeAB = N*(N+1)/2; //Only enough space for the nonzero portions of the matrices
      sizeC = N*N; // All of C will be nonzero, in general!

      A = (double *) calloc(sizeAB, sizeof(double));
      B = (double *) calloc(sizeAB, sizeof(double));
      C = (double *) calloc(sizeC, sizeof(double));

      srand(12345); // Use a standard seed value for reproducibility

      // This assumes A is stored by rows, and B is stored by columns. Other storage schemes are permitted
      for (i=0; i<sizeAB; i++) A[i] = ((double) rand()/(double)RAND_MAX);
      for (i=0; i<sizeAB; i++) B[i] = ((double) rand()/(double)RAND_MAX);

      // Time the serial/parallel matrix multiplication computation
#ifdef PARALLEL
      MPI_Barrier(MPI_COMM_WORLD);
#endif

      wctime = matmul(N, A, B, C);


      free(A);
      free(B);

      // Remainder of the code checks the result against the correct answer (read into Ctrue)
      Ctrue = (double *) calloc(sizeC, sizeof(double));

      fptr = fopen(files[run],"rb");

      if(fptr != NULL) {
        fread(Ctrue, sizeof(double), sizeC, fptr);
        fclose(fptr);    

        //Compute the Frobenius norm of Ctrue-C
        Fnorm = 0.;
        for (i=0; i<sizeC; i++) Fnorm += (Ctrue[i]-C[i])*(Ctrue[i]-C[i]);
        Fnorm = sqrt(Fnorm);

        // Print a table row
        printf ("  %5d    %9.4f  %15.10f\n", N, wctime, Fnorm);
      } else {
        printf("  %5d    %9.4f  Could not open comparison file\n", N, wctime);
      }

      free(Ctrue);
      free(C);
    }

  } // End master only branch.

#ifdef PARALLEL
  MPI_Finalize();
#endif

}
