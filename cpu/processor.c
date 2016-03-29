#include "stdio.h"
#include "omp.h"
#include "pipeline.h"
#include "unistd.h"
#include "stdlib.h"

// ARM like instruction set
// Be careful of Hazards!

char *source = "./instructions";
int clock_signal = CLK_LOW;
int halt =0;

int main()
{
	FILE * fp = fopen(source, "r");
	if (fp == NULL)
	{
		printf("Can't find input file!\n");
		return 1;
	}

	instruction issue;
	instruction decode;
	instruction execute;

	omp_set_num_threads(5);
	#pragma omp parallel
	{
	int id=omp_get_thread_num();
	switch(id)
	{
		case 0: cpu_clock();
			break;
		case 1: cpu_issue(fp,&issue);
			break;
		case 2: cpu_decode(&decode,&issue);
			break;
		case 3: cpu_execute(&execute,&decode);
			break;
			// Registers and memory access in one step
		case 4: cpu_data_access(&execute);
			break;
		default: printf("Recheck the number of threads");
	}
	}
	printf("Execution Complete..CPU and Memory State:\n");
	printf("R0: %d\tR1: %d\tR2: %d\tR3: %d\n",R0,R1,R2,R3);
	printf("Memory:\n");
	for(int i=0;i<MEMSIZE;i++)
		printf("%d  ",MEMORY[i]);
	printf("\n");
	return 0;
}

// Stop clock PIPELINE_DEPTH cycles after halt is Issued 
void cpu_clock(){
	int ctr = 0;
	while(1)
	{
		nanosleep(&clockspec,NULL);
		clock_signal = clock_signal ^ 1;
		printf("clock %d\n",clock_signal);
		if (halt == 1 && ctr++ == 2*PIPELINE_DEPTH)
			break;
	}
	clock_signal = CLK_HALT;
}

void cpu_issue(FILE *fp, instruction *issue){
	int flag = 0;
	while(1)
	{
		if(clock_signal == CLK_HIGH && flag == 1){
		if(fgets(issue->string,sizeof(issue->string),fp)== NULL) break;
		issue-> valid = 1;
		//printf("%s",issue->string);
		flag = 0;
		nanosleep(&clockspec,NULL);
		}
		else 
// Decode shouldn't start until next instruction is issued
		if (clock_signal == CLK_LOW){
			issue -> valid = 0;
			flag =1;
			nanosleep(&clockspec,NULL);
		}
	}
	halt = 1;
	printf("Halting Pipeline in %d cycles\n",PIPELINE_DEPTH);
}

void cpu_decode(instruction* decode, instruction* issue){
	char *extracted;
	while(1)
	{
		if(clock_signal == CLK_HALT)break;

		if(clock_signal == CLK_HIGH && issue->valid == 1){			
			printf("decoding: %s",issue->string);
			extracted = strtok(issue->string," ");
			decode->type = extract_type(extracted);
			extracted = strtok(NULL," ");
			decode->dst = extract_register(extracted);
			extracted = strtok(NULL," ");
			if (decode->type == LDA || decode->type == STA)
			{
				decode->address = atoi(extracted);
				//printf("decoded address:%d",decode->address);
			}
			else
			if (decode->type == ADD || decode->type == SUB)
			{
			decode->src1 = extract_register(extracted);
			extracted = strtok(NULL," ");
			decode->src2 = extract_register(extracted);
			}
			else
			if(decode->type == MVI)
			{
				decode -> result = atoi(extracted);
			}
			issue -> valid = 0;
			decode -> valid = 1;
			nanosleep(&clockspec,NULL);
		}
		else
		if(clock_signal == CLK_LOW )
			decode-> valid = 0;

	}
}

void cpu_execute(instruction* execute, instruction* decode){
	printf("execute\n");
	while(1){
		if(clock_signal == CLK_HALT)break;
		if(clock_signal == CLK_HIGH && decode->valid == 1){			
		execute->type = decode-> type;
		execute-> dst = decode-> dst;
		switch(decode->type)
		{
			case LDA: 	execute->address = decode->address;break;
			case STA: 	execute->address = decode->address; break;
			case MVI:	execute-> result = decode -> result;break;
			case ADD: 	printf("Adding %d and %d\n",*(decode->src1),*(decode->src2));
					execute-> result = *(decode->src1) + *(decode-> src2);
					break;
			case SUB: 	printf("Subtracting %d from %d\n",*(decode->src1),*(decode->src2));
					execute-> result = *(decode->src1) - *(decode-> src2);
					break;
			default:	printf("ERR: Error in execution;%d",decode->type);
		}
		execute-> valid = 1;
		decode->valid = 0;
		nanosleep(&clockspec,NULL);
		}
		else
		if(clock_signal == CLK_LOW )
			execute-> valid = 0;
	}
}

void cpu_data_access(instruction* execute){
	printf("data_access\n");
	while(1){
		if(clock_signal == CLK_HALT)break;
		if(clock_signal == CLK_HIGH && execute->valid == 1){			
		switch(execute->type)
		{
			case LDA:	*(execute->dst) = MEMORY[execute->address];break;
			case STA: 	MEMORY[execute->address] = *(execute->dst);break;
			case ADD:	*(execute->dst) = execute->result;break;
			case SUB:	*(execute->dst) = execute->result;break;
			case MVI:	*(execute->dst) = execute->result;break;
			default:	printf("ERR: This should not happen");
		}
		execute->valid=0;
		nanosleep(&clockspec,NULL);
		}
	}
}

