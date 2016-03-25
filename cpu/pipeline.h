//Operations
#define LDA 0
#define STA 1
#define ADD 2
#define SUB 3


//Registers
#define R0 0
#define R1 1
#define R2 2

void cpu_issue();
void cpu_decode();
void cpu_execute();
void cpu_memory_access();
void cpu_writeback();

typedef struct {
	char *string;
	int type;
	int src;
	int dst;
	int op1;
	int op2;
	int address;
	int result;
} instruction;


