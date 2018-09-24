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

void initPointsAndCentroids(float * points, float *centroids, int n){

	srand((unsigned int)time(NULL));
	for(int i=0; i<n; ++i){
		centroids[i*2]=points[i*2]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));;
		centroids[i*2+1]=points[i*2+1]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));;
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
				*toMergeA = j;
				*toMergeB = i;
			}
		}

}

int mergeClusters(float *points, int toMergeA, int toMergeB, int *clusterID, 
					int nPoints, float *centroids, int nCluster, float *distances){
	//adjusting clusterID
	for(int i=0; i < nPoints; ++i){
		if(clusterID[i] == toMergeB)
			clusterID[i] == toMergeA;
	}

	//adjusting centroids of toMergeA;
	float sumX=0;
	float sumY=0;
	int n=0;

	for(int i=0; i < nPoints; ++i){
		if(clusterID[i] == toMergeA){
			sumX += points[i*2];
			sumY += points[i*2+1];
			++n;
		}
	}

	centroids[toMergeA*2]= sumX / n;
	centroids[toMergeA*2+1]= sumY / n;

	// ADJUST DISTANCES
		//toMergeA's distances
	for(int i=0; i < nCluster; ++i){
		if(i < toMergeA)
			distances[((toMergeA*(toMergeA-1))/2)+i] = sqrt( pow(centroids[i*2]-centroids[toMergeA*2], 2.0f) + pow(centroids[i*2+1]-centroids[toMergeA*2+1], 2.0f) );
		else
			distances[((i*(i-1))/2)+toMergeA] = sqrt( pow(centroids[i*2]-centroids[toMergeA*2], 2.0f) + pow(centroids[i*2+1]-centroids[toMergeA*2+1], 2.0f) );
	}

		//"removing" toMergeB distances
	if(toMergeB != (nCluster-1))
		for(int i=0; i<nCluster; ++i)
			if(i < toMergeB)
				distances[((toMergeB*(toMergeB-1))/2)+i] = distances[(((nCluster-1)*((nCluster-1)-1))/2)+i];
			else
				distances[((i*(i-1))/2)+toMergeB] = distances[(((nCluster-1)*((nCluster-1)-1))/2)+i];
	


	return --nCluster;
}




int main(int argc, char *argv[]){
	if (argc != 3)
		error("usage: hierarchical nPoints nCluster ");

	int nPoints = atoi(argv[1]);
	int nCluster = nPoints;
	int target = atoi(argv[2]);

	float *points = (float *)malloc(sizeof(float)*2*nPoints);
	int *clusterID = (int *)malloc(sizeof(int)*nPoints);
	float *centroids = (float *)malloc(sizeof(float)*2*nPoints);
	float *distances = (float *)malloc(sizeof(float)*(nPoints*(nPoints-1))/2);
	int toMergeA, toMergeB;

	initPointsAndCentroids(points, centroids, nPoints);
	initDistances(distances, points, nPoints);
	
	clock_t start,stop; 

	start = clock();
	while (nCluster > target){
		findMinDistances(distances, nPoints, &toMergeA, &toMergeB);
		nCluster = mergeClusters(points, toMergeA, toMergeA, clusterID, nPoints, centroids, nCluster, distances);
	}
	stop=clock();

	//printf("toMergeA: %i\ntoMergeB: %i\n", toMergeA, toMergeB);
	printf("%f secondi\n", ((double)(stop-start))/CLOCKS_PER_SEC );
	
}