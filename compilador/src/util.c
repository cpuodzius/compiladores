#include "util.h"

void stack_init(struct stack *stack) {
	stack->index = 0;
}

void stack_push(struct stack *stack, int elem) {
	stack->elem[stack->index] = elem;
	stack->index += 1;
}

int stack_pop(struct stack *stack) {
	stack->index -= 1;
	return stack->elem[stack->index];
}

unsigned char stack_empty(struct stack *stack) {
	return (stack->index == 0);
}

int stack_top(struct stack *stack) {
	return (stack->index == 0) ? -1 : stack->elem[stack->index - 1];
}
