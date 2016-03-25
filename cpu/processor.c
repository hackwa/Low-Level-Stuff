#include<stdio.h>
#include "omp.h"
#include "pipeline.h"

char *source = "./instructions";

int main()
{
	FILE * fp = fopen(source, "r");
	if (fp == NULL)
	{
		printf("Can't find input file!\n");
		return 1;
	}

	instruction *fetch;
	instruction *decode;
	instruction *execute;
	instruction *memory_accesse;
	instruction *writeback;

	omp_set_num_threads(5);
	#pragma omp parallel
	{
	int id=omp_get_thread_num();
	printf("Hello(%d)",id);
	printf("World(%d)\n",id);
	switch(id)
	{
		case 0: cpu_issue();
			break;
		case 1: cpu_decode();
			break;
		case 2: cpu_execute();
			break;
		case 3: cpu_memory_access();
			break;
		case 4: cpu_writeback();
			break;
		default: printf("This should not get executed");
	}
	}
	return 0;
}

void cpu_issue()
{
	printf("issue\n");
}
void cpu_decode()
{
	printf("decode\n");
}
void cpu_execute()
{
	printf("execute\n");
}
void cpu_memory_access()
{
	printf("memory_access\n");
}
void cpu_writeback()
{
	printf("writeback");
}
