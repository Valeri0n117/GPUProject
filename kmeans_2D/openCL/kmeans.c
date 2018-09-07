#include "ocl_boilerV.h"
#include <math.h>

int error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
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

	cl_mem ClusterID = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(int)*nPoints, NULL, &err);
	ocl_check(err, "allocation of clusterID");

	cl_mem distances = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(float)*nPoints, NULL, &err);
	ocl_check(err, "allocation of distances");

	cl_mem centroids = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(float)*nClusters, NULL, &err);
	ocl_check(err, "allocation of centroids");


	//loading kernels
	cl_kernel initPoints_k = clCreateKernel(prog, "initPoints", &err);
	ocl_check(err, "create kernel initPoints");

	cl_kernel initClusterID = clCreateKernel(prog, "initPoints", &err);
	ocl_check(err, "create kernel initClusterID");

	cl_kernel initRandomCentroids_k = clCreateKernel(prog, "initRandomCentroids", &err);
	ocl_check(err, "create kernel initRandomCentroids");

	cl_kernel assignPoints_k = clCreateKernel(prog, "assignPoints", &err);
	ocl_check(err, "create kernel assignPoints");

	cl_kernel adjustCentroids_k = clCreateKernel(prog, "adjustCentroids", &err);
	ocl_check(err, "create kernel adjustCentroids");
	









}