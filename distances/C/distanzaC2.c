#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

clock_t start,stop; 

int error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

void stampa(float *distanze, int n){
	for(int i=0; i<(n*(n-1))/2; i++)
		printf("%f\n", distanze[i] );
	return;
}


void verifyDyagonal(float *distanze, int n){

	for(int i=1; i<n ; i++)
		if(distanze[((i*(i-1))/2)+i-1] != 1 )
			printf("%d not one \n", i);

}




int main(int argc, char *argv[]){

	if (argc < 2)
		error("inserire il numero di elementi");

	int n= atoi(argv[1]);

	//allocation matrix for locations of points
	float *p = (float *)malloc(sizeof(float)*n*2);
	for(int i=0; i<n; ++i){
		p[i*2]=0;
		p[i*2+1]=(float)i;
	}

	//allocation triangular matrix
	float *distanze = (float *)malloc(sizeof(float)*(n*(n-1))/2);

	start=clock();
	for(int i=0; i<n; ++i)
		for(int j=0; j<i; ++j){
			distanze[((i*(i-1))/2)+j]= sqrt( pow(p[i*2]-p[j*2], 2.0f) + pow(p[i*2+1]-p[j*2+1], 2.0f) );
		}
	stop=clock();

	verifyDyagonal(distanze, n);
	//stampa(distanze,n);

	printf("%f secondi\n", ((double)(stop-start))/CLOCKS_PER_SEC );

	free(p);
	free(distanze);

	return 0;
}

/* index verification
	for(int i=0; i<n; ++i)
		for(int j=0; j<n; ++j){
			if(i>=j){
				int x=( (i*(i+1))/2) +j;
				printf("%d\n", x);
			}
		}


		for(int i=1; i<n; ++i)
			for(int j=0; j<i; ++j){
				int x=( (i*(i-1))/2)+j;
				printf("%d\n", x);
			}

		*/

