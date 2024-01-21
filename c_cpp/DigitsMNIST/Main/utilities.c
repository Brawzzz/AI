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
