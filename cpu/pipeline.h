void cpu_issue();
void cpu_decode();
void cpu_execute();
void cpu_memory_access();
void cpu_writeback();

typedef struct {
	char *string;
	int type;
	char *src;
	char *dst;
	int op1;
	int op2;
	int address;
	int result;
} instruction;


