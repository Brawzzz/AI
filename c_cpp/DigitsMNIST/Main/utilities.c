//-------------------------- INCLUDES --------------------------//
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>

//-------------------------- FUNCTION DEFINITIONS --------------------------//
void clear(int nmb_rows) {

	for (int i = 0; i < nmb_rows; i++) {
		printf("\033[F");
	}
	for (int j = 0; j < nmb_rows; j++) {
		printf("\033[K\r");
	}

	return;
}

void progression_bar(int state, int end) {

	if (state > end || state < 0 || end < 0) {
		return;
	}

	printf("|");
	fflush(stdout);
	for (int i = 0; i < state; i++){
		printf("*");
		fflush(stdout);
	}
	for (int j = state; j < end; j++) {
		printf("-");
		fflush(stdout);
	}
	printf("|");
	fflush(stdout);

	return;
}

void error(char* error_msg, int error_number){
	fprintf_s(stderr, error_msg);
	exit(error_number);
}

PChain new_chain(void* memory_adress) {

	PChain p_chain = (PChain)malloc(sizeof(Chain));
	if (p_chain == NULL) {
		error("\n*** allocation error (function : new_chain) ***\n", ALLOCATION_ERROR);
		exit(INT_MIN);
	}

	p_chain->memory_adress = memory_adress;
	p_chain->next = NULL;

	return p_chain;
}

PChain stack(PChain stack, void* memory_adress) {

	PChain new = new_chain(memory_adress);

	if (stack == NULL) {
		stack = new;
		return stack;
	}
	else {
		new->next = stack;
		stack = new;

		return stack;
	}
}

PChain unstack(PChain stack, void** pop) {

	if (stack == NULL) {
		printf("\n*** Impossible to unstack : empty stack ***\n");
		return stack;
	}
	else {
		PChain p_stack = stack;

		*pop = p_stack->memory_adress;
		stack = p_stack->next;

		free(p_stack);
		p_stack = NULL;

		return stack;
	}
}

void print_stack(PChain stack) {

	if (stack == NULL) {
		printf("\n*** EMPTY STACK ***\n");
		return;
	}

	PChain p_stack = stack;

	printf("\n");
	while(p_stack != NULL) {
		printf("%p\n", p_stack->memory_adress),
		p_stack = p_stack->next;
	}
	printf("\n");
	
	return;
}

void* malloc_m(size_t size, PChain* memory_stack) {

	void* buffer = (void*)malloc(size);
	*memory_stack = stack(*memory_stack, buffer);

	return buffer;
}

void free_m(void* block, PChain* memory_stack) {

	void* pop = 0;

	free(block);
	*memory_stack = unstack(*memory_stack, &pop);

	return;
}

