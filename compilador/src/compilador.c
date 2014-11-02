#include "lexico.h"
#include "sintatico.h"

void compile(char *filename) {
        struct token tokens[MAX_TOKEN_NUM];
	SINTATICO_MAQUINAS maquinas[MAX_MAQUINA_NUM];
        lexico(filename, tokens);
	sintatico(tokens, maquinas);
}

int main() {
        compile("teste.cel");
}
