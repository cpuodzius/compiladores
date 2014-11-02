#ifndef __SINTATICO_H
#define __SINTATICO_H

#include <stdio.h>
#include <string.h>
#include "lexico.h"

#define TOKEN(t_tipo, t_valor) (tokens[*index].tipo == t_tipo && tokens[*index] == t_valor)
#define TOKEN_RESERVADO(t_reservado) (tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "t_reservado") == 0))
#define ROLLBACK_ELSE	else { 				\
				*index = rollback;	\
				return 0;		\
			}				

unsigned char programa(struct token tokens[], int *index);

void sintatico(struct token tokens[]) {
	int index;
	programa(tokens, &index);
}

unsigned char programa(struct token tokens[], int *index) {
	int rollback = *index;
	if(TOKEN_RESERVADO(BEGIN)) {
		*index += 1;
		do {
			print_token(tokens[*index]);
			*index += 1;
		} while(!TOKEN_RESERVADO(END));
		print_token(tokens[*index]);
		*index += 1;
	}
	ROLLBACK_ELSE
	printf("TODO: programa");
	return 1;
}
/*
unsigned char programa(struct token tokens[], int *index) {
	int rollback = *index;
	if(tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "BEGIN") == 0)) {
		*index += 1;
		sequencia_de_comandos(tokens, index);
		if(tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "END") == 0))
			*index += 1;
		else {
			*index = rollback;
			return 0;
		}
	}
	else {
		*index = rollback;
		return 0;
	}
	printf("TODO: programa");
	return 1;
}

unsigned char sequencia_de_comandos(struct token tokens[], int *index) {
	unsigned char is_comando;
	do {
		is_comando = comando(tokens, index);
	} while(is_comando);
	printf("TODO: sequencia_de_comandos");
	return 1;
}

unsigned char comando(struct token tokens[], int *index) {
	if(!atribuicao(tokens, index))
		if(!atribuicao(tokens, index))
			if(!leitura(tokens, index))
				if(!impressao(tokens, index))
					if(!decisao(tokens, index))
						if(!laco(tokens, index))
							return 0;
	printf("TODO: comando");
	return 1;
}

unsigned char atribuicao(struct token tokens[], int *index) {
	int rollback = *index;
	expressao(tokens, index);
	if(tokens[*index].tipo != T_ATRIBUICAO)
		*index += 1;
	else {
		*index = rollback;
		return 0;
	}
	if(tokens[*index].tipo = T_VAR)
		*index += 1;
	else {
		*index = rollback;
		return 0;
	}
	printf("TODO: atribuicao");
	return 1;
}

unsigned char expressao(struct token tokens[], int *index) {
	if(!exp_aritmetica(token, index))
		if(!exp_logica(token, index))
			return 0;
	printf("TODO: expressao");
	return 1;
}

unsigned char exp_aritimetica(struct token tokens[], int *index) {
	int rollback = *index;
	unsigned char is_exp_aritimetica = 0;
	if(tokens[*index].tipo = T_VAR) {
		*index += 1;
		is_exp_aritimetica = 1;
		if(tokens[*index].tipo == T_OPERADOR && (tokens[*index].valor == INCREMENTO || tokens[*index].valor == DECREMENTO))
			*index += 1;
		else {
			do {
				rollback = *index;
				if(tokens[*index].tipo != T_OPERADOR || (tokens[*index].valor != SOMA || tokens[*index].valor != SUBTRACAO || tokens[*index].valor != MULTIPLICAO || tokens[*index].valor != DIVISAO))
					break;
				*index += 1;
			} while(termo(tokens, index));
			*index = rollback;
		}
	}
	else {
		*index = rollback;
		if(termo(tokens[*index], index)) {
			do {
				rollback = *index;
				if(tokens[*index].tipo != T_OPERADOR || (tokens[*index].valor != SOMA || tokens[*index].valor != SUBTRACAO || tokens[*index].valor != MULTIPLICAO || tokens[*index].valor != DIVISAO))
					break;
				*index += 1;
			} while(termo(tokens, index));
			*index = rollback;
		}
	}
	if(is_exp_aritmetica)
		printf("TODO: exp_aritmetica");
	else
		*index = rollback;
	return is_exp_aritmetica
}

unsigned char exp_logica(struct token tokens[], int *index) {
	rollback = *index;
	if(tokens[*index].tipo = T_OPERADOR && tokens[*index].valor == NOT) {
		*index += 1;
		if(tokens[*index].tipo = T_VAR)
			*index += 1;
		else {
			*index = rollback;
			return 0;
		}
	}
	else if(tokens[*index].tipo = T_VAR || tokens[*index].tipo = T_INT)
		*index += 1;
	else
		return 0;
	do {
		rollback = *index;
		if(tokens[*index].tipo = T_OPERADOR && (tokens[*index].valor = AND || tokens[*index].valor = OR))
			*index += 1;
		else
			break;
		if(tokens[*index].tipo = T_OPERADOR && tokens[*index].valor = NOT) {
			*index += 1;
			if(tokens[*index].tipo = T_VAR)
				*index += 1;
			else
				break;
		}
		else if(tokens[*index].tipo = T_VAR || tokens[*index].tipo = T_INT)
				*index += 1;
		else
			break;
	} while(1);
	*index = rollback;
	printf("TODO: exp_logica");
	return 1;
}

unsigned char termo(struct token tokens[], int *index) {
	if(tokens[*index].tipo = T_VAR || tokens[*index].tipo = T_INT || tokens[*index].tipo = T_REAL)
		*index += 1
	else
		return 0;
	printf("TODO: termo");
	return 1;
}

unsigned char leitura(struct token tokens[], int *index) {
	int rollback = *index;
	if(tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "INPUT") == 0)) {
		*index += 1;
		if(tokens[*index].tipo == T_VAR) {
			*index += 1;
			while(tokens[*index].tipo == T_VAR)
				*index += 1;
		}
		else {
			*index = rollback;
			return 0;
		}
	}
	else
		return 0;
	printf("TODO: leitura");
	return 1;
}

unsigned char impressao(struct token tokens[], int *index) {
	int rollback = *index;
	if(tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "OUTPUT") == 0)) {
		*index += 1;
		if(tokens[*index].tipo == T_TEXT || tokens[*index].tipo == T_VAR) {
			*index += 1;
			while(tokens[*index].tipo == T_TEXT || tokens[*index].tipo == T_VAR)
				*index += 1;
		}
		else {
			*index = rollback;
			return 0;
		}
	}
	else
		return 0;
	printf("TODO: impressao");
	return 1;

}

unsigned char decisao(struct token tokens[], int *index) {
	int rollback = *index;
	if(tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "IF") == 0)) {
		*index += 1;
		if(tokens[*index].tipo == T_DELIMITADOR && (tokens[*index].valor = PARENTESES_ABRE)) {
			*index += 1;
			if(!exp_logica(token, index))
				if(!comparacao(token, index)) {
					*index = rollback;
					return 0;
				}
			if(tokens[*index].tipo == T_DELIMITADOR && (tokens[*index].valor = PARENTESES_FECHA)) {
				*index += 1;
				if(tokens[*index].tipo == T_DELIMITADOR && (tokens[*index].valor = CHAVES_ABRE)) {
					*index += 1;
					if(!sequencia_de_comandos(tokens, index)) {
						*index = rollback;
						return 0;
					}
					if(tokens[*index].tipo == T_DELIMITADOR && (tokens[*index].valor = CHAVES_FECHA)) {
						*index += 1;
						if(tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "ELSE") == 0)) {
							*index += 1;
							if(tokens[*index].tipo == T_DELIMITADOR && (tokens[*index].valor = CHAVES_ABRE)) {
								*index += 1;
								if(!sequencia_de_comandos(tokens, index)) {
									*index = rollback;
									return 0;
								}
								if(tokens[*index].tipo == T_DELIMITADOR && (tokens[*index].valor = CHAVES_FECHA)) {
									*index += 1;
								}
								else {
									*index = rollback;
									return 0;
								}
							}
							else {
								*index = rollback;
								return 0;
							}
						}
						else {
							*index = rollback;
							return 0;
						}
					}
					else {
						*index = rollback;
						return 0;
					}
				}
			}
			else {
				*index = rollback;
				return 0;
			}
		}
		else {
			*index = rollback;
			return 0;
		}
	}
	else
		return 0;
	printf("TODO: decisao");
	return 1;
}

unsigned char comparacao(struct token tokens[], int *index) {
	int rollback = *index;
	if(!termo(token, index))
		return 0;
	if(tokens[*index].tipo == T_COMPARACAO)
		*index += 1
	else
		return 0;
	if(!termo(token, index)) {
		*index = rollback;
		return 0;
	}
	printf("TODO: comparacao");
	return 1;
}

unsigned char laco(struct token tokens[], int *index) {
	int rollback = *index;
	if(tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], "FOR") == 0)) {
		*index += 1;
		if(tokens[*index].tipo == T_DELIMITADOR && tokens[*index].valor == PARENTESES_ABRE) {
			*index += 1;
			if(comparacao(tokens, index) || exp_logica(token, index)) {
				if(tokens[*index].tipo == T_DELIMITADOR && tokens[*index].valor == PARENTESES_FECHA) {
					*index += 1;
					if(tokens[*index].tipo == T_DELIMITADOR && tokens[*index].valor == CHAVES_ABRE) {
						*index += 1;
						if(sequencia_de_comandos(tokens, index)) {
							if(tokens[*index].tipo == T_DELIMITADOR && tokens[*index].valor == CHAVES_FECHA) {
								*index += 1;
							}
							else {
								*index = rollback;
								return 0;
							}
						}
						else {
							*index = rollback;
							return 0;
						}
					}
					else {
						*index = rollback;
						return 0;
					}
				}
				else {
					*index = rollback;
					return 0;
				}
			}
			else {
				*index = rollback;
				return 0;
			}
		}
		else {
			*index = rollback;
			return 0;
		}
	}
	else
		return 0;
	printf("TODO: laco");
	return 1;
}
*/

#if SINTATICO_SELFTEST

int main() {
}

#endif
#endif // __SINTATICO_H
