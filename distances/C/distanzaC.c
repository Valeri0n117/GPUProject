#include "ocl_boilerV.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
//const int n=(1024);

clock_t start,stop; 

void stampa(float *distanze, int n){
	for(int i=0; i<(n*(n+1))/2; i++)
		printf("%f\n", distanze[i] );
	return;
}




int main(int argc, char *argv[]){
	int n= atoi(argv[1]);

	float *p = (float *)malloc(sizeof(float)*n*2);
	for(int i=0; i<n; ++i){
		p[i*2]=0;
		p[i*2+1]=(float)i;
	}

	float *distanze = (float *)malloc(sizeof(float)*(n*(n+1))/2);

	start=clock();
	for(int i=0; i<n; ++i)
		for(int j=0; j<n; ++j){
			distanze[( (i*(i+1))/2) +j]= sqrt( pow(p[i*2]-p[j*2], 2.0f) + pow(p[i*2+1]-p[j*2+1], 2.0f) );
		}
	stop=clock();

	/* PROVA INDICI
	for(int i=0; i<n; ++i)
		for(int j=0; j<n; ++j){
			if(i>=j){
				int x=( (i*(i+1))/2) +j;
				printf("%d\n", x);
			}
		}
		*/

	//stampa(distanze, n);

	printf("%f secondi\n", ((double)(stop-start))/CLOCKS_PER_SEC );

	free(p);
	free(distanze);

	return 0;
}
