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

void printD(float *distances, int n){
	for(int i=0; i<(n*(n+1))/2; i++)
		printf("%f\n", distances[i] );
	return;
}


//TO DO
void verifyDyagonal(float *distances, int n){

	for(int i=0; i<n ; i++)
		if(distances[((i*(i+1))/2)+i] != 0 )
			printf("%d not zero \n", i);

}




int main(int argc, char *argv[]){

	if (argc < 2)
		error("insert number of elements");

	int n= atoi(argv[1]);

	//allocation matrix for locations of points
	float *p = (float *)malloc(sizeof(float)*n*2);
	for(int i=0; i<n; ++i){
		p[i*2]=0;
		p[i*2+1]=(float)i;
	}

	//allocation triangular matrix
	float *distances = (float *)malloc(sizeof(float)*(n*(n+1))/2);

	start=clock();
	for(int i=0; i<n; ++i)
		for(int j=0; j<=i; ++j){
			distances[((i*(i+1))/2) +j]= sqrt( pow(p[i*2]-p[j*2], 2.0f) + pow(p[i*2+1]-p[j*2+1], 2.0f) );
		}
	stop=clock();

	verifyDyagonal(distances, n);
	//printD(distances,n);

	printf("%f secondi\n", ((double)(stop-start))/CLOCKS_PER_SEC );

	free(p);
	free(distances);

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


		for(int i=0; i<n; ++i)
		for(int j=0; j<=i; ++j){

			int x=( (i*(i+1))/2) +j;
			printf("%d\n", x);
		}

		*/

