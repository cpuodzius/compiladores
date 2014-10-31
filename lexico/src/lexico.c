#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"
#include "state_machine.h"

int string_to_int(char *str);
float string_to_real(char *str);
char *string_to_text(char *str);

void print_token(struct token token);

#define MAX_TOKEN_NUM		1000

#define INPUT_RANGE		256
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

/**********************************************************/
void get_next_char(FILE *src, char *current, char *lookahead) {
	*current = *lookahead;
	if(*current == EOF) {
		lookahead = NULL;
		return;
	}
	else
		*lookahead = fgetc(src);
}

int lexic_next_state(int *state, char input, char lookahead, char buffer[32], int *buffer_idx) {
	*state = state_machine[*state][(int)input];

	if(*state != 0) {
		buffer[*buffer_idx] = input;
		*buffer_idx += 1;
	}

	buffer[*buffer_idx] = '\0';
	return (*state == 0 || state_machine[*state][(int)lookahead] == 0) ? 0 : 1;
}

struct token get_token(FILE *src, char *current, char *lookahead, int *linha, int *coluna) {
	struct token token;
	int state = 0;
	char buffer[128];
	int buffer_idx;

	//printf("\n\nEstados: ");
	while(state == 0) {
		buffer_idx = 0;
		do {
			//printf("%d ", state);
			get_next_char(src, current, lookahead);

			if(*current == '\n') {
				*coluna = 0;
				*linha =+ 1;
			}
			else
				*coluna += 1;

			if(*current == EOF)
				break;
		} while(lexic_next_state(&state, *current, *lookahead, buffer, &buffer_idx));
	}

	//printf("\nToken: %s\n", buffer);

	int index_reservado;
	int index_var;

	token.tipo = token_tipo[state];
	token.valor = token_valor[state];

	// Decidir tipo do token
	switch(token.tipo) {
		case T_NONE:
			break;
		case T_VAR:
			break;
		case T_COMENTARIO:
			break;
		case T_BOOLEAN:
			break;
		case T_COMPARACAO:
			break;
		case T_ATRIBUICAO:
			break;
		case T_DELIMITADOR:
			break;
		case T_OPERADOR:
			break;
		case T_INT:
			token.valor = int_table_index;
			int_table[int_table_index++][0] = string_to_int(buffer);
			break;
		case T_REAL:
			token.valor = real_table_index;
			real_table[real_table_index++][0] = string_to_real(buffer);
			break;
		case T_TEXT:
			token.valor = text_table_index;
			strcpy(text_table[text_table_index++], string_to_text(buffer));
			break;
		case T_RESERVADO:
			// verificar se é palavra reservada, caso contrário é variavel
			index_reservado = get_reservado(buffer);
			if(index_reservado >= 0) {
				token.valor = index_reservado;
			}
			else if(strcmp(buffer, "#") == 0) { // É um comentário
				state = 0;
				token.tipo = T_COMENTARIO;
			}
			else if(strcmp(buffer, "TRUE") == 0) {
				token.tipo = T_BOOLEAN;
				token.valor = 1;
			}
			else if(strcmp(buffer, "FALSE") == 0) {
				token.tipo = T_BOOLEAN;
				token.valor = 0;
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
			}
			break;
	}
	token.linha = *linha;
	token.coluna = *coluna;
	return token;
}

void lexico(char *filename, struct token tokens[MAX_TOKEN_NUM]) {
	FILE *src = fopen(filename, "r");

	if(src == NULL) {
	  perror("Error in opening file");
	  return;
	}

	int linha, coluna;
	linha = coluna = 0;

	char current[1], lookahead[1];
	*lookahead = fgetc(src);

	unsigned int i = 0;
	do {
		tokens[i] = get_token(src, current, lookahead, &linha, &coluna);
		print_token(tokens[i]);
	} while(!(tokens[i].tipo == T_RESERVADO && (strcmp(reservado[tokens[i].valor], "END") == 0)));
	fclose(src);
}

void sintatico(struct token tokens[]) {

}

void compile(char *filename) {
	struct token tokens[MAX_TOKEN_NUM];
	lexico(filename, tokens);

	sintatico(tokens);
}

int main() {
	compile("teste.cel");
}

int string_to_int(char *str) {
        int i, sign = 1, n = 0;
        for(i = 0; str[i] != '\0'; i++) {
		if(i == 0 && str[i] == '-') {
			sign = -1;
			continue;
		}
                n *= 10;
                n += str[i] - '0';
        }
        return sign * n;
}

float string_to_real(char *str) {
        int i, sign = 1, div = 1;
        float a = 0.0, b = 0.0;
        for(i = 0; str[i] != '.'; i++){
		if(i == 0 && str[i] == '-') {
			sign = -1;
			continue;
		}
                a *= 10;
                a += str[i] - '0';
        }
        for(i++; str[i] != '\0'; i++) {
                div *= 10;
                b += (str[i] - '0') / (float) div;
        }
        return sign * (a + b);
}

char* string_to_text(char *str) {
        int i;
        for(i = 1; str[i] != '"'; i++) {
                str[i - 1] = str[i];
        }
	str[i - 1] = '\0';
        return str;
}

int get_reservado(char *buffer) {
	int i = 0, index = -1;
	do {
		if(strcmp(buffer, reservado[i]) == 0) {
			index = i;
			break;
		}
		i++;
	} while(strcmp(reservado[i], "END") != 0);
	if(strcmp(buffer, "END") == 0)
		index = i;
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
	//printf("\ntoken: %d %d\n", token.tipo, token.valor);
	switch(token.tipo) {
		case T_NONE:
			return;
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
		case T_DELIMITADOR:
			switch(token.valor) {
				case PARENTESES_ABRE:
					printf("{delimitador, ( }");
					break;
				case PARENTESES_FECHA:
					printf("{delimitador, ) }");
					break;
				case COLCHETES_ABRE:
					printf("{delimitador, [ }");
					break;
				case COLCHETES_FECHA:
					printf("{delimitador, ] }");
					break;
				case CHAVES_ABRE:
					printf("{delimitador, { }");
					break;
				case CHAVES_FECHA:
					printf("{delimitador, } }");
					break;
			}
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
	printf("\n");
}

