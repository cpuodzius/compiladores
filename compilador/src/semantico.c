#include "semantico.h"

#include "mvn.h"
#include <string.h>

#ifdef DEBUG
#include <assert.h>
#endif

#define TOKEN(t_tipo, t_valor) (token->tipo == t_tipo && token->valor == t_valor)
#define TOKEN_TIPO(t_tipo) (token->tipo == t_tipo)	

void get_symbol(struct symbol *symbol, struct token *token);
void set_symbol_name(struct *symbol, struct token *token);
void new_symbol_table(struct symbol_table *table, struct symbol_table *parent);
void insert_symbol(struct symbol_table *table, struct symbol *elem);

void semantico(char *filename, struct token tokens[MAX_TOKEN_NUM], unsigned int tokens_num, SINTATICO_MAQUINAS maquinas[MAX_MAQUINA_NUM], unsigned int maquinas_num) {
	unsigned int i, index = 0;
	struct symbol_table escopos, *current;
	new_symbol_table(escopos, NULL);
	for(i = 0; i < token_num; i++) {
		if(TOKEN(T_DELIMITADOR, CHAVES_ABRE)) {
			current = malloc(sizeof(struct symbol_table));
		}

	}
}

#define PROC_NAME_SIZE		32

char *get_proc_name() {
	static unsigned int cnt = 0;
	char *name = malloc(PROC_NAME_SIZE);
	sprintf(name, "proc_%d", cnt++);
	return name;
} 

void get_symbol(struct symbol *symbol, struct token *token) {
	switch(token->tipo) {
		case T_DELIMITADOR:
#ifdef DEBUG
			assert(symbol->valor != CHAVES_ABRE);
#endif
			symbol->name = get_proc_name();
			symbol->_class = C_PROC;
			symbol->st = st;
			break;
		case C_INT:	//FALLTHROUGH
		case C_REAL:	//FALLTHOUGH
		case C_TEXT:
			symbol->_class = C_VAR;
			symbol -> C_VAR;
			break;
		default:
			break;
	}
}

void set_symbol_name(struct *symbol, struct token *token) {
	if(TOKEN_TIPO(T_VAR)) {
		strcpy(symbol->name, var_table[token->valor]);
	}
}

void new_symbol_table(struct symbol_table *table, struct symbol_table *parent) {
	table->size = table->last = 0;
	table->parent = parent;
}

void insert_symbol(struct symbol_table *table, struct symbol *elem) {
	table->symbol[table->last] = *elem;
	table->last += 1;
	elem->desl = table->size;
	if(symbol->_class == C_VAR) {
		switch(symbol->tipo) {
			case T_INT:
				table->size += MVN_INT_SIZE;
				break;
			case T_REAL:
				table->size += MVN_FLOAT_SIZE;
				break;
			case T_TEXT:
				table->size += (strlen(text_table[elem->valor]) * MVN_BYTE_SIZE);
				break;
		}
	}
}


#ifdef SEMANTICO_SELFTEST

int main() {
	return 0;
}

#endif
