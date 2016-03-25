#include<stdio.h>
#include "omp.h"
#include "pipeline.h"

char *source = "./instructions";
int clock = 0;
int halt =0;

int main()
{
	FILE * fp = fopen(source, "r");
	if (fp == NULL)
	{
		printf("Can't find input file!\n");
		return 1;
	}

	instruction *issue;
	instruction *decode;
	instruction *execute;
	instruction *memory_access;
	instruction *writeback;

	omp_set_num_threads(6);
	#pragma omp parallel
	{
	int id=omp_get_thread_num();
	//printf("Hello(%d)",id);
	//printf("World(%d)\n",id);
	switch(id)
	{
		case 0: cpu_clock();
			break;
		case 1: cpu_issue(fp,issue);
			break;
		case 2: cpu_decode(decode,issue);
			break;
		case 3: cpu_execute(execute,decode);
			break;
		case 4: cpu_memory_access(memory_access,execute);
			break;
		case 5: cpu_writeback(writeback,execute);
			break;
		default: printf("This should not get executed");
	}
	}
	return 0;
}

// Stop clock 5 cycles after halt is Issued 
void cpu_clock(){
	int ctr = 0;
	while(1)
	{
		for(int i=10000;i>0;i--)
			for(int j=10000; j>0;j--);
		clock = clock ^ 1;
		printf("clock %d\n",clock);
		if (halt == 1 && ctr++ == PIPELINE_DEPTH)
			break;
	}
}

void cpu_issue(FILE *fp, instruction *issue){
	int flag = 0;
	instruction issued;
	issue = &issued;
	while(1)
	{
		if(clock == 1 && flag == 1){
		if(fgets(issued.string,sizeof(issued.string),fp)== NULL) break;
		printf("%s",issued.string);
		flag = 0;
		}
		else 
		if (clock == 0){
			issue = NULL;
			flag =1;
		}
	}
	halt = 1;
	printf("issue\n");
}

void cpu_decode(instruction* decode, instruction* issue){
	int ctr = 0;
	printf("decode\n");
}

void cpu_execute(instruction* execute, instruction* decode){
	int ctr = 0;
	printf("execute\n");
}

void cpu_memory_access(instruction* memory_access, instruction* execute){
	int ctr = 0;
	printf("memory_access\n");
}

void cpu_writeback(instruction* writeback, instruction* execute){
	int ctr = 0;
	printf("writeback");
}
