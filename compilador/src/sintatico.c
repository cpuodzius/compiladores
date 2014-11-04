#include <stdio.h>
#include <string.h>
#include "util.h"

#include "sintatico.h"

#define TOKEN(t_tipo, t_valor) (tokens[*index].tipo == t_tipo && tokens[*index].valor == t_valor)
#define TOKEN_TIPO(t_tipo) (tokens[*index].tipo == t_tipo)
#define TOKEN_RESERVADO(t_reservado) (tokens[*index].tipo == T_RESERVADO && (strcmp(reservado[tokens[*index].valor], t_reservado) == 0))
#define MACHINE(maquina)	maquina(tokens, index, stack, last)

#ifdef	DEBUG
#define NEXT_TOKEN()				\
	print_machine(stack_top(stack));	\
	print_token(*last);			\
	*index += 1;				\
	*last = tokens[*index];
#else
#define NEXT_TOKEN()		\
	*index += 1;		\
	*last = tokens[*index];
#endif

#define PUSH_MACHINE(machine)			\
	unsigned char accepted = 0;		\
	int rollback = *index;			\
	stack_push(stack, machine);

#define ROLLBACK_MACHINE()	\
	stack_pop(stack);	\
	*index = rollback;

#define RETURN()			\
	if(!accepted) {			\
		ROLLBACK_MACHINE();	\
	}				\
	return accepted

unsigned char programa(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char sequencia_de_comandos(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char comando(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char atribuicao(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char expressao(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char exp_aritimetica(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char exp_logica(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char termo(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char leitura(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char impressao(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char decisao(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char comparacao(struct token tokens[], int *index, struct stack *stack, struct token *last);
unsigned char laco(struct token tokens[], int *index, struct stack *stack, struct token *last);

unsigned char sintatico(struct token tokens[], SINTATICO_MAQUINAS maquinas[MAX_TOKEN_NUM]) {
	int index = 0, i = 0;
	unsigned char accepted = 0;
	struct stack stack;
	struct token last_token;
	stack_init(&stack);
	accepted = programa(tokens, &index, &stack, &last_token);
	if(accepted) {
		for(i = 0; i < stack.index; i++) {
			maquinas[i] = stack.elem[i];
			print_machine(maquinas[i]);
		}
	}
	else {
		printf("erro sintatico :: linha %d, coluna %d :: token : ", last_token.linha, last_token.coluna);
		print_token(last_token);
	}
	return accepted;
}

unsigned char programa(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_PROGRAMA);
	if(TOKEN_RESERVADO("BEGIN")) {
		NEXT_TOKEN();
		if(MACHINE(sequencia_de_comandos)) {
			if(TOKEN_RESERVADO("END")) {
				NEXT_TOKEN();
				accepted = 1;
			}
		}
	}
	RETURN();
}

unsigned char sequencia_de_comandos(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_SEQUENCIA_DE_COMANDOS);
	while(MACHINE(comando)) {}
	accepted = 1;
	RETURN();
}

unsigned char comando(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_COMANDO);
	if(MACHINE(atribuicao) || MACHINE(leitura) || MACHINE(impressao) || MACHINE(decisao) || MACHINE(laco))
		accepted = 1;
	RETURN();
}

unsigned char atribuicao(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_ATRIBUICAO);
	if(MACHINE(expressao)) {
		if(TOKEN_TIPO(T_ATRIBUICAO)) {
			NEXT_TOKEN();
			if(TOKEN_TIPO(T_VAR)) {
				NEXT_TOKEN();
				accepted = 1;
			}
		}
	}
	RETURN();
}

unsigned char expressao(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_EXPRESSAO);
	if(MACHINE(exp_aritimetica) || MACHINE(exp_logica))
		accepted = 1;
	RETURN();
}

unsigned char exp_aritimetica(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_EXP_ARITIMETICA);
	if(TOKEN_TIPO(T_VAR)) {
		NEXT_TOKEN();
		if(TOKEN(T_OPERADOR, INCREMENTO) || TOKEN(T_OPERADOR, DECREMENTO)) {
			NEXT_TOKEN();
			accepted = 1;
		}
		else
			*index = rollback;
	}
	if(!accepted) {
		if(MACHINE(termo)) {
			accepted = 1;
			do {
				rollback = *index;
				if(TOKEN(T_OPERADOR, SOMA) || TOKEN(T_OPERADOR, SUBTRACAO) || TOKEN(T_OPERADOR, MULTIPLICACAO) || TOKEN(T_OPERADOR, DIVISAO)) {
					NEXT_TOKEN();
				}
				else
					break;
			} while(MACHINE(termo));
			*index = rollback;
		}
	}
	RETURN();
}

unsigned char exp_logica(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_EXP_LOGICA);
	if(TOKEN(T_OPERADOR, NOT)) {
		NEXT_TOKEN();
		if(TOKEN_TIPO(T_VAR)) {
			NEXT_TOKEN();
			accepted = 1;
		}
	}
	else if(TOKEN_TIPO(T_VAR) || TOKEN_TIPO(T_INT)) {
		NEXT_TOKEN();
		accepted = 1;
	}
	if(accepted) {
		do {
			rollback = *index;
			if(TOKEN(T_OPERADOR, AND) || TOKEN(T_OPERADOR, OR)) {
				NEXT_TOKEN();
				if(TOKEN(T_OPERADOR, NOT)) {
					NEXT_TOKEN();
					if(TOKEN_TIPO(T_VAR)) {
						NEXT_TOKEN();
					}
					else
						break;
				}
				else if(TOKEN_TIPO(T_VAR) || TOKEN_TIPO(T_INT)) {
					NEXT_TOKEN();
				}
				else
					break;
			}
			else
				break;
		} while(1);
		*index = rollback;
	}
	RETURN();
}

unsigned char termo(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_TERMO);
	if(TOKEN_TIPO(T_VAR) || TOKEN_TIPO(T_INT) || TOKEN_TIPO(T_REAL)) {
		NEXT_TOKEN();
		accepted = 1;
	}
	RETURN();
}

unsigned char leitura(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_LEITURA);
	if(TOKEN_RESERVADO("INPUT")) {
		NEXT_TOKEN();
		if(TOKEN_TIPO(T_VAR)) {
			NEXT_TOKEN();
			accepted = 1;
			while(TOKEN_TIPO(T_VAR)) {
				NEXT_TOKEN();
			}
		}
	}
	RETURN();
}

unsigned char impressao(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_IMPRESSAO);
	if(TOKEN_RESERVADO("OUTPUT")) {
		NEXT_TOKEN();
		if(TOKEN_TIPO(T_TEXT) || TOKEN_TIPO(T_VAR)) {
			NEXT_TOKEN();
			accepted = 1;
			while(TOKEN_TIPO(T_TEXT) || TOKEN_TIPO(T_VAR)) {
				NEXT_TOKEN();
			}
		}
	}
	RETURN();
}

unsigned char decisao(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_DECISAO);
	if(TOKEN_RESERVADO("IF")) {
		NEXT_TOKEN();
		if(TOKEN(T_DELIMITADOR, PARENTESES_ABRE)) {
			NEXT_TOKEN();
			if(MACHINE(exp_logica) || MACHINE(comparacao)) {
				if(TOKEN(T_DELIMITADOR, PARENTESES_FECHA)) {
					NEXT_TOKEN();
					if(TOKEN(T_DELIMITADOR, CHAVES_ABRE)) {
						NEXT_TOKEN();
						if(MACHINE(sequencia_de_comandos)) {
							if(TOKEN(T_DELIMITADOR, CHAVES_FECHA)) {
								NEXT_TOKEN();
								if(TOKEN_RESERVADO("ELSE")) {
									NEXT_TOKEN();
									if(TOKEN(T_DELIMITADOR, CHAVES_ABRE)) {
										NEXT_TOKEN();
										if(MACHINE(sequencia_de_comandos)) {
											if(TOKEN(T_DELIMITADOR, CHAVES_FECHA)) {
												NEXT_TOKEN();
												accepted = 1;
											}
										}
									}
								}
								else
									accepted = 1;
							}
						}
					}
				}
			}
		}
	}
	RETURN();
}

