#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 

int main(int argc, char *argv[]){

FILE *fp1;
FILE *fp2;
FILE *fp3;
FILE *fp4;
FILE *fp5;

char str1[50], str2[50], str3[50];
int control = 0;
int row, column, v_size, order = 0;
int i, j, ch;
float x, y;

strcpy(str1, argv[1]);
strcpy(str2, argv[2]);
strcpy(str3, argv[3]);

//printf("argv 0 %s \n", str1);
//printf("argv 1 %s \n", str2);
//printf("argv 2 %s \n", str3);


fp1 = fopen(str1,"r");
fp2 = fopen(".mat_dot.txt","w");
fp3 = fopen(".vec_dot.txt","w");
fp4 = fopen(str2,"r");

if(fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL){
	printf("Files do not exist!!!\n");
	exit(1);
}	          

/* Get rows and numbers of matrix */

fscanf(fp1,"%d",&row);
fscanf(fp1,"%d",&column);	

/* Removing symbols of ":" and creating a new file of matrix without ':' */

while((ch = fgetc(fp1)) != EOF){

	if((ch >= '0' && ch <= '9' || ch == '.')) 	
		fputc(ch,fp2); 
	else	
		fputc(32,fp2);

	if(ch == '\n') fputc('\n',fp2);
		
}        

/* Testing getting rows and columns */

//printf("linhas %d\n", row);
//printf("colunas %d\n", column);

/* Vector with rows and colums */

float data[row][column];

/* Get rows of vector */

fscanf(fp4,"%d",&v_size);

/* removing symbols of ":" and creating a new file of vector without ':' */

while((ch = fgetc(fp4)) != EOF){
			 
	if(ch >= '0' && ch <= '9' || ch == '.')
		fputc(ch,fp3);
	else
		fputc(32,fp3);

	if(ch == '\n') fputc('\n',fp2);
			
} 

/* Testing getting rows */

//printf("linhas vector %d\n", v_size);

//10x100 x 100x1 = 10 x 1

/* Creating vectors to store the final computation */

float vector[v_size];
float result[row];

/* Initializing result vector with 0 */

for (i = 0; i < row; i++){
	result[i] = 0;
}

fclose(fp1);
fclose(fp2);
fclose(fp3);
fclose(fp4);

/* print values in matrix */

fp2 = fopen(".mat_dot.txt","r");

for (i = 0; i < row; i++){
	for(j = 0; j < column; j++){
		fscanf(fp2,"%f",&x);
		data[i][j] = x;
		//printf("data[%d][%d] = %.2f \n", i, j, data[i][j]);
	}
}

/* print values in vector */

fp3 = fopen(".vec_dot.txt","r");

if(fp3 == NULL){
  printf("Error! File do not exist!!!\n");   
  exit(1);             
}


for (i = 0; i < v_size; i++){
	fscanf(fp3,"%f",&y);
	vector[i] = y;
	//printf("vector[%d] = %.2f \n", i, vector[i]);
}

printf("\n");
fclose(fp2);
fclose(fp3);

/* Computation of matrix and vector */

for(i = 0; i < row; i++){
	for(j = 0; j < column; j++){
				result[i] += (data[i][j]*vector[j]);
		}
}

/* Open file for store results */

fp5 = fopen(str3,"w");

if(fp5 == NULL){
  printf("Error! File do not exist!!!\n");   
  exit(1);             
}

fprintf(fp5,"%d",row);
fputc('\n', fp5);

/* Show and store results in a file */

for(i = 0; i < row; i++){
	//printf("resultante[%i] = %.2f \n", i, result[i]);
	fprintf(fp5,"%.2f",result[i]);
	control++;
	if(control < row) fputc(':', fp5);
	else fputc('\n', fp5);
}

fclose(fp5);
return 0;

}