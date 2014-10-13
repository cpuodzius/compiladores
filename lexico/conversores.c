/*
	A idéia desse programa é implementar todas as conversoes de dados necessarias pelo léxico.
	A entrada consiste sempre em um vetor de char terminado por \0 e a saída é o dado no tipo transformado

	É suposto que as strings de entradas sao válidas (ou seja, ex.: nao há chamadas para a funcao string_to_int com uma string que contenha letras)
*/

#include <stdio.h>

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
	for(i = 0; str[i] != '\0'; i++) {
		printf("%c ", str[i]);
		str[i - 1] = str[i];
	}
	str[i - 1] = '\0';
	printf("\n%s\n", str);
	return str;
}

int main() {
	printf("%d %d %d\n", string_to_int("7493"), string_to_int("8327923"), string_to_int("1588542"));
	printf("%f %f %f\n", string_to_real("74.000"), string_to_real("823.12340"), string_to_real("142.11"));
	printf("%s\n", string_to_text("\"Isso é um teste!\""));
	return 0;
}
