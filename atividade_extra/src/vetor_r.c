#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 

int main(int argc, char *argv[]){

char *file1 = argv[1];
int row, column;
float x;
int i,j;

FILE *fp1;
fp1 = fopen(file1,"r");

fscanf(fp1,"%d",&row);
fscanf(fp1,"%d",&column);

float *data = (float *)malloc(row * column * sizeof(float));

printf("Linhas: %d\n", row);
printf("Colunas: %d\n", column);

for (i = 0; i < row; i++){
	for(j = 0; j < column; j++){
		fscanf(fp1,"%f",&x);
		*(data + i*column + j) = x;
	}
}

for (i = 0; i < row; i++){
	for(j = 0; j < column; j++){
		printf("%.2f ", *(data + i*column + j));
	}
}

}