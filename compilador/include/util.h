#ifndef	__UTIL_H_
#define	__UTIL_H_

#ifndef	STACK_SIZE
#define STACK_SIZE	1000
#endif

struct stack {
	unsigned int index;
	int elem[STACK_SIZE];
};

void stack_init(struct stack *stack);
void stack_push(struct stack *stack, int elem);
int stack_pop(struct stack *stack);

#endif	// __UTIL_H_
