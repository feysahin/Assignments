#include<stdio.h>

void printMatrix(int n, int m[][n]);
void numOf1(int n, int m[][n]);

int main(){
	int i, j, n;
	
	printf("n: ");
	scanf("%d", &n);
	
	int n2=2*n+3, matrix[n2][n2];
	
	//ba�lang��ta 0'lardan olu�an bir matris olu�turma
	for(i=0; i<2*n+3;i++){
		for(j=0; j<2*n+3; j++) matrix[i][j] = 0;
	} 
	//sadece sol �st �eyre�i gezinip indis oynamalar� ile simetri alarak di�er �eyrekleri de de�i�tirme
	for(i=0; i<n+2; i++){
		for(j=0; j<n+2; j++){
			if(i+j>=n+2){
				matrix[i][j]=1;
				matrix[n2-1-i][j]=1;
				matrix[i][n2-1-j]=1;
				matrix[n2-1-i][n2-1-j]=1;
			}
		}
	}
	printMatrix(n2,matrix);
	numOf1(n2,matrix);
	return 0;
}
void numOf1(int n, int m[][n]){
	int i, j, count;
	for(i=1; i<n-1; i++){
		count=0;
		for(j=0; j<n; j++){
			if(m[i][j]==1) count++;
		}
		printf("\nTotal 1 in row %d is: %d", i+1, count);
	}
}
void printMatrix(int n, int m[][n]){
	int i, j;
	
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			printf(" %d ", m[i][j]);
		}
		printf("\n");
	}
}
