#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/*
####################################################
=> Execução serial
# gcc integral_serial.c -o serial -lm
# time ./serial 1.0 10000.0 80000000
####################################################
=> Execução paralela
# gcc -fopenmp integral_paralelo.c -o paralelo -lm
# time ./serial 1.0 10000.0 80000000
####################################################
*/

double f(double x) {
   double return_val = 0.0;
   // return_val = x*x*x*x - x*x*x + x*x - x + 1;
   return_val = cos(x * log(1.0/x));
   return return_val;
}

int main(int argc, char *argv[]) {
   // Valor da integral
   double integral; 
   // Limites do intervalo
   double a, b;
   // Número de trapézios
   int n;
   // Base do trapézio
   double h;
   double x;
   int i;

   a = atof(argv[1]);
   b = atof(argv[2]);
   n = atoi(argv[3]);

   h = (b - a) / n;
   integral = (f(a) + f(b))/2.0;

   //integral *= h;
   //printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral);

   // Fazer redução
   #pragma omp parallel shared(n, h, a, b) private(x, i) reduction(+:integral)
   
   {
      x = (((b - a) / omp_get_num_threads()) * omp_get_thread_num()) + a;
         
      for (i = 0; i < (n / omp_get_num_threads()); i++) {
         x += h;
         integral += f(x);
      }
   }

   integral *= h;
   printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral);

   return 0;
}