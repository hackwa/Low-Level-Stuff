#include<stdio.h>
#include "omp.h"
#include "pipeline.h"

char *source = "./instructions";

int main()
{
	omp_set_num_threads(5);
	char *current_instruction = NULL;
	instruction decoded;
	int result = -1
	FILE * fp = fopen(source, "r");
	if (fp == NULL)
	{
		printf("Can't find input file!\n");
		return 1;
	}
	#pragma omp parallel
	{
	int id=omp_get_thread_num();
	printf("Hello(%d)",id);
	printf("World(%d)\n",id);
	switch(id)
	{
		case 0: issue();
			break;
		case 1: decode();
			break;
		case 2: execute();
			break;
		case 3: memory_access();
			break;
		case 4: writeback();
			break;
		default: printf("This should not get executed");
	}
	}
	return 0;
}

void issue()
{
	printf("issue\n");
}
void decode()
{
	printf("decode\n");
}
void execute()
{
	printf("execute\n");
}
void memory_access()
{
	printf("memory_access\n");
}
void writeback()
{
	printf("writeback");
}
