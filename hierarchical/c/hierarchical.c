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

void printArray(float *array, int n){
	for(int i=0; i<(n*(n-1))/2; i++)
		printf("%f\n", array[i] );
	return;
}

void initPoints(float * points, int n){

	srand((unsigned int)time(NULL));
	for(int i=0; i<n; ++i){
		points[i*2]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));;
		points[i*2+1]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));;
	}
	
}

void initDistances(float * distances, float * p, int nPoints){

	for(int i=0; i<nPoints; ++i)
		for(int j=0; j<i; ++j){
			distances[((i*(i-1))/2)+j]= sqrt( pow(p[i*2]-p[j*2], 2.0f) + pow(p[i*2+1]-p[j*2+1], 2.0f) );
		}

}

void findMinDistances(float *distances, int nPoints, int *toMergeA, int *toMergeB){

	float tmp = distances[0];

	for(int i=0; i<nPoints; ++i)
		for(int j=0; j<i; ++j){
			if(distances[((i*(i-1))/2)+j] < tmp){
				tmp = distances[((i*(i-1))/2)+j];
				*toMergeA = i;
				*toMergeB = j;
			}
		}

}


int main(int argc, char *argv[]){
	if (argc != 3)
		error("usage: hierarchical nPoints nCluster ");

	int nPoints = atoi(argv[1]);
	int nCluster = atoi(argv[2]);

	float *points = (float *)malloc(sizeof(float)*2*nPoints);
	int *clusterID = (int *)malloc(sizeof(int)*nPoints);
	//float *clusterID = (float *)malloc(sizeof(float)*2*nPoints);
	float *distances = (float *)malloc(sizeof(float)*(nPoints*(nPoints-1))/2);
	int toMergeA, toMergeB;

	initPoints(points, nPoints);
	initDistances(distances, points, nPoints);
	findMinDistances(distances, nPoints, &toMergeA, &toMergeB);

	printf("toMergeA: %i\n toMergeB: %i\n", toMergeA, toMergeB);
	
}