#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "omp.h"

/*
####################################################
=> Execução serial
# gcc integral_serial.c -o serial -lm
# time ./serial 1.0 10000.0 80000000
####################################################
=> Execução paralela
# mpicc -fopenmp integral_paralelo.c -o paralelo -lm
# time mpirun -np 8 ./paralelo 1.0 10000.0 80000000
####################################################
*/

double f(double x) {
   double return_val = 0.0;
   // return_val = x*x*x*x - x*x*x + x*x - x + 1;
   return_val = cos(x * log(1.0/x));
   return return_val;
}

int main(int argc, char *argv[]) {

    // Valor da integral global
    double integral_global = 0.0; 
    // Limites do intervalo
    double a, b;
    // Número de trapézios
    int n, i;
    // Variável local para armazenar o resultado de uma parte da integral em cada processo
    double calc_task = 0;
    // Base do trapézio
    double h;
    double x;
    double area_trapezio;

    /* Recebe os argumentos */
    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);
    
    /* Cálculo do tamanho do trapézio */ 
    h = (b - a) / n;

    /* Variáveis do MPI */
    int tasks_qtd, It_qtd, rank;

    /* Inicialização do MPI */
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &tasks_qtd);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Todos os processos calculam um x local - inicio da integral */
    x = (((b - a) / tasks_qtd) * rank) + a;
    
    double init_integral = x;                                       /* Início do cálculo */
    double final_integral = x + ((b - a) / tasks_qtd);  /* Fim do cálculo */
    
    /* Redução na variável "calc_task" */
    #pragma omp parallel shared(init_integral, final_integral, n, h) private(x, i, area_trapezio) reduction(+:calc_task)

    {
        /* Cada thread ficará responsável por uma parte da área do trapézio */
        area_trapezio = (((final_integral - init_integral) / omp_get_num_threads()) * omp_get_thread_num()) + init_integral;
        
        /* cada thread faz o cálculo de cada área */
        for(i = 0; i < ((n / tasks_qtd) / omp_get_num_threads()); i++){
            area_trapezio += h;
            calc_task += f(area_trapezio);
        }
    }

    /* A ideia é somar a contribuição de cada processo com uma operação de redução */
    MPI_Reduce(&calc_task, &integral_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    /* processo com rank = 0 consolida o cálculo total e soma a integral global */ 
    if(rank == 0){
        integral_global += (f(a) + f(b)) / 2.0;
        integral_global *= h;
        printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral_global);
    }

    MPI_Finalize();
    
    return 0;

}
