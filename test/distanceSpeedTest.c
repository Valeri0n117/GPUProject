#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DIST(xa, ya, xb, yb) sqrt(pow(xa+xb, 2.0f) + pow(ya+yb, 2.0f) )

clock_t start,stop; 

void initPoints(int n, int *points){

	for (int i=0; i<n; i++){
		//points[i*2]=0;
		//points[i*2+1]=i;

		srand((unsigned int)time(NULL));
		points[i*2]=rand();
		points[i*2+1]=rand();
	}
}

void initRandomCentroid(int nClusters, float *centroids, int nPoints, int *points){

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

	for (int i=0; i < nClusters; i++){
		centroids[i*2]= (float)rand()/(float)(RAND_MAX) * maxX + minX;
		centroids[i*2+1]= (float)rand()/(float)(RAND_MAX) * maxY + minY;	
	}
	
	

}

int main(){

	int nPoints=2048;
	float *points= (float *)malloc(sizeof(float)*nPoints*2);
	float *distance= (float *)malloc(sizeof(float)*nPoints);
	float *distance2= (float *)malloc(sizeof(float)*nPoints);


	start = clock();

	for(int i = 0; i < nPoints; i++)
		distance[i] = DIST(points[i*2], points[i*2+1], 0, 0);

	stop = clock();	
	double timeC = (double)(stop-start)/CLOCKS_PER_SEC;
	printf("%f\n", timeC);

	start = clock();
	for(int i=0; i < nPoints; i++)
		distance2[i]=++distance[i];
	stop=clock();
	timeC = (double)(stop-start)/CLOCKS_PER_SEC;
	printf("%f\n", timeC);






}