#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DIST(xa, ya, xb, yb) sqrt(pow(xa+xb, 2.0f) + pow(ya+yb, 2.0f) )



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

void initRandomCentroid(int nClusters, float *centroids, int nPoints, int *points){

	srand((unsigned int)time(NULL));

	int minX, maxX, minY, maxY;
	minX = maxX = points[0];
	minY = maxY = points[1];

	for (int i=1; i < nPoints; i++){
		if(points[i*2] < minX) minX = points[i*2];
		if(points[i*2] > maxX) maxX = points[i*2];
		if(points[i*2+1] < minY) minY = points[i*2+1];
		if(points[i*2+1] > maxY) maxY = points[i*2+1];
	}

	for (int i=0; i < nClusters; i++){
		centroids[i*2]= (float)rand()/(float)(RAND_MAX) * maxX + minX;
		centroids[i*2+1]= (float)rand()/(float)(RAND_MAX) * maxY + minY;	
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


/*OLD VERSION

void assignPoints(int nPoints, int *points, int nClusters, float *centroids, int *clusterID, float *distances){

	float bestDist, tmpDist;

	for(int i=0; i < nPoints; i++){

		float bestDist = sqrt( pow(points[i*3]-centroids[0], 2.0f) + pow(points[i*3+1]-centroids[1], 2.0f));
		points[i*3+2]=0;

		for(int j=1; j < nClusters, j++){

			tmpDist = sqrt( pow(points[i*3]-centroids[j*2], 2.0f) + pow(points[i*3+1]-centroids[j*2+1], 2.0f));

			if (tmpDist < bestDist){
				bestDist = tmpDist;
				points[i*3+2]= j;
			}
		}

	}
} */

void assignPoints(int nPoints, int *points, int *clusterID, float *distances, int nClusters, float *centroids){
	float tmpDist;

	for (int i=0; i<nPoints; i++){

		distances[i]= DIST(points[i*2], points[i*2+1], centroids[i*2], centroids[i+2+1]);
		clusterID[i]=0;

		for(int j=1; j<nClusters; j++){

			tmpDist=DIST(points[i*2], points[i+2+1], centroids[i*2], centroids[i*2+1]);

			if(tmpDist < distances[i]){
				clusterID[i]=j;
				distances[i]=tmpDist;
			}
		}

	}
}


void adjustCentroids(int nClusters, float *centroids, int nPoints, int *points, int *clusterID){
	int *sumX = (int *)malloc(sizeof(int)*nClusters);
	int *sumY = (int *)malloc(sizeof(int)*nClusters);
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


int main (int argc, char *argv[]){

	if (argc < 3)
		error("USAGE: kmeans nPoints nClusters");

	int nPoints = atoi(argv[1]);
	int nClusters = atoi(argv[2]);

	int *points = (int *)malloc(sizeof(int)*nPoints*2);  // (X, Y)
	int *clusterID = (int *)malloc(sizeof(int)*nPoints);
	float *distances = (float *)malloc(sizeof(float)*nPoints);
	float *centroids = (float *)malloc(sizeof(float)*nClusters*2);   // (X, Y)


	initPoints(nPoints, points);
	initRandomCentroid(nClusters, centroids, nPoints, points);


}