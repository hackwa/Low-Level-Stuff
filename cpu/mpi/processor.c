#include "stdio.h"
#include "mpi.h"
#include "pipeline.h"
#include "unistd.h"
#include "stdlib.h"

// ARM like instruction set
// Be careful of Hazards!

const char *source = "./instructions";

int main(int argc, char* argv[])
{
	int numtasks,rank,len,rc;
	char hostname[MPI_MAX_PROCESSOR_NAME];

	FILE * fp = fopen(source, "r");
	if (fp == NULL)
	{
		printf("Can't find input file!\n");
		return 1;
	}

	rc=MPI_Init(&argc, &argv);

	if (rc != MPI_SUCCESS) {
		printf("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(hostname, &len);
	printf ("Number of tasks = %d My rank = %d Running on %s\n",numtasks,rank,hostname);

	switch(rank)
	{
		case 0: cpu_issue(fp);
			break;
		case 1: cpu_decode();
			break;
		case 2: cpu_execute();
			break;
			// Registers and memory access in one step
		case 3: cpu_data_access();
			break;
		default: printf("This process does nothing.");
	}
	MPI_Finalize();
}

void cpu_issue(FILE *fp){
	instruction issue;
	while(1)
	{
		if(fgets(issue.string,sizeof(issue.string),fp)== NULL) break;
		printf("issue %s",issue.string);
		MPI_Send(issue.string,20,MPI_CHAR,1,42,MPI_COMM_WORLD);
		nanosleep(&clockspec,NULL);
	}
}

void cpu_decode(){
	MPI_Status Stat;
	instruction decode;
	char *extracted;
	while(1)
	{
		MPI_Recv(decode.string,20,MPI_CHAR,0,42,MPI_COMM_WORLD,&Stat);
		printf("decoding: %s",decode.string);
		
		extracted = strtok(decode.string," ");
		decode.type = extract_type(extracted);
		extracted = strtok(NULL," ");
		decode.dst = extract_register(extracted);
		extracted = strtok(NULL," ");
		if (decode.type == LDA || decode.type == STA)
		{
			decode.address = atoi(extracted);
		}
		else
		if (decode.type == ADD || decode.type == SUB)
		{
		decode.src1 = extract_register(extracted);
		extracted = strtok(NULL," ");
		decode.src2 = extract_register(extracted);
		}
		else
		if(decode.type == MVI)
		{
			decode.result = atoi(extracted);
		}
		nanosleep(&clockspec,NULL);
	}
}

void cpu_execute(){
	instruction execute;
	printf("execute\n");
	/*
	while(1){
		MPI_Send(issue.string,20,MPI_CHAR,1,42,MPI_COMM_WORLD);
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
	}*/
}

void cpu_data_access(){
	printf("data_access\n");
	/*
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
	printf("Execution Complete..CPU and Memory State:\n");
	printf("R0: %d\tR1: %d\tR2: %d\tR3: %d\n",R0,R1,R2,R3);
	printf("Memory:\n");
	for(int i=0;i<MEMSIZE;i++)
		printf("%d  ",MEMORY[i]);
	printf("\n");
	}*/
}

