void issue();
void decode();
void execute();
void memory_access();
void writeback();

typedef struct {
	int type;
	int op1;
	int op2;
	int address;
} instruction;
