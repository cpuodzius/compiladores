#ifndef __LEXICO_H_
#define __LEXICO_H_

#define MAX_TOKEN_NUM           1000

typedef enum tipo {
        T_INT,
        T_REAL,
        T_TEXT,
        T_RESERVADO,
        T_COMENTARIO,
        T_BOOLEAN,
        T_VAR,
        T_COMPARACAO,
        T_ATRIBUICAO,
        T_DELIMITADOR,
        T_OPERADOR,
        T_NONE
} TIPO;

struct token {
        TIPO tipo;
        int valor;
        int linha, coluna;
};

typedef enum TIPO_DELIMITADOR {
        PARENTESES_ABRE,
        PARENTESES_FECHA,
        CHAVES_ABRE,
        CHAVES_FECHA,
        COLCHETES_ABRE,
        COLCHETES_FECHA,
} TIPO_DELIMITADOR;

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
        DECREMENTO,
	NOT,
	AND,
	OR
} TIPO_OPERADOR;

#ifdef PALAVRAS_RESERVADAS
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
#else
extern char *reservado[];
#endif	// PALAVRAS_RESEVADAS

#define SYMBOL_TABLE_SIZE       100
#define MAX_VAR_SIZE            32
#define MAX_TEXT_SIZE           128

char var_table[SYMBOL_TABLE_SIZE][MAX_VAR_SIZE];
int int_table[SYMBOL_TABLE_SIZE][MAX_VAR_SIZE];
float real_table[SYMBOL_TABLE_SIZE][MAX_VAR_SIZE];
char text_table[SYMBOL_TABLE_SIZE][MAX_TEXT_SIZE];

void lexico(char *filename, struct token tokens[MAX_TOKEN_NUM]);
void print_token(struct token token);

#endif // __LEXICO_H_
