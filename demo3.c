#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thpool.h"
#include "thpool.c"
#include "my_funcs.h"
#include "my_funcs.c"

double U_in[DATA_SIZE];
double Z_out[DATA_SIZE];

struct thread_data{
	int thread_id;
	double pp[30];

};

void *task(void* threadarg){
	printf("Calculatinf cost function!\n");	
	struct thread_data *my_data;

	my_data=(struct thread_data*)threadarg;
	double a=my_cost_func(my_data->pp);
}
int main(int argc, char **argv) {
	/******************test system dynamics*******************/
	// read initial pp
	double pp[30];
	FILE *ptr_file1;
	ptr_file1=fopen("pp_LF_test_LF1.txt","r");
	if (!ptr_file1){
			printf("Read pp failed!\n");
			return 1;
	}
	for (int i=0;i<30;i++)
		fscanf(ptr_file1,"%lf",&pp[i]);
			
	fclose(ptr_file1);
	
	for (int i=0;i<30;i++)
			printf("%.9f\n",pp[i]);
	// read current
	ptr_file1=fopen("U_in.txt","r");
	if (!ptr_file1){
		printf("Read Current failed!\n");
			return 1;
	}
	for (int i=0;i<DATA_SIZE;i++)
		fscanf(ptr_file1,"%lf",&U_in[i]);			
	fclose(ptr_file1);
	// read voltage
	ptr_file1=fopen("Z_out.txt","r");
	if (!ptr_file1){
		printf("Read Voltage failed!\n");
			return 1;}
	for (int i=0;i<DATA_SIZE;i++)
		fscanf(ptr_file1,"%lf",&Z_out[i]);			
	fclose(ptr_file1);
	/********************************************************/
	struct thread_data td[20];
	puts("Making threadpool with 4 threads");
	threadpool thpool=thpool_init(4);
	
	puts("Adding 50 tasks to threadpool");

	clock_t t1,t2;							
	t1=clock();
	int i=0;
	for ( i=0;i<50;i++)
			td[i].thread_id=i;
			for(int j=0;j<30;j++)
				td[i].pp[j]=pp[j];
			thpool_add_work(thpool,task,&td[i]);
	puts("Killing threadpool");
	thpool_wait(thpool);
	thpool_destroy(thpool);

//	double COST=my_cost_func(pp);
	t2=clock();
	//printf("HELLO! Cost= %.3f\n",COST);
	
	printf("Elapsed time =%.6fs\n",((double)(t2-t1))/CLOCKS_PER_SEC);
	return 0;

}


