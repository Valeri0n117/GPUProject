#include "ocl_boilerV.h"
#include <math.h>

int error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

/*  for this project the initialization speed is irrilevant, is more important to have a pseudo random set generated with a seed because
	we need to compare the C version and the OpenCL version
size_t preferred_wg_initPoints;

cl_event initPoints(cl_command_queue que, cl_kernel initPoints_k, cl_int nPoints,
						cl_mem points){
	//TO DO
}

*/

void initPoints(int n, float *points){

	for (int i=0; i<n; i++){
		//points[i*2]=i*10;
		//points[i*2+1]=i*10;

		points[i*2]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));
		points[i*2+1]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));
	}
}

size_t preferred_wg_initClusterID;

cl_event initClusterID(cl_command_queue que, cl_kernel initClusterID_k, cl_int nPoints
						cl_mem clusterID){
	cl_int err;
	size_t gws[] = {round_mul_up(nPoints, preferred_wg_initClusterID)};
	cl_event = in_evt;

	err = clSetKernelArg(initClusterID_k, 0, sizeof(nPoints), &nPoints);
	ocl_check(err, "set initClusterID arg 0");
	err = clSetKernelArg(initClusterID_k, 1, sizeof(points), &points);
	ocl_check(err, "set initClusterID arg 1");queueNDRangeKernel

	err = clEnqueueNDRangeKernel(que, initClusterID_k,
									1, NULL, gws, NULL, //launch grid
									0, NULL, &initClusterID_evt); //waitlist
	ocl_check(err, "launching initClusterID");
}


//test to do "how many iteration will avoid a min/max ceck on centroid's initialization?"
size_t preferred_wg_initRandomCentroids;

cl_event initRandomCentroids(cl_command_queue que, cl_kernel initRandomCentroids_k, cl_int nClusters,
								cl_mem centroid, cl_int nPoints, cl_mem points ){
	//TO DO
}


size_t preferred_wg_assignPoints;

cl_event assignPoints(cl_command_queue que, cl_kernel assignPoints_k, cl_int nPoints, cl_mem points
						cl_mem clusterID, cl_mem distances, cl_int nClusters, cl_mem centroids, 
						cl_int continue){
	//TO DO
}


size_t preferred_wg_adjustCentroids(cl_command_queue que, cl_kernel adjustCentroids_k, cl_int nClusters
									cl_mem centroids, cl_int nPoints, cl_mem points, cl_mem clusterID){
	//TO DO
}


int main(int argc, char *argv[]){

	if(argc != 3)
		error("USAGE: number_of_points number_of_clusters");

	int nPoints = atoi(argv[1]);
	int nClusters = atoi(argv[2]);

	if(nPoints <= 0 || nClusters <= 0 )
		error("number of points must be positive");

	cl_platform_id p = select_platform();
	cl_device_id d = select_device(p);
	cl_context ctx = create_context(p, d);
	cl_command_queue que = create_queue(ctx, d);
	cl_program prog = create_program("distanza.ocl", ctx, d);
	cl_int err;

	//memory allocation

	cl_mem points = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(float)*nPoints*2, NULL, &err);
	ocl_check(err, "allocation of points");

	cl_mem clusterID = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(int)*nPoints, NULL, &err);
	ocl_check(err, "allocation of clusterID");

	cl_mem distances = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(float)*nPoints, NULL, &err);
	ocl_check(err, "allocation of distances");

	cl_mem centroids = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(float)*nClusters, NULL, &err);
	ocl_check(err, "allocation of centroids");


	//loading kernels
	
/*	see line 9
	cl_kernel initPoints_k = clCreateKernel(prog, "initPoints", &err);
	ocl_check(err, "create kernel initPoints"); */

	cl_kernel initClusterID_k = clCreateKernel(prog, "initPoints", &err);
	ocl_check(err, "create kernel initClusterID");

	cl_kernel initRandomCentroids_k = clCreateKernel(prog, "initRandomCentroids", &err);
	ocl_check(err, "create kernel initRandomCentroids");

	cl_kernel assignPoints_k = clCreateKernel(prog, "assignPoints", &err);
	ocl_check(err, "create kernel assignPoints");

	cl_kernel adjustCentroids_k = clCreateKernel(prog, "adjustCentroids", &err);
	ocl_check(err, "create kernel adjustCentroids");



	//estimating workgroup preferred size

/*	see line 9
	err = clGetKernelWorkGroupInfo(initPoints_k, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_initPoints), &preferred_wg_initPoints, NULL);
	ocl_check(err, "WG info initPoints"); */

	err = clGetKernelWorkGroupInfo(initClusterID, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_initClusterID), &preferred_wg_initClusterID, NULL);
	ocl_check(err, "WG info initClusterID");

	err = clGetKernelWorkGroupInfo(initRandomCentroids_k, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_initRandomCentroids), &preferred_wg_initRandomCentroids, NULL);
	ocl_check(err, "WG info initRandomCentroids");

	err = clGetKernelWorkGroupInfo(assignPoints_k, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_assignPoints), &preferred_wg_assignPoints, NULL);
	ocl_check(err, "WG info assignPoints");

	err = clGetKernelWorkGroupInfo(adjustCentroids_k, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_adjustCentroids), &preferred_wg_adjustCentroids, NULL);
	ocl_check(err, "WG info adjustCentroids");


	
	cl_event initClusterID_evt = initClusterID(que, initClusterID_k, nPoints, clusterID);
	







}