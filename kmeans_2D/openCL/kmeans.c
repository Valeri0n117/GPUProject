#include "ocl_boilerV.h"
#include <math.h>

int error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}


void initPoints(int n, float *points){

	for (int i=0; i<n; i++){
		//points[i*2]=i*10;
		//points[i*2+1]=i*10;

		points[i*2]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));
		points[i*2+1]=(float)((double)rand()/(double)(RAND_MAX/999999.0f));
	}
}

size_t preferred_wg_initClusterID;

cl_event initClusterID(cl_command_queue que, cl_kernel initClusterID_k, cl_int nPoints,
						cl_mem clusterID_d){
	cl_int err;
	size_t gws[] = {round_mul_up(nPoints, preferred_wg_initClusterID)};
	cl_event initClusterID_evt;

	err = clSetKernelArg(initClusterID_k, 0, sizeof(nPoints), &nPoints);
	ocl_check(err, "set initClusterID arg 0");
	err = clSetKernelArg(initClusterID_k, 1, sizeof(clusterID_d), &clusterID_d);
	ocl_check(err, "set initClusterID arg 1");

	err = clEnqueueNDRangeKernel(que, initClusterID_k,
									1, NULL, gws, NULL, //launch grid
									0, NULL, &initClusterID_evt); //waitlist
	ocl_check(err, "launching initClusterID");

	return initClusterID_evt;
}

