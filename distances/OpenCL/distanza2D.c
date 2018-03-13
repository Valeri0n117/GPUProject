#include "ocl_boilerV.h"
#include <math.h>

int error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

size_t preferred_wg_init;

cl_event init(cl_command_queue que, cl_kernel init_k, cl_mem posizioni, cl_int nels){

	cl_int err;
	size_t gws[] =  {round_mul_up(nels, preferred_wg_init)};
	cl_event init_evt;

	err= clSetKernelArg(init_k, 0, sizeof(posizioni), &posizioni);
	ocl_check(err,"set init arg 0");
	err= clSetKernelArg(init_k, 1, sizeof(nels), &nels);
	ocl_check(err,"set init arg 1");

	err= clEnqueueNDRangeKernel(que, init_k,
		1, NULL, gws, NULL, //griglia di lancio
		0, NULL, &init_evt);  //waiting list
	ocl_check(err,"incodamento init");

	return init_evt;

}

size_t preferred_wg_calcola;
cl_event calcola(cl_command_queue que, cl_kernel calcola_k, cl_mem posizioni,
	cl_mem distanze, cl_int nels, cl_event init_evt){

	cl_int err;
	size_t gws[]= { 
		round_mul_up(nels, preferred_wg_calcola),
		round_mul_up(nels, preferred_wg_calcola)};

	cl_event calcola_evt;

	err= clSetKernelArg(calcola_k, 0, sizeof(posizioni), &posizioni);
	ocl_check(err, "set calcola arg 0");
	err= clSetKernelArg(calcola_k, 1, sizeof(distanze), &distanze);
	ocl_check(err, "set calcola arg 1");
	err= clSetKernelArg(calcola_k, 2, sizeof(nels), &nels);
	ocl_check(err, "set calcola arg 2");

	cl_event wait_list[] = { init_evt };
	err= clEnqueueNDRangeKernel(que, calcola_k, 
		2, NULL, gws, NULL,
		1,wait_list, &calcola_evt);
	ocl_check(err, "incodamento calcola");

	return calcola_evt;
}

int verifyDiagonal(float *dist, int nels){
	int ret=0;
	for(int i=0; i<nels; ++i){
		if(dist[i*nels+i] != 0){
			ret=1;
			printf("%i diverso da zero: %f \n", i, dist[i*nels+i]);
		}
	}

	return ret;
}

 int verify(float *dist, int nels){
	float expected;
	int ret=0;
	for(int i=0; i<nels; ++i)
		for(int j=0; j<nels; ++j) {
			expected= sqrt(pow(i-j, 2.0f) + pow(i-j, 2.0f));
			if(dist[i*nels+j] != expected ){
				ret=1;
				printf("expected:%f calculated:%f \n", expected, dist[i*nels+j] );
			}

		}

} 

/*
void print(float *dist, int nels){

	for(int i=0; i<5; ++i){

		for(int j=0; j<5; ++j)
			printf("%f\n", dist[i*nels+j]  );
		printf("\n");
	}
	return;
} */




int main(int argc, char *argv[]){

	if (argc < 2)
		error("inserire il numero di elementi");

	int nels=atoi(argv[1]);
	if(nels<= 0)
		error("il numero di elementi deve essere maggiore di 0");

	const size_t memsize = sizeof(float)*nels;
	
	//hic sunt leones

	cl_platform_id p = select_platform();
	cl_device_id d = select_device(p);
	cl_context ctx = create_context(p, d);
	cl_command_queue que = create_queue(ctx, d);
	cl_program prog = create_program("distanza.ocl", ctx, d);
	cl_int err;

	//estrazione kernel

	cl_kernel init_k = clCreateKernel(prog, "init", &err);
	ocl_check(err, "create kernel init");
	cl_kernel calcola_k = clCreateKernel(prog, "calcola", &err);
	ocl_check(err, "create kernel calcola");

	err = clGetKernelWorkGroupInfo(init_k, d,
		CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
		sizeof(preferred_wg_init), &preferred_wg_init, NULL);
	err = clGetKernelWorkGroupInfo(calcola_k, d,
		CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
		sizeof(preferred_wg_calcola), &preferred_wg_calcola, NULL);

	//allocazione memoria

	cl_mem posizioni= clCreateBuffer(ctx, CL_MEM_READ_WRITE, memsize*2 , NULL, &err);
	ocl_check(err, "allocazione posizioni");
	cl_mem distanze= clCreateBuffer(ctx, CL_MEM_READ_WRITE, memsize*memsize, NULL, &err);
	ocl_check(err,"allocazione matrice distanze");

	cl_event init_evt= init(que, init_k, posizioni, nels);
	cl_event calcola_evt= calcola(que, calcola_k, posizioni, distanze, nels, init_evt);

	cl_event copy_evt;
	float *input = clEnqueueMapBuffer(que, distanze, CL_TRUE, CL_MAP_READ, 0, 
		memsize*memsize, 1, &calcola_evt, &copy_evt, &err);
	ocl_check(err,"incodamento MAP"); 


	printf("init time:\t%gms\t%gGB/s\n", runtime_ms(init_evt),
		(2.0*memsize*2)/runtime_ns(init_evt));
	printf("calcola time:\t%gms\t%gGB/s\n", runtime_ms(calcola_evt),
		(5.0*memsize*memsize)/runtime_ns(calcola_evt));
	printf("copy time:\t%gms\t%gGB/s\n", runtime_ms(copy_evt),
		 (1.0*memsize*memsize)/runtime_ns(copy_evt));

	//print(input, nels);
	

	return verifyDiagonal(input, nels);

	

}