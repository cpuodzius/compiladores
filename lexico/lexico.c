#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state_machine.h"

int string_to_int(char *str);
float string_to_real(char *str);
char *string_to_text(char *str);

struct token {
	TIPO tipo;
	int valor;
	int linha, coluna;
};

void print_token(struct token token);

#define SYMBOL_TABLE_SIZE	100
#define MAX_VAR_SIZE		32
#define MAX_TEXT_SIZE		128

char var_table[SYMBOL_TABLE_SIZE][MAX_VAR_SIZE];
int int_table[SYMBOL_TABLE_SIZE][MAX_VAR_SIZE];
float real_table[SYMBOL_TABLE_SIZE][MAX_VAR_SIZE];
char text_table[SYMBOL_TABLE_SIZE][MAX_TEXT_SIZE];

unsigned int var_table_index = 0;
unsigned int int_table_index = 0;
unsigned int real_table_index = 0;
unsigned int text_table_index = 0;

typedef enum tipo {
	TIPO_var,
	TIPO_reservado,
	TIPO_int,
	TIPO_real,
	TIPO_text,
	TIPO_boolean,
} TIPO;

typedef enum reservado {
	COMPARACAO,
	ATRIBUICAO,
	BEGIN,
	END
} RESERVADO;

typedef enum TIPO_COMPARACAO {
	MENOR,
	MENOR_OU_IGUAL,
	IGUAL,
	MAIOR_OU_IGUAL,
	MAIOR
} TIPO_COMPARACAO;

char terminal[] = {'\n',' ', '(', '[', '{', '}', ']', ')'};

void get_next_char(FILE *src, char *current, char *lookahead) {
	*current = *lookahead;
	if(feof(src)) {
		lookahead = NULL;
		return;
	}
	*lookahead = fgetc(src);
}

int next_state(int *state, char input, char buffer[32], int *buffer_idx) {
	buffer[*buffer_idx] = input;
	int i, not_terminal = 1;
	if(input < ' ') {	// caracter de controle
		not_terminal = 0;
	}
	else {
		for(i = 0; i < sizeof(terminal); i++) {
			if(input == terminal[i]) {
				not_terminal = 0;
				break;
			}
		}
	}
	if(not_terminal)
		*buffer_idx += 1;
	buffer[*buffer_idx] = '\0';
	return not_terminal;
}

struct token get_token(FILE *src, char *current, char *lookahead, int *linha, int *coluna) {
	struct token token;
	int state = 0;
	char buffer[128];
	int buffer_idx = 0;

	do {
		get_next_char(src, current, lookahead);
		*coluna += 1;
		if(*current == EOF)
			break;
	} while(next_state(&state, *current, buffer, &buffer_idx));

	if(*current == '\n') {
		*coluna = 0;
		*linha =+ 1;
	}

	// Decidir tipo do token
	switch(state) {
		case 1:
			token.tipo = TIPO_int;
			token.valor = int_table_index;
			int_table[int_table_index++][0] = string_to_int(buffer);
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 2:
			token.tipo = TIPO_real;
			token.valor = real_table_index;
			real_table[real_table_index++][0] = string_to_real(buffer);
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 3:
			token.tipo = TIPO_text;
			token.valor = text_table_index;
			strcpy(text_table[text_table_index++], string_to_text(buffer));
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 4:
			token.tipo = COMPARACAO;
			token.valor = MENOR;
			token.linha = *linha;
			token.coluna = *coluna;
		case 5:
			token.tipo = COMPARACAO;
			token.valor = MENOR_OU_IGUAL;
			token.linha = *linha;
			token.coluna = *coluna;
		case 6:
			token.tipo = COMPARACAO;
			token.valor = IGUAL;
			token.linha = *linha;
			token.coluna = *coluna;
		case 7:
			token.tipo = COMPARACAO;
			token.valor = MAIOR_OU_IGUAL;
			token.linha = *linha;
			token.coluna = *coluna;
		case 8:
			token.tipo = COMPARACAO;
			token.valor = MAIOR;
			token.linha = *linha;
			token.coluna = *coluna;
		case 9:
			token.tipo = TIPO_var;
			token.valor = var_table_index;
			strcpy(var_table[var_table_index++], buffer);
			token.linha = *linha;
			token.coluna = *coluna;
			break;
	}

	return token;
}

void lexico(char *filename) {
	FILE *src = fopen(filename, "r");

	if(src == NULL) {
	  perror("Error in opening file");
	  return;
	}

	int linha, coluna;
	linha = coluna = 0;

	char current[1], lookahead[1];
	*lookahead = fgetc(src);

	int state = 0;
	struct token token;
	do {
		token = get_token(src, current, lookahead, &linha, &coluna);
	} while(current[0] != EOF);
	fclose(src);
}

void compile(char *filename) {
	lexico(filename);
}

int main() {
	compile("teste.cel");
}

int string_to_int(char *str) {
        int i, n = 0;
        for(i = 0; str[i] != '\0'; i++) {
                n *= 10;
                n += str[i] - '0';
        }
        return n;
}

float string_to_real(char *str) {
        int i, div = 1;
        float a = 0.0, b = 0.0;
        for(i = 0; str[i] != '.'; i++){
                a *= 10;
                a += str[i] - '0';
        }
        for(i++; str[i] != '\0'; i++) {
                div *= 10;
                b += (str[i] - '0') / (float) div;
        }
        return (a + b);
}

char* string_to_text(char *str) {
        int i;
        for(i = 0; str[i + 1] != '\0'; i++) {
                str[i] = str[i + 1];
                str[i + 1] = '\0';
        }
        return str;
}

void print_token(struct token token) {
	switch(token.tipo) {
		case TIPO_var:
			break;
		case TIPO_int:
			break;
		case TIPO_real:
			break;
		case TIPO_text:
			break;
		case TIPO_boolean:
			break;
	}
TIPO_var,
        TIPO_reservado,
        TIPO_int,
        TIPO_real,
        TIPO_text,
        TIPO_boolean,	
}
