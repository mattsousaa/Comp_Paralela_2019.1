#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ./criarMatriz 100 100 matriz100x100.txt
// qnt_linhas qnt_colunas arqArmazenar

int main(int argc, char *argv[]) {
   // Recuperar as dimensões da matriz.
   int linhas = atoi(argv[1]);
   int colunas = atoi(argv[2]);

   // Recuperar nome do arquivo para salvar a matriz.
   char *nomeDoArquivo = argv[3];

   // Abrir arquivo para escrita.
   FILE *arquivo = fopen(nomeDoArquivo, "w+");

   // Escreve as dimensões.
   fprintf(arquivo, "%d\n", linhas);
   fprintf(arquivo, "%d\n", colunas);
   int i,j;
   float valor = 1.0;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas-1; j++) {            
            fprintf(arquivo, "%05.2f:", valor);
        }
        fprintf(arquivo, "%05.2f", valor);
        fprintf(arquivo, "\n");
    }

   // Fecha arquivo.
   fclose(arquivo);

   return 0;
}