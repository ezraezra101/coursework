#include <stdio.h>
struct node {
	int val;
};

struct node aNode;

struct node node2 = {4};

struct node *ptr = &aNode;

int main(){
	aNode.val = 13;
	//ptr = &aNode;
	// *ptr = node2;
	
	printf("%d\n", (*ptr).val);
	printf("%d\n", aNode.val);
}