unsigned char comparacao(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_COMPARACAO);
	if(MACHINE(termo)) {
		if(TOKEN_TIPO(T_COMPARACAO)) {
			NEXT_TOKEN();
			if(MACHINE(termo)) {
				accepted = 1;
			}
		}
	}
	RETURN();
}

unsigned char laco(struct token tokens[], int *index, struct stack *stack, struct token *last) {
	PUSH_MACHINE(SINTATICO_LACO);
	if(TOKEN_RESERVADO("FOR")) {
		NEXT_TOKEN();
		if(TOKEN(T_DELIMITADOR, PARENTESES_ABRE)) {
			NEXT_TOKEN();
			if(MACHINE(comparacao) || MACHINE(exp_logica)) {
				if(TOKEN(T_DELIMITADOR, PARENTESES_FECHA)) {
					NEXT_TOKEN();
					if(TOKEN(T_DELIMITADOR, CHAVES_ABRE)) {
						NEXT_TOKEN();
						if(MACHINE(sequencia_de_comandos)) {
							if(TOKEN(T_DELIMITADOR, CHAVES_FECHA)) {
								NEXT_TOKEN();
								accepted = 1;
							}
						}
					}
				}
			}
		}
	}
	RETURN();
}

void print_machine(SINTATICO_MAQUINAS maquina) {
	switch(maquina) {
        	case SINTATICO_PROGRAMA:
			printf("Maquina: programa\n");
			break;
        	case SINTATICO_SEQUENCIA_DE_COMANDOS:
			printf("Maquina: sequencia_de_comandos\n");
			break;
        	case SINTATICO_COMANDO:
			printf("Maquina: comando\n");
			break;
        	case SINTATICO_ATRIBUICAO:
			printf("Maquina: atribuicao\n");
			break;
        	case SINTATICO_EXPRESSAO:
			printf("Maquina: expressao\n");
			break;
        	case SINTATICO_EXP_ARITIMETICA:
			printf("Maquina: exp_aritmetica\n");
			break;
        	case SINTATICO_EXP_LOGICA:
			printf("Maquina: exp_logica\n");
			break;
        	case SINTATICO_TERMO:
			printf("Maquina: termo\n");
			break;
        	case SINTATICO_LEITURA:
			printf("Maquina: leitura\n");
			break;
        	case SINTATICO_IMPRESSAO:
			printf("Maquina: impressao\n");
			break;
        	case SINTATICO_DECISAO:
			printf("Maquina: decisao\n");
			break;
        	case SINTATICO_COMPARACAO:
			printf("Maquina: comparacao\n");
			break;
        	case SINTATICO_LACO:
			printf("Maquina: laco\n");
			break;
	}
}
