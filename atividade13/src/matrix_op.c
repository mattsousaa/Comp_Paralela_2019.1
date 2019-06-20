#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <omp.h>
#include "aibb.h"

/* Função para ler matriz de um arquivo e armazenar em um array dinâmico
=> Parâmetros: Ponteiro de arquivo e ponteiro para linhas e colunas; 
=> Retorno: Referência na memória do array alocado. */
float *lerMatriz(char *str1, int *linhas, int *colunas){

int row, column;
int i, j, ch;
float x;

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

float *data = (float *)malloc(row * column * sizeof(float));

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
		fscanf(fp2,"%f",&x);
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
		fscanf(fp1,"%f",&x);
		*(data + i*column + j) = x;
	}
}

return data;
	
}

}

/* Função para imprimir matriz. Importante para depuração.
=> Parâmetros: Ponteiro de referência da matriz juntamente com linhas e colunas; 
=> Retorno: (void function) */
void imprimirMatriz(float *data, int row, int column){

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
void escreverMatriz(char *out_mat, float *data, int row, int column){

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
void multiplicarMatrizes(float *result, float *mat1, int line_m1, int col_m1, float *mat2, int line_m2, int col_m2){
	
	for(int i = 0; i < line_m1; i++){
		for(int j = 0; j < col_m2; j++){
            *(result + i*col_m2 + j) = 0;
			for(int k = 0; k < col_m1; k++){
				*(result + i*col_m2 + j) += *(mat1 + i*col_m1 + k) * *(mat2 + k*col_m2 + j);
			}
		}
	}	
}

/* Função para liberar memória após alocação dinâmica de vetores
=> Parâmetros: Ponteiro do array;
=> Retorno: (void function) */
void liberarMatriz(float *data){
	free(data);
}

/* Função serial para cálculo de distâncias pelo método de Bellman Ford 
=> Parâmetros: Número de vértices e matriz do grafo;
=> Retorno: (void function) */
void BellmanFord_serial(int vertices, float *mat1){

/*
This implementation takes in a graph, represented as
lists of vertices and edges, and fills two arrays
(distance and predecessor) about the shortest path
from the source to each vertex
*/

float distancia[vertices];
int predecessor[vertices];
int i, j, k;

/* Step 1: initialize graph */

for (i = 0; i < vertices; i++){
    distancia[i] = INFINITY;    /* Initialize the distance to all vertices to infinity */
    predecessor[i] = -1;        /* And having a null predecessor (-1) */
}

distancia[0] = 0;               /* The distance from the source to itself is, of course, zero */

/* Step 2: relax edges repeatedly */
for (i = 0; i < vertices-1; i++){
    for (j = 0; j < vertices; j++){
        for (k = 0; k < vertices; k++){
            if ((distancia[j] + *(mat1 + j*vertices + k)) < distancia[k]){
                distancia[k] = distancia[j] + *(mat1 + j*vertices + k);
                predecessor[k] = j;
            }
        }
    }
}

/* Step 3: check for negative-weight cycles */

for (j = 0; j < vertices; j++){
    if ((distancia[i] + *(mat1 + i*vertices + j)) > distancia[j])
        printf("Graph contains a negative-weight cycle\n");
}

/* Print distancy and predecessor */

for(i = 0; i < vertices; i++)
    printf("distancia[%d]: %.2f, predecessor[%d]: %d\n", i, distancia[i], i, predecessor[i]);

}

/* Função paralela para cálculo de distâncias pelo método de Bellman Ford 
=> Parâmetros: Número de vértices, matriz do grafo e quantidade de threads definidas no OMP;
=> Retorno: (void function) */
void BellmanFord_paralelo(int vertices, float *mat1, int p){

/*
This implementation takes in a graph, represented as
lists of vertices and edges, and fills two arrays
(distance and predecessor) about the shortest path
from the source to each vertex
*/

int i, j;
float weight;
float distancia[vertices];
int predecessor[vertices];

/* Assim como na atividade 12 iremos definir um início e um fim para o cálculo 
da atividade para cada thread */
int chunk_init[p], chunk_end[p];

/* Step 1: Encontrar os limites locais para cada thread a partir dos vértices */
int local_limit = vertices / p;

/* Inicializa laço paralelo */
#pragma omp parallel for
    for(i = 0; i < p; i++){
        chunk_init[i] = local_limit * i;
        chunk_end[i] = local_limit * (i + 1);

        if (i == p - 1) chunk_end[i] = vertices;
        
    }

/* Inicializa laço paralelo */
#pragma omp parallel for
    /* Step 2: initialize graph */
    for (i = 0; i < vertices; i++){
        distancia[i] = INFINITY;    /* Initialize the distance to all vertices to infinity */
        predecessor[i] = -1;        /* And having a null predecessor (-1) */
    }
    distancia[0] = 0;               /* The distance from the source to itself is, of course, zero */
    
int calc_thread_total, calc_thread[p];

#pragma omp parallel shared(chunk_init, chunk_end, distancia, predecessor, vertices, calc_thread) private(weight)
    {   
        int atual_thread = omp_get_thread_num();
        for (int k = 0; k < vertices - 1; k++){
            /* Step 3: relax edges repeatedly */
            calc_thread[atual_thread] = 0;
            for (int i = 0; i < vertices; i++){
                for (int j = chunk_init[atual_thread]; j < chunk_end[atual_thread]; j++){
                    weight = *(mat1 + i*vertices + j);
                    if (*(mat1 + i*vertices + j) < INFINITY){
                        if ((distancia[i] + weight) < distancia[j]){
                            distancia[j] = distancia[i] + weight;
                            predecessor[j] = i;
                            calc_thread[atual_thread] = 1;
                        }
                    }
                }
            }
#pragma omp barrier
#pragma omp single
            {
                calc_thread_total = 0;
                for (int thread_who = 0; thread_who < p; thread_who++)    calc_thread_total != calc_thread[thread_who];
                
            }

            if (!calc_thread_total) break;
            
        }
    }

    for (int i = 0; i < vertices; i++)
        printf("distancia[%d]: %.2f, predecessor[%d]: %d\n", i, distancia[i], i, predecessor[i]);
    
}