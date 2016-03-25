#define PIPELINE_DEPTH 5

//Operations
#define LDA 0
#define STA 1
#define ADD 2
#define SUB 3


//Registers
#define R0 0
#define R1 1
#define R2 2


typedef struct {
	char string[20];
	int type;
	int src;
	int dst;
	int op1;
	int op2;
	int address;
	int result;
} instruction;

void cpu_clock();
void cpu_issue(FILE*,instruction*);
void cpu_decode(instruction*,instruction*);
void cpu_execute(instruction*, instruction*);
void cpu_memory_access(instruction*, instruction*);
void cpu_writeback(instruction*, instruction*);
