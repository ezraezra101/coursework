#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "timing.h"

#define DEBUG

#define MIN(a,b) (((a)<(b))?(a):(b))
#define A_ROW_TO_IDX(row) ((row)*((row)+1)/2)
#define B_COL_TO_IDX(col) ((col)*((col)+1)/2)
#define C_ROW_TO_IDX(row,N) ((row)*(N))


#define ROOT_P 0 // Root for gather/scatter operations. Has access to A and B.
#define B_TAG 2 // Message tag for filling the B buffer


// Multiply part of the matrix serially
void matmul_part(int row, int col, int N, int nrows, int ncols, double* Abuf, double* Bbuf, double* Cbuf) {
	int k, iA, iB, iC, i, j, Bbuf_offset, Abuf_offset, Cbuf_offset;
	Cbuf_offset = C_ROW_TO_IDX(row,N);
	Abuf_offset = A_ROW_TO_IDX(row);
	Bbuf_offset = B_COL_TO_IDX(col);


	for (i=row; i<row+nrows; i++) {
    iA = A_ROW_TO_IDX(i);
    for (j=col; j<col+ncols; j++,iC++) {
      iB = B_COL_TO_IDX(j);
      iC = C_ROW_TO_IDX(i, N) + j;
      Cbuf[iC-Cbuf_offset] = 0.;
      for (k=0; k<=MIN(i,j); k++)
      	Cbuf[iC-Cbuf_offset] += Abuf[iA+k-Abuf_offset] * Bbuf[iB+k-Bbuf_offset]; // avoids using known-0 entries 
    }
  }
}

