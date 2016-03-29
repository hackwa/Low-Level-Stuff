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
		// This steps reads the register values received from last process
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
	MPI_Request Req;
	char *extracted;
	char issue[20];
	uint32_t decode[20]={0};
	while(1)
	{
	// Has to be nonblocking otherwise a deadlock will occur
		MPI_Irecv(GPR,REGSIZE,MPI_UINT32_T,3,42,MPI_COMM_WORLD,&Req);
	// Blocking for Synchronization
		MPI_Recv(issue,20,MPI_CHAR,0,42,MPI_COMM_WORLD,&Stat);
		printf("decoding: %s",issue);
		
		extracted = strtok(issue," ");
		decode[0] = extract_type(extracted);
		extracted = strtok(NULL," ");
		decode[1] = extract_register(extracted);
		extracted = strtok(NULL," ");
		if (decode[0] == LDA || decode[0] == STA || decode[0] == MVI)
		{
			decode[2] = atoi(extracted);
		}
		else
		if (decode[0] == ADD || decode[0] == SUB)
		{
		decode[2] = GPR[extract_register(extracted)];
		extracted = strtok(NULL," ");
		decode[3] = GPR[extract_register(extracted)];
		}
		MPI_Send(decode,20,MPI_UINT32_T,2,42,MPI_COMM_WORLD);
		nanosleep(&clockspec,NULL);
	}
}

void cpu_execute(){
	MPI_Status Stat;
	uint32_t execute[20]={0};
	printf("execute\n");
	while(1){
		MPI_Recv(execute,20,MPI_UINT32_T,1,42,MPI_COMM_WORLD,&Stat);
		switch(execute[0])
		{
			case LDA: 	printf("executing: LDA\n");
					execute[4] = execute[2];break;
			case STA: 	printf("executing: STA\n");
					execute[4] = execute[2];break;
			case MVI:	printf("executing: MVI\n");
					execute[4] = execute[2];break;
			case ADD: 	printf("executing: ADD\n");
					printf("Adding %d and %d\n",execute[2],execute[3]);
					execute[4] = execute[2] + execute[3];
					break;
			case SUB: 	printf("executing SUB\n");
					printf("Subtracting %d from %d\n",execute[2],execute[3]);
					execute[4] = execute[2] - execute[3];
					break;
			default:	printf("ERR: Error in execution");
		}
		MPI_Send(execute,20,MPI_UINT32_T,3,42,MPI_COMM_WORLD);
		nanosleep(&clockspec,NULL);
		}
}

void cpu_data_access(){
	MPI_Request Req;
	printf("data_access\n");
	uint32_t data_access[20] = {0};
	while(1){
		MPI_Status Stat;
		MPI_Recv(data_access,20,MPI_UINT32_T,2,42,MPI_COMM_WORLD,&Stat);
		switch(data_access[0])
		{
			case LDA:	GPR[data_access[1]] = MEMORY[data_access[4]];
					break;
			case STA:	MEMORY[data_access[4]] = GPR[data_access[1]];
					break;
			case MVI:	GPR[data_access[1]] = data_access[4];
					break;
			case ADD:	GPR[data_access[1]] = data_access[4];
					break;
			case SUB:	GPR[data_access[1]] = data_access[4];
					break;
		}
		MPI_Isend(GPR,REGSIZE,MPI_UINT32_T,1,42,MPI_COMM_WORLD,&Req);
		nanosleep(&clockspec,NULL);
		printf("Registers: \n");
		for(int i=0;i<REGSIZE;i++)
			printf("%d ",GPR[i]);
		printf("\n");
		printf("Memory: \n");
		for(int i=0;i<MEMSIZE;i++)
			printf("%d ",MEMORY[i]);
		printf("\n");
	}
}
