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

		//srand((unsigned int)time(NULL));
		//points[i*2]=rand();
		//points[i*2+1]=rand();
	}
}

void initRandomCentroid(int nCluster, float *centroids, int nPoints, int *points){

	srand((unsigned int)time(NULL));

	int minX, maxX, minY, maxY;
	minX = maxX = points[0];
	minY = maxY = points[1];

	for (int i=1; i < nPoints; i++){
		if(points[i*3] < minX) minX = points[i*3];
		if(points[i*3] > maxX) maxX = points[i*3];
		if(points[i*3+1] < minY) minY = points[i*3+1];
		if(points[i*3+1] > maxY) maxY = points[i*3+1];
	}

	for (int i=0; i < nCluster; i++){
		centroids[i*3]= (float)rand()/(float)(RAND_MAX) * maxX + minX;
		centroids[i*3+1]= (float)rand()/(float)(RAND_MAX) * maxY + minY;	
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
		printf("%i\n", points[i*3]);
		printf("%i\n", points[i*3+1]);
	}
}

void printDistances(int n, float *distances){

	for(int i=0; i<n; i++)
		printf("%f\n", distances[i] );
}

void assignPoints(int nPoints, int *points, int nCluster, float *centroids){

	/* TO DO */


}


int main (int argc, char *argv[]){

	if (argc < 3)
		error("USAGE: kmeans nPoints nCluster");

	int nPoints = atoi(argv[1]);
	//int nDistances = (nPoints*(nPoints-1))/2;
	int nCluster = atoi(argv[2]);

	int *points = (int *)malloc(sizeof(int)*nPoints*3);  // (X, Y, CLUSTER_ID)
	float *centroids = (float *)malloc(sizeof(float)*nCluster);
	//float *distances = (float *)malloc(sizeof(float)*nDistances);


	initPoints(nPoints, points);
	initRandomCentroid(nCluster, centroids, nPoints, points);


	//printPoints(nPoints, points);
	//initDistances(nDistances,points, distances);
	//printDistances(nDistances, distances);






}