// Multiply C = A*B from process with rank ROOT_P.
// Other processes must call this to help. (A,B, and C can be NULL though)
double matmul(int N, double *A, double *B, double *C) {

  double wctime0, wctime1, cputime;
  timing(&wctime0, &cputime);

	int world_size, rank, i, error;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
#ifdef DEBUG
  double calctime = 0.0, comtime = 0.0, tmptime = wctime0;

  timing(&wctime1, &cputime);
  comtime += wctime1 - tmptime;
  tmptime = wctime1;
#endif

	int *counts = (int *) calloc(world_size, sizeof(int));
	int *offsets = (int *) calloc(world_size, sizeof(int));
	int *rows = (int *) calloc(world_size, sizeof(int)); // Starting row of each process
	int *nrows = (int *) calloc(world_size, sizeof(int)); // Block size of each process

	int blocksize = N/world_size; // Number of rows handled by a process
	
	if(N/world_size != N/(float)world_size) {
		printf("Cannot currently multiply a %dx%d matrix with %d processes\n", N,N,world_size);
		exit(1);
	} else if(N < world_size) {
		printf("You are attempting to multiply %d%d matrices with %d processes. This seems kinda dumb.\n", N,N,world_size);
		printf("Try with fewer processes than the matrices have rows\n");
		exit(1);
	}

	if(rank == ROOT_P && (A == NULL || B == NULL || C == NULL)) {
		printf("You have passed an invalid array to the root node\n");
		exit(1);
	}

	for(i=0; i<world_size; i++) {
		nrows[i] = blocksize;
		rows[i] = i==0 ? 0 : rows[i-1]+nrows[i-1];
		offsets[i] = A_ROW_TO_IDX(rows[i]);
		counts[i] = A_ROW_TO_IDX(rows[i]+nrows[i]) - offsets[i];
	}

	double *Abuf, *Bbuf, *Cbuf;
	if(rank != ROOT_P) {
		Abuf = calloc(counts[rank], sizeof(double));
		// TODO: when changing to task 4 and 5, counts[worldsize-1] might no longer be the biggest.
		Bbuf = calloc(counts[world_size-1], sizeof(double));
		Cbuf = calloc(nrows[rank]*N, sizeof(double));
	} else {
		// Don't need to make buffers for ROOT_P - it already has all of A and B and C
		Abuf = &A[A_ROW_TO_IDX(rows[ROOT_P])];
		Bbuf = &B[B_COL_TO_IDX(rows[ROOT_P])];
		Cbuf = &C[C_ROW_TO_IDX(rows[ROOT_P], N)];
	}

	///////////////
	// Scatter A //

#ifdef DEBUG
  timing(&wctime1, &cputime);
  calctime += wctime1 - tmptime;
  tmptime = wctime1;
#endif

	error = MPI_Scatterv(
		A, counts, offsets, MPI_DOUBLE,
		rank==ROOT_P ? MPI_IN_PLACE : Abuf, counts[rank], MPI_DOUBLE,
		ROOT_P, MPI_COMM_WORLD
	);

	if(error != MPI_SUCCESS) {
		printf("Error when trying to send out A\n");
		exit(1);
	}

	///////////////
	// Scatter B //
	error = MPI_Scatterv(
		B, counts, offsets, MPI_DOUBLE,
		rank==ROOT_P ? MPI_IN_PLACE : Bbuf, counts[world_size-1], MPI_DOUBLE,
		ROOT_P, MPI_COMM_WORLD
	);

	if(error != MPI_SUCCESS) {
		printf("Error when trying to send out B\n");
		exit(1);
	}

#ifdef DEBUG
  timing(&wctime1, &cputime);
  comtime += wctime1 - tmptime;
  tmptime = wctime1;
#endif

	/////////////////////////////
	// Perform multiplcation   //

	for(i=0; i<world_size; i++) {
		int cur_b_idx = (world_size+rank-i) % world_size;

		matmul_part(rows[rank], rows[cur_b_idx], N, nrows[rank], nrows[cur_b_idx],
			Abuf, Bbuf, Cbuf);


#ifdef DEBUG
  timing(&wctime1, &cputime);
  calctime += wctime1 - tmptime;
  tmptime = wctime1;
#endif

		// Note: Root process has access to the whole A and B. Why not use it?
		// It even helps stop deadlock
		if(ROOT_P != (rank+1)%world_size) {
			MPI_Send(Bbuf, counts[cur_b_idx], MPI_DOUBLE, (rank+1)%world_size, B_TAG, MPI_COMM_WORLD);
		}
		if(rank != ROOT_P) {
			MPI_Status status;
			MPI_Recv(Bbuf, counts[(world_size+cur_b_idx-1)%world_size], MPI_DOUBLE,
				rank-1, B_TAG, MPI_COMM_WORLD, &status);
		} else {
			Bbuf = &B[offsets[(world_size+cur_b_idx-1)%world_size]];
		}


#ifdef DEBUG
  timing(&wctime1, &cputime);
  comtime += wctime1 - tmptime;
  tmptime = wctime1;
#endif

	}

	//////////////
	// Gather C //

	for(i=0;i<world_size;i++) {
		counts[i] = N*nrows[i];
		offsets[i] = i==0 ? 0 : counts[i-1]+offsets[i-1];
	}

	error = MPI_Gatherv(
		rank==ROOT_P ? MPI_IN_PLACE : Cbuf, counts[rank], MPI_DOUBLE,
		C, counts, offsets, MPI_DOUBLE,
		ROOT_P, MPI_COMM_WORLD
	);

	if(error != MPI_SUCCESS) {
		printf("Error when trying to gather C to root.\n");
		exit(1);
	}

#ifdef DEBUG
  timing(&wctime1, &cputime);
  comtime += wctime1 - tmptime;
  tmptime = wctime1;
#endif

	free(counts);
	free(offsets);
	free(rows);
	free(nrows);
	if(rank != ROOT_P) {
		free(Abuf);
		free(Bbuf);
		free(Cbuf);
	}

  timing(&wctime1, &cputime);

#ifdef DEBUG
  calctime += wctime1 - tmptime;

  printf("Process %d spent %lf time calculating and %lf time communicating\n", rank, calctime, comtime);
#endif

  return(wctime1 - wctime0);
}
