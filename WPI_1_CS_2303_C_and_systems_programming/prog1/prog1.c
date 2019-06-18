#include <math.h>
#include <stdio.h>

//This program takes pairs of points, finds the distance between them and averages those distances.
//Next it computes the log(average)/log(2) and graphs the value

//Returns log based 2 of x
double log2(double x){
	return log(x)/log(2);
}//End log2

//Returns the Euclidean distance between two 2 dimensional points
double eucDist(double x1, double y1, double x2, double y2){
	return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}//End eucDist


//Averages the distance between pairs of points and computes and graphs the log of the average base 2.
int main(void){	
	printf("How many pairs of points are you entering?\n");
	int numPts; //Number of pairs input
	scanf("%d", &numPts);
	
	double sum = 0; // sum of the distances between the points
	int i; //Used for for loops
	for(i=0; i<numPts; i++){
		int x1, y1, x2, y2;
		printf("Please input a point pair:\n");
		scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
		
		double dist = eucDist( x1, y1, x2, y2);
		printf("Distance between the points: %f\n", dist);
		sum += dist;
	}//End for loop
	
	double avg = sum/ (double) numPts; //Average of the distances between the points
	printf("\nAverage distance: %lf\n", avg);
	
	double log2avg = log2(avg);
	printf("Log2 of the average: %lf\n", log2avg);
	
	//Make bar graph [of ceil (log2 (average Euclidean distance))] here
	printf("|\n|");

	for( i=0; i < ceil(log2avg); i++){
		printf("#");
	}//end for loop
	
	//prints axis
	printf("\n-");
	for(i=0;i<2*ceil(log2avg); i++){
		printf("-");
	}
	
	//labels axis
	if(ceil(log2avg) == 1){
		printf("\n 1 2");
	}
	else {
		printf("\n0");
		for(i=1; i<2*ceil(log2avg); i++){
			//If i is at one of the quarters of log2avg*2
			if((i % (2*(int)ceil(log2avg)%4 -2)) == 0) {
				printf("%3d", i+2);
				i=i+2;
			}
			else
				printf(" ");
		}
	}
	printf("\nThe log based 2 of the average of the Euclidian distances between the point pairs.\n\n");
	
} //end main