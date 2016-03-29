#include "time.h"
#include "string.h"
#include "stdint.h"

#define MEMSIZE 10
#define REGSIZE 4

uint32_t GPR[REGSIZE];
uint32_t MEMORY[MEMSIZE]={0};

#define PIPELINE_DEPTH 4

#define REG0 0
#define REG1 1
#define REG2 2
#define REG3 3

//Clock
#define CLK_HIGH 1
#define CLK_LOW 0
#define CLK_HALT 3

//Operations
#define LDA 0
#define STA 1
#define ADD 2
#define SUB 3
#define MVI 4

typedef struct {
	char string[20];
	int type;
	int src1;
	int src2;
	int dst;
	int address;
	uint32_t result;
	int valid;
} instruction;

void cpu_clock();
void cpu_issue(FILE*);
void cpu_decode();
void cpu_execute();
void cpu_data_access();

struct timespec clockspec ={
.tv_sec = 0,
.tv_nsec = 100000000
};

int extract_register(char* input)
{
	if (input == NULL) return -1;
	if (strncasecmp(input, "r0", 2) == 0)
		return REG0;
	if (strncasecmp(input, "r1", 2) == 0)
		return REG1;
	if (strncasecmp(input, "r2", 2) == 0)
		return REG2;
	if (strncasecmp(input, "r3", 2) == 0)
		return REG3;
	printf("ERR: error in register extraction\n");
	return -1;
}

int extract_type(char* input)
{
	if (strncasecmp(input, "lda", 3) == 0)
		return LDA;
	if (strncasecmp(input, "sta", 3) == 0)
		return STA;
	if (strncasecmp(input, "add", 3) == 0)
		return ADD;
	if (strncasecmp(input, "sub", 3) == 0)
		return SUB;
	if (strncasecmp(input, "mvi", 3) == 0)
		return MVI;
	printf("ERR: error in type extraction\n");
	return -1;
}