void initRandomCentroids(int nClusters, float *centroids, int nPoints, float *points){

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


size_t preferred_wg_assignPoints;

cl_event assignPoints(cl_command_queue que, cl_kernel assignPoints_k, cl_int nPoints, cl_mem points_d,
						cl_mem clusterID_d, cl_int nClusters, cl_mem centroids_d, cl_mem moreToDo, cl_event wait){
	
	cl_int err;
	size_t gws[] = {round_mul_up(nPoints, preferred_wg_assignPoints)};

	cl_event assignPoints_evt;

	err = clSetKernelArg(assignPoints_k, 0, sizeof(int), &nPoints);
	ocl_check(err, "set assignPoints arg 0");
	err = clSetKernelArg(assignPoints_k, 1, sizeof(points_d), &points_d);
	ocl_check(err, "set assignPoints arg 1");
	err = clSetKernelArg(assignPoints_k, 2, sizeof(clusterID_d), &clusterID_d);
	ocl_check(err, "set assignPoints arg 2");
	err = clSetKernelArg(assignPoints_k, 3, sizeof(int), &nClusters);
	ocl_check(err, "set assignPoints arg 3");
	err = clSetKernelArg(assignPoints_k, 4, sizeof(centroids_d), &centroids_d);
	ocl_check(err, "set assignPoints arg 4");
	err = clSetKernelArg(assignPoints_k, 5, sizeof(moreToDo), &moreToDo);
	ocl_check(err, "set assignPoints arg 5");
	if(wait != NULL){
		cl_event wait_list[] = {wait};
		err= clEnqueueNDRangeKernel(que, assignPoints_k,
									1, NULL, gws, NULL, //griglia di lancio
									1, wait_list, &assignPoints_evt);  //waiting list
		ocl_check(err,"launching assignPoints");
	}else {
		err= clEnqueueNDRangeKernel(que, assignPoints_k,
									1, NULL, gws, NULL, //griglia di lancio
									0, NULL, &assignPoints_evt);  //waiting list
		ocl_check(err,"launching assignPoints");
	}

	return assignPoints_evt;

}

cl_event setMoreToDo(cl_command_queue que, cl_kernel setMoreToDo_k, cl_mem moreToDo_d, cl_int set, cl_event wait){

	cl_int err;
	size_t gws[] = {1};

	cl_event setMoreToDo_evt;

	err = clSetKernelArg(setMoreToDo_k, 0, sizeof(moreToDo_d), &moreToDo_d);
	ocl_check(err, "set setMoreToDo arg 0");
	err = clSetKernelArg(setMoreToDo_k, 1, sizeof(int), &set);
	ocl_check(err, "set setMoreToDo arg 1");

	cl_event wait_list[] = {wait};
	err = clEnqueueNDRangeKernel(que, setMoreToDo_k,
									1, NULL, gws, NULL,
									1, wait_list, &setMoreToDo_evt);
	ocl_check(err, "launching setMoreToDo");

	return setMoreToDo_evt;
}


size_t preferred_wg_adjustCentroids;

cl_event adjustCentroids(cl_command_queue que, cl_kernel adjustCentroids_k, cl_int nClusters,
									cl_mem centroids_d, cl_int nPoints, cl_mem points_d, cl_mem clusterID_d, cl_event wait){

	cl_int err;
	size_t gws[] = {round_mul_up(nClusters, preferred_wg_assignPoints)};

	cl_event adjustCentroids_evt;

	err = clSetKernelArg(adjustCentroids_k, 0, sizeof(int), &nClusters);
	ocl_check(err, "set adjustCentroids arg 0");
	err = clSetKernelArg(adjustCentroids_k, 1, sizeof(centroids_d), &centroids_d);
	ocl_check(err, "set adjustCentroids arg 1");
	err = clSetKernelArg(adjustCentroids_k, 2, sizeof(int), &nPoints);
	ocl_check(err, "set adjustCentroids arg 2");
	err = clSetKernelArg(adjustCentroids_k, 3, sizeof(points_d), &points_d);
	ocl_check(err, "set adjustCentroids arg 3");
	err = clSetKernelArg(adjustCentroids_k, 4, sizeof(clusterID_d), &clusterID_d);
	ocl_check(err, "set adjustCentroids arg 4");

	cl_event wait_list[] = {wait};
	err= clEnqueueNDRangeKernel(que, adjustCentroids_k,
		1, NULL, gws, NULL, //griglia di lancio
		1, wait_list, &adjustCentroids_evt);  //waiting list
	ocl_check(err,"launching adjustCentroids");

	return adjustCentroids_evt;
}


cl_ulong total_runtime_ns(cl_event first, cl_event last){

	cl_int err;
	cl_ulong start, end;
	err = clGetEventProfilingInfo(first, CL_PROFILING_COMMAND_START,
		sizeof(start), &start, NULL);
	ocl_check(err, "get start");
	err = clGetEventProfilingInfo(last, CL_PROFILING_COMMAND_END,
		sizeof(end), &end, NULL);
	ocl_check(err, "get end");
	return (end - start);
}

double total_runtime_ms(cl_event first, cl_event last){
	return total_runtime_ns(first, last)*1.0e-6;
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
	cl_program prog = create_program("kmeans.ocl", ctx, d);
	cl_int err;

	//memory allocation

	float *points = (float *)malloc(sizeof(float)*nPoints*2);
	float *centroids = (float *)malloc(sizeof(float)*nClusters*2);
	initPoints(nPoints, points);
	initRandomCentroids(nClusters, centroids, nPoints, points);

	cl_mem points_d = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*nPoints*2, points, &err);
	ocl_check(err, "allocation of points");

	cl_mem centroids_d = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*nClusters*2, centroids, &err);
	ocl_check(err, "allocation of clusterID");
	//starting from here, the host copy of "points" and "clusterID" is no more needed, I will add a free() after some study of clCreateBuffer's flags

	cl_mem clusterID_d = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(int)*nPoints, NULL, &err);
	ocl_check(err, "allocation of centroids");

	cl_mem moreToDo_d = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(int), NULL, &err);
	ocl_check(err, "allocation of moreToDo");


	//loading kernels

	cl_kernel initClusterID_k = clCreateKernel(prog, "initClusterID", &err);
	ocl_check(err, "create kernel initClusterID");

	cl_kernel assignPoints_k = clCreateKernel(prog, "assignPoints", &err);
	ocl_check(err, "create kernel assignPoints");

	cl_kernel setMoreToDo_k = clCreateKernel(prog, "setMoreToDo", &err);
	ocl_check(err, "create kernel setMoreToDo");

	cl_kernel adjustCentroids_k = clCreateKernel(prog, "adjustCentroids", &err);
	ocl_check(err, "create kernel adjustCentroids");



	//estimating workgroup preferred size

	err = clGetKernelWorkGroupInfo(initClusterID_k, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_initClusterID), &preferred_wg_initClusterID, NULL);
	ocl_check(err, "WG info initClusterID");
	
	err = clGetKernelWorkGroupInfo(assignPoints_k, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_assignPoints), &preferred_wg_assignPoints, NULL);
	ocl_check(err, "WG info assignPoints");

	err = clGetKernelWorkGroupInfo(adjustCentroids_k, d, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
									sizeof(preferred_wg_adjustCentroids), &preferred_wg_adjustCentroids, NULL);
	ocl_check(err, "WG info adjustCentroids"); 



	cl_event initClusterID_evt = initClusterID(que, initClusterID_k, nPoints, clusterID_d);

	int *moreToDo = (int*)malloc(sizeof(int));
	*moreToDo = 1;

	cl_event assignPoints_evt, setMoreToDo_evt, adjustCentroids_evt, copyMoreToDo_evt;
	cl_event first_event;
	adjustCentroids_evt = NULL;

	/* just a try */ _Bool first=1;

	while(*moreToDo){

		*moreToDo=0;

		assignPoints_evt = assignPoints(que, assignPoints_k, nPoints, points_d,
						clusterID_d, nClusters, centroids_d, moreToDo_d, adjustCentroids_evt);
		moreToDo = clEnqueueMapBuffer(que, moreToDo_d, CL_TRUE, CL_MAP_READ, 0, 
						sizeof(int), 1, &assignPoints_evt, &copyMoreToDo_evt, &err);
		ocl_check(err, "enqueue read moreToDo");
		if(first){first_event = assignPoints_evt; first = 0;}



		setMoreToDo_evt = setMoreToDo(que, setMoreToDo_k, moreToDo_d, 0, copyMoreToDo_evt);



		adjustCentroids_evt = adjustCentroids(que, adjustCentroids_k, nClusters,
									centroids_d, nPoints, points_d, clusterID_d, setMoreToDo_evt);

		printf("%i\n", *moreToDo);


	}


	clFinish(que);
	printf("%f\n", total_runtime_ms(first_event, adjustCentroids_evt) );
	







}