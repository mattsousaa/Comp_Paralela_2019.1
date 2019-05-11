#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
01. monteCarloPi(n) 
02. | acertos ← 0 
03. | para i ← 0 até n 
04. | | x ← sorteie um número real entre 0 e 1 
05. | | y ← sorteie um número real entre 0 e 1 
06. | | se(x * x + y * y < 1) 
07. | | | acertos ← acertos + 1 
08. | | fim_se 
09. | fim_para 
10. | retorne 4 * acertos / n 
11. fim_monteCarloPi

*/

double monteCarloPi(int n){

double i = 0;
double x = 0;
double y = 0;
double z = 0;
int acertos = 0;

srand(time(NULL));

for(i = 0; i < n; i++){

    x = (double)rand() / RAND_MAX;
    y = (double)rand() / RAND_MAX;

    z = x * x + y * y;

    if(z <= 1) acertos++;
}
    return (double) 4*acertos / n;
}

int main(int argc, char *argv[]){
    
    int n = atoi(argv[1]);
    double result;

    result = monteCarloPi(n);

    printf("pi = %.5f \n", result);
}

