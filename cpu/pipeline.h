#include "time.h"
#include "string.h"
#include "stdint.h"

#define MEMSIZE 10

uint32_t R0=0,R1=0,R2=0,R3=0;
uint32_t MEMORY[MEMSIZE]={0};

#define PIPELINE_DEPTH 4

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
void cpu_data_access(instruction*);

struct timespec clockspec ={
.tv_sec = 0,
.tv_nsec = 100000000
};

int * extract_register(char* input)
{
	if (input == NULL) return NULL;
	if (strncasecmp(input, "r0", 2) == 0)
		return &R0;
	if (strncasecmp(input, "r1", 2) == 0)
		return &R1;
	if (strncasecmp(input, "r2", 2) == 0)
		return &R2;
	if (strncasecmp(input, "r3", 2) == 0)
		return &R3;
	printf("ERR: error in register extraction\n");
	return NULL;
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
