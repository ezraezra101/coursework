#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Matrix multiplication!




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
void destroyArray(double** array){
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

double** multiply(int firstRows, int columns, int secondColumns, double** matrix1, double** matrix2){
	printf("Multiplying matrices\n");
	
	double** product = createArray(firstRows, secondColumns);
	int i;//Counts first matrices rows
	int j;
	int k;
	for(i=0; i < firstRows; i++){ //for each row in the first matrix
		for(j=0; j < secondColumns; j++){ //for each column in the final matrix
			int sumOfProducts = 0;
			for(k=0; k < columns; k++){
				sumOfProducts += matrix1[i][k] * matrix2[k][j];
			}
			product[i][j] = sumOfProducts;
		}
	}//end outer for
	
	return product;
}

int main(){
	printf("We're multiplying matrices today!\n");
	printf("First matrix:\n");
	int rows1, columns1;
	double** matrix1 = scanMatrix(&rows1, &columns1);
	
	printf("\nSecond matrix:\n");
	int rows2, columns2;
	double** matrix2 = scanMatrix(&rows2, &columns2);
	
	if(columns1 != rows2){
		printf("matrix multiplication isn't defined when the number of columns offirst matrix don't equal the rows of the second");
		return 1;
	}
	
	double** product = multiply(rows1, columns1, columns2, matrix1, matrix2);
	
	printMatrix(rows1, columns2, product);
	return 0;
}