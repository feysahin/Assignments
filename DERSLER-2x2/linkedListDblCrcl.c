#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

typedef struct node{
	int data;
	struct node *left;
	struct node *right;
}NODE;

NODE *newNode();
void traverseRight(NODE **);
void traverseLeft(NODE **);

int main(){
	
	NODE *head = newNode();
	NODE *tmp = head, *tmp2;
	int i;
	
	printf("\ndata of new node: ");
	scanf("%d", &tmp->data);
	int n=3;
	for(i=1; i<n; i++){
		tmp2 = newNode();
		printf("\ndata of new node: ");
		scanf("%d", &tmp2->data);
		
		tmp->right = tmp2;
		tmp2->left = tmp;
		tmp = tmp2;
	}
	tmp->right = head;
	head->left = tmp;
		
	//traverseRight(&head);
	traverseLeft(&head);
	
	return 0;
}

void traverseRight(NODE **phead){
	NODE *tmp = *phead;
	
	while(tmp != NULL){
		printf("%d ", tmp->data);
		tmp = tmp->right;
		sleep(1);
	}
}

void traverseLeft(NODE **phead){
	NODE *tmp = *phead;
	
	while(tmp != NULL){
		tmp = tmp->left;
		printf("%d ", tmp->data);
		sleep(1);
	}
}

NODE *newNode(){
	return (NODE*)malloc(sizeof(NODE));
}
