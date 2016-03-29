#include "time.h"
#include "string.h"

int R0=0,R1=10,R2=0,R3=0;
int MEMORY[10];

#define PIPELINE_DEPTH 5

//Clock
#define CLK_HIGH 1
#define CLK_LOW 0
#define CLK_HALT 3

//Operations
#define LDA 0
#define STA 1
#define ADD 2
#define SUB 3

typedef struct {
	char string[20];
	int type;
	int * src1;
	int * src2;
	int * dst;
	int address;
	int result;
	int valid;
} instruction;

void cpu_clock();
void cpu_issue(FILE*,instruction*);
void cpu_decode(instruction*,instruction*);
void cpu_execute(instruction*, instruction*);
void cpu_memory_access(instruction*, instruction*);
void cpu_writeback(instruction*, instruction*);

struct timespec clockspec ={
.tv_sec = 0,
.tv_nsec = 100000000
};

int * extract_register(char* input)
{
	if (input == NULL) return NULL;
	if (strncasecmp(input, "r0", 2) == 0)
	{
		printf("extracted R0\n");
		return &R0;
	}
	if (strncasecmp(input, "r1", 2) == 0)
	{
		printf("extracted R1\n");
		return &R1;
	}
	if (strncasecmp(input, "r2", 2) == 0)
	{
		printf("extracted R2\n");
		return &R2;
	}
	if (strncasecmp(input, "r3", 2) == 0)
	{
		printf("extracted R3\n");
		return &R3;
	}
	return NULL;
}

int extract_type(char* input)
{
	if (strncasecmp(input, "lda", 3) == 0)
	{
		printf("lda was here\n");
		return LDA;
	}
	if (strncasecmp(input, "sta", 3) == 0)
	{
		printf("sta was here\n");
		return STA;
	}
	if (strncasecmp(input, "add", 3) == 0)
	{
		printf("add was here\n");
		return ADD;
	}
	if (strncasecmp(input, "sub", 3) == 0)
	{
		printf("sub was here\n");
		return SUB;
	}
	return -1;
}
