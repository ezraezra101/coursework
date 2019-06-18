#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int x;
	struct node *y;
} Node;

int main ()
{	
	Node x = {1, NULL};
	Node y = {2, NULL};
	x.y = &y;
	printf("%d\n", (x.y)->x);
	
	
	char* s1 = "string";
	char* s2 = "string";
	char s3[] = "string";
	
	printf("%s", s1);
	//*s1 = 'S'; //causes 'bus error 10'
	//but
	*(s3+1) = 'T'; //works just fine
	printf("%s", s3);
	printf("%s\n", s2);
	
	//This part doesn't cause allocation errors, but this may be compiler shenanigans
	/*
	int *array;
	array = malloc(sizeof(int) * 100000);
	int i = 0;
	while(array != NULL && i < 5000000)
	{
		i++;
		free(array);
		array = malloc(sizeof(int) * 100000);
		if (array == NULL){
			printf("NULL");
		}
		
	}
	printf("%d",i);
	*/
}