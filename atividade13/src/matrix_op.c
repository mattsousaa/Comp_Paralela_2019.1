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
void BellmanFord_serial(float *mat1, int v_grafo){

/*
This implementation takes in a graph, represented as
lists of vertices and edges, and fills two arrays
(distance and predecessor) about the shortest path
from the source to each vertex
*/

float distancia[v_grafo];
int predecessor[v_grafo];
int i, j, k;

/* Step 1: initialize graph */

for (i = 0; i < v_grafo; i++){
    distancia[i] = INFINITY;    /* Initialize the distance to all vertices to infinity */
    predecessor[i] = -1;        /* And having a null predecessor (-1) */
}

distancia[0] = 0;               /* The distance from the source to itself is, of course, zero */

/* Step 2: relax edges repeatedly */
for (i = 0; i < v_grafo - 1; i++){
    for (j = 0; j < v_grafo; j++){
        for (k = 0; k < v_grafo; k++){
            if ((distancia[j] + *(mat1 + j*v_grafo + k)) < distancia[k]){
                distancia[k] = distancia[j] + *(mat1 + j*v_grafo + k);
                predecessor[k] = j;
            }
        }
    }
}

/* Step 3: check for negative-weight cycles */

/*
for (j = 0; j < v_grafo; j++){
    if ((distancia[i] + *(mat1 + i*v_grafo + j)) > distancia[j])
        printf("Graph contains a negative-weight cycle\n");
}*/

/* Print distancy and predecessor */

for(i = 0; i < v_grafo; i++)
    printf("distancia[%d]: %.2f, predecessor[%d]: %d\n", i, distancia[i], i, predecessor[i]);

}

/* Função paralela para cálculo de distâncias pelo método de Bellman Ford 
=> Parâmetros: Número de vértices, matriz do grafo e quantidade de threads definidas no OMP;
=> Retorno: (void function) */
void BellmanFord_paralelo(float *mat1, int v_grafo, int qtd_threads){

/*
This implementation takes in a graph, represented as
lists of vertices and edges, and fills two arrays
(distance and predecessor) about the shortest path
from the source to each vertex
*/

int i, j, k;
float weight;
int who_am_i;
float distancia[v_grafo];
int predecessor[v_grafo];

/* As variáveis abaixo calculam o trabalho global e local das threads */
int calc_thread_total, calc_thread[qtd_threads];

/* É precido definir os extremos para o cálculo local de cada thread.
As variáveis abaixo definem as partes de cada vértice (chunk) para execução. */
int chunk_init[qtd_threads], chunk_end[qtd_threads];

/* Step 1: Encontrar os limites locais para cada thread a partir dos vértices */
int local_limit = v_grafo / qtd_threads;


for(i = 0; i < qtd_threads; i++){
    chunk_init[i] = local_limit * i;
    chunk_end[i] = local_limit * (i + 1);

    if (i == qtd_threads - 1) chunk_end[i] = v_grafo;
    
}

/* Step 2: initialize graph */
for (i = 0; i < v_grafo; i++){
    distancia[i] = INFINITY;    /* Initialize the distance to all vertices to infinity */
    predecessor[i] = -1;        /* And having a null predecessor (-1) */
}
distancia[0] = 0;               /* The distance from the source to itself is, of course, zero */
    

/* Inicializa processo de paralização do código serial pela função "BellmanFord_serial" */
#pragma omp parallel shared(predecessor, v_grafo, calc_thread, distancia, chunk_init, chunk_end) private(weight)
    {   
        int atual_thread = omp_get_thread_num();
        for(int i = 0; i < v_grafo - 1; i++){
            /* Step 3: relax edges repeatedly */
            calc_thread[atual_thread] = 0;
            for(int j = 0; j < v_grafo; j++){
                for(int k = chunk_init[atual_thread]; k < chunk_end[atual_thread]; k++){
                    weight = *(mat1 + j*v_grafo + k);
                    if(*(mat1 + j*v_grafo + k) < INFINITY){
                        if((distancia[j] + weight) < distancia[k]){
                            distancia[k] = distancia[j] + weight;
                            predecessor[k] = j;
                            calc_thread[atual_thread] = 1;
                        } /*else{
                            printf("Graph contains a negative-weight cycle\n");
                        }*/
                    }
                }
            }

/* É necessário uma barreira de sincronização para todas as threads sincronizarem*/
#pragma omp barrier
#pragma omp single
        {
            calc_thread_total = 0;
            for(who_am_i = 0; who_am_i < qtd_threads; who_am_i++)    
                INIT_SYNCRONIZATION(calc_thread_total, calc_thread, who_am_i);   
        }

        /* Se o trabalho total de todas as threads acabar, então sai do laço paralelo */
        if(!calc_thread_total) break;
            
        }
    }

/* Print distancy and predecessor */

for(i = 0; i < v_grafo; i++)
    printf("distancia[%d]: %.2f, predecessor[%d]: %d\n", i, distancia[i], i, predecessor[i]);
    
}