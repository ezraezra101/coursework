#include <stdio.h>


void aFunc(int x[][5]){
	int i;
	for(i = 0; i < 25; i++){
		printf("%d", x[0][i]);
	}
}

int main(){

	int x[][5] = {{5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}};
	
	aFunc(x);

	int anInt = ' ';
	printf(" %d", anInt);

	return 0;
}