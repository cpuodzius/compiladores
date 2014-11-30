
#define SYMBOL_MAX_LENGHT	32
#define SYMBOL_MAX_NUM		100

typedef enum _class {
        C_PROC,
	C_VAR
} CLASS;

struct symbol;
struct symbol_table;

struct symbol {
	unsigned int desl;
	char name[SYMBOL_MAX_LENGHT];
	CLASS _class;
	TIPO tipo;
	struct symbol_table st;
}

struct symbol_table {
	unsigned int size, last;
	struct symbol[SYMBOL_MAX_NUM];
	struct symbol_table *parent;
};
