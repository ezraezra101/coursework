#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//This take the inputted matrix and puts it in reduced row echelon form using Gaussian elimination




//static const int rows = 5;
//static const int columns = 4;
static bool verbose = false;

//Code from http://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-to-a-function-in-c
double** createArray(int n, int m){
	
	if(verbose){
		printf("Creating a %d by %d matrix", n, m);
	}
	
	double* values = calloc(m * n, sizeof(double));
	double** rows = malloc(n*sizeof(double*));
	for (int i=0; i<n; i++){
		rows[i] = values + i*m;
	}
	return rows;
}

//Code from http://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-to-a-function-in-c
void destroyArray(float** array){
	free(*array);
	free(array);
}

//Prints out a matrix to the terminal
void printMatrix(int rows, int columns, double** matrix){
	printf("\n");
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			printf("%f\t",matrix[i][j]);
		}
		
		printf("\n");
	}
}

double** scanMatrix( int *rows, int *columns){
	
	printf("How many rows does your matrix have?\n");
	scanf("%d", rows);
	printf("How many columns?\n");
	scanf("%d", columns);
	
	
	//double matrix[*rows][*columns];
	double** matrix = createArray(*rows, *columns);
	
	printf("Type in your Matrix:\n");
	//Matrix initialization:
	for (int i = 0; i < *rows; i++){
		
		for (int j = 0; j < *columns; j++){
			scanf("%lf", &matrix[i][j]);
		}
	} //End for
	
	if(verbose){
		printMatrix(*rows, *columns, matrix);
	}
	return matrix;
}


// helper for row echelon form
void steps1to3(int rows, int columns, double** matrix, int startRow){
	//Step 1: find leftmost nonzero column. This is the pivot column & the pivot position is at the top.

	
	int pivotCol = startRow;
	int pivotRow = startRow;
	while (matrix[pivotRow][pivotCol] == 0)	{
		pivotRow++;
		if (pivotRow >= rows){
			pivotRow = startRow;
			pivotCol++;
		}
		else if (pivotCol > columns){
			printf("Empty matrix....\n");
		}
	}//end while

	//Step 2: select a nonzero entry in the pivot column as the pivot.
	//If necessary, interchange rows to move this entry into the pivot position
		
	if (pivotRow != startRow){
		double tempVal;
		
		if(verbose){
			printf("Interchanging rows %d and %d\n", startRow, pivotRow);
		}
		
		for (int j = pivotCol; j < columns; j++){
			tempVal = matrix[pivotRow][j];
			
			matrix[pivotRow][j] = matrix[startRow][j];
			matrix[startRow][j] = tempVal;
			
		} //End for loop
		
		pivotRow = startRow;//not sure if this works
	} //End if


	//Adding a step: Make pivot position 1 via a scaling operation (moved from step 5)
	
	double scaleFactor = matrix[pivotRow][pivotCol];
	if(verbose){
		printf("Dividing row %d by %lf\n", pivotRow, scaleFactor);
	}
	for(int j = pivotCol; j < columns; j++){
		matrix[pivotRow][j] /= scaleFactor;
	}
	

	//Step 3: use row operations to create zeros in all the positions below the pivot.
	
	for (int i = pivotRow+1; i < rows; i++){
		double scaleFactor = matrix[i][pivotCol] / matrix[pivotRow][pivotCol];
		
		if (verbose){
			printf("Subtracting %lf times row %d from row %d\n", scaleFactor, pivotRow, i);
		}
		
		for (int j = pivotCol; j < columns; j++){
			matrix[i][j] = matrix[i][j] - matrix[pivotRow][j] * scaleFactor;
		}//End column for loop
		
	}//End row for loop

} //End steps1to3

//Puts a matrix in row echelon form with leading 1s
void echelonForm(int rows, int columns, double** matrix){
	//Step 4: Repeat steps 1 to 3, ignoring the pivot position's row and all rows above it
	
	if(verbose){
		printf("Taking matrix to echelon form\n");
	}

	for(int i=0; i<rows; i++){
		steps1to3( rows, columns, matrix, i);
	}
	
	if(verbose){
		printf("Row echelon form:");
		printMatrix(rows,columns, matrix);
	}
}

//Puts a matrix in reduced row echelon form
void reducedRowEchelonForm( int rows, int columns, double** matrix){
	echelonForm(rows, columns, matrix);
	
	if(verbose){
		printf("Taking matrix to reduced echelon form\n");
	}
	
	//Step 5: Beginning with the rightmost (& lowest) pivot and working upward & to the left,
	//		make each pivot 1 via a scaling operation &
	//		create zeros above each pivot.
	
	for (int i = rows-1; i >= 0; i--){

		//locates pivot column		
		int pivotCol=0;
		int pivotRow=i;
		for (int j = 0; j<columns && matrix[i][j] == 0; j++){
			pivotCol++;
		} //end column for loop 1
		
		
		/*
		//scales pivot row
		double scaleFactor = matrix[pivotRow][pivotCol];
		for (int j = pivotCol; j < columns; j++){
			matrix[pivotRow][j] /= scaleFactor;
		} //end for
		
		if(verbose){
			printf("Multiplying row %d by 1/%lf\n", pivotRow, scaleFactor);
		}
		*/
		
		//Creates zeros above each pivot
		for (int i2 = i-1; i2 >= 0; i2--){
				double scaleFactor = matrix[i2][pivotCol];
		
				if(verbose){
					printf("Subtracting %lf times row %d from row %d\n", scaleFactor, pivotRow, i2);
				}
		
				for (int j = pivotCol; j < columns; j++){
					
					matrix[i2][j] -= matrix[pivotRow][j] * scaleFactor;
				}//End column for loop
	
			}//End row for loop

			
		}//End pivot position loop
		
		
} //end reducedRowEchelonForm



//Computes the RREF of a matrix
int main(int argc, char *argv[])
{
	if (argc >0){
		verbose = true;
	}
	
	int rows;
	int columns;
	
	
	//double matrix[rows][columns];
	double **matrix = scanMatrix(&rows, &columns);
	/*
	matrix[0][0] = 2;
	matrix[0][1] = 3;
	matrix[1][0] = 4;
	matrix[1][1] = 5;
	*/
	/*
	matrix[0][0] =  1;
	matrix[0][1] =  2;
	matrix[0][2] =  3;
	matrix[0][3] =  9;
	matrix[1][0] =  2;
	matrix[1][1] = -1;
	matrix[1][2] =  1;
	matrix[1][3] =  8;
	matrix[2][0] =  3;
	matrix[2][1] =  0;
	matrix[2][2] = -1;
	matrix[2][3] =  3;
	*/
//	| 1  2  3 9|	| 1 0 0  2 |
//	| 2 -1  1 8|  ~	| 0 1 0 -1 |
//	| 3  0 -1 3|	| 0 0 1  3 |
	
	/*
	matrix[0][0] =  0;
	matrix[0][1] =  1;
	matrix[0][2] = -4;
	matrix[0][3] =  8;
	matrix[1][0] =  2;
	matrix[1][1] = -3;
	matrix[1][2] =  2;
	matrix[1][3] =  1;
	matrix[2][0] =  5;
	matrix[2][1] = -8;
	matrix[2][2] =  7;
	matrix[2][3] =  1;
	*/
	reducedRowEchelonForm(rows, columns, matrix);
		
	//Print final matrix:
	printMatrix(rows, columns, matrix);
	
	return 0;
}//End Main
