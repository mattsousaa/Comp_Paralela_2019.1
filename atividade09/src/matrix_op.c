#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <omp.h>
#include "aibb.h"

/* Função para ler matriz de um arquivo e armazenar em um array dinâmico
=> Parâmetros: Ponteiro de arquivo e ponteiro para linhas e colunas; 
=> Retorno: Referência na memória do array alocado. */
double *lerMatriz(char *str1, int *linhas, int *colunas){

int row, column;
int i, j, ch;
double x;

FILE *fp1;
FILE *fp2;

fp1 = fopen(str1,"r");
fp2 = fopen(".mat_dot.txt","w");

if(fp1 == NULL || fp2 == NULL)	printf("Files do not exist!!!\n");

/* Get rows and numbers of matrix */
fscanf(fp1,"%d",&row);
fscanf(fp1,"%d",&column);

*linhas = row;
*colunas = column;

double *data = (double *)malloc(row * column * sizeof(double));

if(column != 1){

/* Removing symbols of ":" and creating a new file of matrix without ':' */

while((ch = fgetc(fp1)) != EOF){

	if((ch >= '0' && ch <= '9' || ch == '.')) 	
		fputc(ch,fp2); 
	else	
		fputc(32,fp2);

	if(ch == '\n') fputc('\n',fp2);
		
}    

fclose(fp1);
fclose(fp2);

fp2 = fopen(".mat_dot.txt","r");

for (i = 0; i < row; i++){
	for(j = 0; j < column; j++){
		fscanf(fp2,"%lf",&x);
		*(data + i*column + j) = x;
	}
}

fclose(fp2);

return data;

} else{
	
//printf("Linhas: %d\n", row);
//printf("Colunas: %d\n", column);

for (i = 0; i < row; i++){
	for(j = 0; j < column; j++){
		fscanf(fp1,"%lf",&x);
		*(data + i*column + j) = x;
	}
}

return data;
	
}

}

/* Função para imprimir matriz. Importante para depuração.
=> Parâmetros: Ponteiro de referência da matriz juntamente com linhas e colunas; 
=> Retorno: (void function) */
void imprimirMatriz(double *data, int row, int column){

for (int i = 0; i < row; i++){
	for(int j = 0; j < column; j++){
		printf("%.2f ", *(data + i*column + j));
	}
}

printf("\n\n");

}

/* Função para escrever matriz em um arquivo
=> Parâmetros: Ponteiro de arquivo de saída e matriz, juntamente com ponteiro para linhas e colunas; 
=> Retorno: (void function) */
void escreverMatriz(char *out_mat, double *data, int row, int column){

int i, j;

FILE *print_data;
print_data = fopen(out_mat, "w");
fprintf(print_data, "%d\n", row);
fprintf(print_data, "%d\n", column);

for(i = 0; i < row; i++){
	for(j = 0; j < column-1; j++){
		fprintf(print_data, "%.2f:", *(data + i*column + j));
	}
	fprintf(print_data, "%.2f\n", *(data + i*column + j));
}

fclose(print_data);

}

/* Função para multiplicar matrizes e armazenar em um array dinâmico
=> Parâmetros: Resultado da multiplicação, matriz 1 com linhas e colunas, matriz 2 com linhas e colunas
=> Retorno: (void function) */
void multiplicarMatrizes(double *result, double *mat1, int line_m1, int col_m1, double *mat2, int line_m2, int col_m2){

int i, j, k;

#pragma omp parallel for shared(mat1, mat2, line_m1, col_m2, col_m1, result) private(i, j, k) schedule(static)
	for(i = 0; i < line_m1; i++){
		for(j = 0; j < col_m2; j++){
            *(result + i*col_m2 + j) = 0;
			for(k = 0; k < col_m1; k++){
				*(result + i*col_m2 + j) += *(mat1 + i*col_m1 + k) * *(mat2 + k*col_m2 + j);
			}
		}
	}	
}

/* Função para liberar memória após alocação dinâmica de vetores
=> Parâmetros: Ponteiro do array;
=> Retorno: (void function) */
void liberarMatriz(double *data){
	free(data);
}