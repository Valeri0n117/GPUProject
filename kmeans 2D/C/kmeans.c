#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DIST(i,j) (sqrt( pow(points[i*2]-points[j*2], 2.0f) + pow(points[i*2+1]-points[j*2+1], 2.0f) ));

clock_t start,stop; 

int error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

void initPoints(int n, int *points){

	for (int i=0; i<n; i++){
		points[i*2]=0;
		points[i*2+1]=i;

		//points[i*2]=rand();
		//points[i*2+1]=rand();
	}
}

void initDistances(int n, int * points, float *distances){

	for(int i=0; i<n; ++i)
		for(int j=0; j<i; ++j){
			distances[((i*(i-1))/2)+j]= DIST(i,j);
		}

}

void printPoints(int n, int *points){

	for(int i=0; i<n; i++){
		printf("%i\n", points[i*2]);
		printf("%i\n", points[i*2+1]);
	}
}

void printDistances(int n, float *distances){

	for(int i=0; i<n; i++)
		printf("%f\n", distances[i] );
}


int main (int argc, char *argv[]){

	if (argc < 2)
		error("insert number of elements");

	int nPoints = atoi(argv[1]);
	int nDistances = (nPoints*(nPoints-1))/2;

	int *points = (int *)malloc(sizeof(int)*nPoints*2);
	float *distances = (float *)malloc(sizeof(float)*nDistances);

	initPoints(nPoints, points);
	//printPoints(nPoints, points);
	initDistances(nDistances,points, distances);
	printDistances(nDistances, distances);




}