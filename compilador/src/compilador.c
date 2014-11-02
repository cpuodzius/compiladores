#include "lexico.h"
#include "sintatico.h"

void compile(char *filename) {
        struct token tokens[MAX_TOKEN_NUM];
        lexico(filename, tokens);

	sintatico(tokens);
}

int main() {
        compile("teste.cel");
}
