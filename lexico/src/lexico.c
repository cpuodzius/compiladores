#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state_machine.h"

int string_to_int(char *str);
float string_to_real(char *str);
char *string_to_text(char *str);

typedef enum tipo {
	T_INT,
	T_REAL,
	T_TEXT,
	T_BOOLEAN,
	T_RESERVADO,
	T_VAR,
	T_COMPARACAO,
	T_ATRIBUICAO,
	T_OPERADOR,
	T_COMENTARIO,
} TIPO;

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

int get_reservado(char *buffer);
int get_var(char *buffer);

typedef enum TIPO_COMPARACAO {
	MENOR,
	MENOR_OU_IGUAL,
	IGUAL,
	MAIOR_OU_IGUAL,
	MAIOR
} TIPO_COMPARACAO;

typedef enum TIPO_OPERADOR {
	SOMA,
	SUBTRACAO,
	DIVISAO,
	MULTIPLICACAO,
	INCREMENTO,
	DECREMENTO
} TIPO_OPERADOR;

char *reservado[] = {
	"BEGIN",
	"IF",
	"ELSE",
	"INPUT",
	"OUTPUT",
	"FOR",
	"OR",
	"AND",
	"XOR",
	"NOT",
	"END"
};

char terminal[] = {'\n',' ', '(', '[', '{', '}', ']', ')'}; // Só não é válido quando está na leitura de um text (estado 11) 


/**********************************************************/
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
		if(*state == 11) {
			if(strcmp(input, "\"") == 0) {
				not_terminal = 0;
			}
		}
		else {
			for(i = 0; i < sizeof(terminal); i++) {
				if(input == terminal[i]) {
					not_terminal = 0;
					break;
				}
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

	int index_reservado;
	int index_var;
	// Decidir tipo do token
	switch(state) {
		case 1:
			token.tipo = T_INT;
			token.valor = int_table_index;
			int_table[int_table_index++][0] = string_to_int(buffer);
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 2:
			token.tipo = T_REAL;
			token.valor = real_table_index;
			real_table[real_table_index++][0] = string_to_real(buffer);
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 3:
			// verificar se é palavra reservada, caso contrário é variavel
			index_reservado = get_reservado(buffer);
			if(index_reservado >= 0) {
				token.tipo = T_RESERVADO;
				token.valor = index_reservado;
				token.linha = *linha;
				token.coluna = *coluna;
			}
			else if(strcmp(buffer, "#") == 0) { // É um comentário
				token.tipo = T_COMENTARIO;
				token.linha = *linha;
				token.coluna = *coluna;
				break;
			}
			else if(strcmp(buffer, "TRUE") == 0) {
				token.tipo = T_BOOLEAN;
				token.valor = 1;
				token.linha = *linha;
				token.coluna = *coluna;
				break;
			}
			else if(strcmp(buffer, "FALSE") == 0) {
				token.tipo = T_BOOLEAN;
				token.valor = 0;
				token.linha = *linha;
				token.coluna = *coluna;
				break;
			}
			else {
				token.tipo = T_VAR;
				index_var = get_var(buffer);
				if(index_var >= 0)
					token.valor = index_var;
				else {
					token.valor = var_table_index;
					strcpy(var_table[var_table_index++], buffer);
				}
				token.linha = *linha;
				token.coluna = *coluna;
			}
			break;
		case 4:
			token.tipo = T_COMPARACAO;
			token.valor = MENOR;
			token.linha = *linha;
			token.coluna = *coluna;
		case 5:
			token.tipo = T_COMPARACAO;
			token.valor = IGUAL;
			token.linha = *linha;
			token.coluna = *coluna;
		case 6:
			token.tipo = T_COMPARACAO;
			token.valor = MAIOR;
			token.linha = *linha;
			token.coluna = *coluna;
		case 7:
			token.tipo = T_COMPARACAO;
			token.valor = MENOR_OU_IGUAL;
			token.linha = *linha;
			token.coluna = *coluna;
		case 8:
			token.tipo = T_RESERVADO;
			token.valor = MAIOR;
			token.linha = *linha;
			token.coluna = *coluna;
		case 9:
			token.tipo = T_COMPARACAO;
			token.valor = MAIOR_OU_IGUAL;
			token.linha = *linha;
			token.coluna = *coluna;
		case 12:
			token.tipo = T_TEXT;
			token.valor = text_table_index;
			strcpy(text_table[text_table_index++], string_to_text(buffer));
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 13:
			token.tipo = T_OPERADOR;
			token.valor = SOMA;
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 14:
			token.tipo = T_OPERADOR;
			token.valor = SUBTRACAO;
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 15:
			token.tipo = T_OPERADOR;
			token.valor = INCREMENTO;
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 16:
			token.tipo = T_OPERADOR;
			token.valor = DECREMENTO;
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 17:
			token.tipo = T_OPERADOR;
			token.valor = MULTIPLICACAO;
			token.linha = *linha;
			token.coluna = *coluna;
			break;
		case 18:
			token.tipo = T_OPERADOR;
			token.valor = DIVISAO;
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
		print_token(token);
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

int get_reservado(char *buffer) {
	int i = 0, index = -1;
	do {
		if(strcmp(buffer, reservado[i]) == 0) {
			index = i;
			break;
		}
	} while(strcmp(buffer, "END") != 0);
	return index;
}

int get_var(char *buffer) {
	int i, index = -1;
	for(i = 0; i < var_table_index; i++) {
		if(strcmp(buffer, var_table[i]) == 0) {
			index = i;
			break;
		}
	}
	return index;
}

void print_token(struct token token) {
	printf("token: %d %d\n", token.tipo, token.valor);
	switch(token.tipo) {
		case T_INT:
			printf("{int, %d}", int_table[token.valor][0]);
			break;
		case T_REAL:
			printf("{real, %f}", real_table[token.valor][0]);
			break;
		case T_TEXT:
			printf("{text, %s}", text_table[token.valor]);
			break;
		case T_BOOLEAN:
			if(token.valor)
				printf("{boolean, TRUE}");
			else
				printf("{boolean, FALSE}");
			break;
		case T_RESERVADO:
			printf("{reservado, %s}", reservado[token.valor]);
			break;
		case T_VAR:
			printf("{var, %s}", var_table[token.valor]);
			break;
		case T_COMPARACAO:
			switch(token.valor) {
				case MENOR:
					printf("{comparacao, <}");
					break;
				case MENOR_OU_IGUAL:
					printf("{comparacao, <=}");
					break;
				case IGUAL:
					printf("{comparacao, =}");
					break;
				case MAIOR_OU_IGUAL:
					printf("{comparacao, >=}");
					break;
				case MAIOR:
					printf("{comparacao, >}");
					break;
			}
			break;
		case T_ATRIBUICAO:
			printf("{atribuicao}");
			break;
		case T_OPERADOR:
			switch(token.valor) {
				case SOMA:
					printf("{operador, +}");
					break;
				case SUBTRACAO:
					printf("{operador, -}");
					break;
				case MULTIPLICACAO:
					printf("{operador, *}");
					break;
				case DIVISAO:
					printf("{operador, /}");
					break;
				case INCREMENTO:
					printf("{operador, ++}");
					break;
				case DECREMENTO:
					printf("{operador, --}");
					break;
			}
			break;
		case T_COMENTARIO:
			printf("{comentario}");
			break;
	}
}
