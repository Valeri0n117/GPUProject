#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DIST(xa, ya, xb, yb) sqrt(pow(xa-xb, 2.0f) + pow(ya-yb, 2.0f) )



clock_t start,stop; 

int error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

void setIntArrayToNull(int nels, int* array){

	for(int i = 0; i < nels; i++)
		array[i]= -1;

}

void initPoints(int n, float *points){

	for (int i=0; i<n; i++){
		//points[i*2]=i*10;
		//points[i*2+1]=i*10;

		points[i*2]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));
		points[i*2+1]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));
	}
}

void initRandomCentroid(int nClusters, float *centroids, int nPoints, float *points){

	float minX, maxX, minY, maxY;
	minX = maxX = points[0];
	minY = maxY = points[1];

	for (int i=1; i < nPoints; i++){
		if(points[i*2] < minX) minX = points[i*2];
		if(points[i*2] > maxX) maxX = points[i*2];
		if(points[i*2+1] < minY) minY = points[i*2+1];
		if(points[i*2+1] > maxY) maxY = points[i*2+1];
	}

	for (int i=0; i < nClusters; i++){
		centroids[i*2]= (float)((double)rand()/(double)(RAND_MAX/maxX)) + minX;
		centroids[i*2+1]= (float)((double)rand()/(double)(RAND_MAX/maxX)) + minY;	
	}
}

void printPoints(int n, float *points){

	for(int i=0; i<n; i++){
		printf("%f  ", points[i*2]);
		printf("%f\n", points[i*2+1]);
	}

	printf("\n");
}

void printArray(int n, int *id, float *distances){

	for(int i=0; i<n; i++)
		printf("%i\t%f\n", id[i] , distances[i]);
	printf("\n");
}

_Bool assignPoints(int nPoints, float *points, int *clusterID, float *distances, int nClusters, float *centroids){

	float tmpDist;
	int oldClusterID;
	_Bool changes = 0;

	for (int i=0; i<nPoints; i++){

		oldClusterID = clusterID[i];
		clusterID[i]=0;		
		distances[i]= DIST(points[i*2], points[i*2+1], centroids[0], centroids[1]);

		for(int j=1; j<nClusters; j++){

			tmpDist=DIST(points[i*2], points[i*2+1], centroids[j*2], centroids[j*2+1]);

			if(tmpDist < distances[i]){
				clusterID[i]=j;
				distances[i]=tmpDist;
			}
		}
		
		if(oldClusterID != clusterID[i] ){
					changes=1;
				}
	}

	return changes;
}


void adjustCentroids(int nClusters, float *centroids, int nPoints, float *points, int *clusterID){
	float *sumX = (float *)malloc(sizeof(float)*nClusters);
	float *sumY = (float *)malloc(sizeof(float)*nClusters);
	int *countPoints = (int*)malloc(sizeof(int)*nClusters);

	for(int i=0; i<nPoints; i++){
		sumX[clusterID[i]]+=points[i*2];
		sumY[clusterID[i]]+=points[i*2+1];
		countPoints[clusterID[i]]++;
	}

	for(int j=0; j<nClusters; j++){
		centroids[j*2] = sumX[j]/countPoints[j];
		centroids[j*2+1] = sumY[j]/countPoints[j];
	}

}

float mediumCentroidsDistance(int nPoints, float *distances){

	float sumDist;
	for(int i=0; i < nPoints; i++){
		sumDist += distances[i];
	}

	return sumDist/nPoints;
}

int execToTermination(int nPoints, float *points, int *clusterID, 
	float *distances, int nClusters, float *centroids){
	
	setIntArrayToNull(nPoints, clusterID);
	_Bool changes = 1;
	int iteration = 0;

	while(changes){

		changes = assignPoints(nPoints, points, clusterID, distances, nClusters, centroids);
		adjustCentroids(nClusters, centroids, nPoints, points, clusterID);
		++iteration;

	}

	return iteration;

}

int execToThreshold(int threshold, int nPoints, float *points, int *clusterID, 
	float *distances, int nClusters, float *centroids){

	setIntArrayToNull(nPoints, clusterID);
	_Bool changes = 1;
	int iteration = 0;

	while(changes && iteration < threshold){

		changes = assignPoints(nPoints, points, clusterID, distances, nClusters, centroids);
		adjustCentroids(nClusters, centroids, nPoints, points, clusterID);
		++iteration;

	}

	return iteration;
}

int main (int argc, char *argv[]){

	srand((unsigned int)time(NULL));

	if (argc < 3 || argc > 4)
		error("USAGE: kmeans nPoints nClusters execution_limit(optional)");

	int nPoints = atoi(argv[1]);
	int nClusters = atoi(argv[2]);

	float *points = (float *)malloc(sizeof(float)*nPoints*2);  // (X, Y)
	int *clusterID = (int *)malloc(sizeof(int)*nPoints);
	float *distances = (float *)malloc(sizeof(float)*nPoints);
	float *centroids = (float *)malloc(sizeof(float)*nClusters*2);   // (X, Y)

	initPoints(nPoints, points);
	initRandomCentroid(nClusters, centroids, nPoints, points);

	int iteration;
	if(argc == 3)
		iteration = execToTermination(nPoints, points, clusterID, distances, 
					nClusters, centroids);
	else
		iteration = execToThreshold(atoi(argv[3]) ,nPoints, points, clusterID, distances, 
					nClusters, centroids);

	printf("number of iteration: %i\n", iteration);

}