//-------------------------- INCLUDES --------------------------//
#include "pch.h"
#include "framework.h"

#include "Static_Tables_Lib.h"
#include "utilities.h"

//-------------------------- FUNCTIONS DEFINITIONS --------------------------//
double* new_tab(int width, int height) {

	int size = width * height;

	double* new_tab = (double*)malloc(size * sizeof(double));
	if (new_tab == NULL) {
		error("*** allocation error (function : new_tab)***", ALLOCATION_ERROR);
		exit(INT_MIN);
	}

	for (int i = 0; i < size; i++) {
		new_tab[i] = 0.0f;
	}

	return new_tab;
}

bool verify_range_index(int width, int height, int row, int col) {
	if (row >= 0 && row < height && col >= 0 && col < width) {
		return true;
	}
	return false;
}

void set(double* tab, int width, int height, int row, int col, double value) {

	if (verify_range_index(width, height, row, col)) {
		tab[col + row * width] = value;
		return;
	}
	error("*** Segmentation fault (function : set) ***\n", SEG_FAULT);
	exit(INT_MIN);
}

double get(double* tab, int width, int height, int row, int col) {
	if (verify_range_index(width, height, row, col)) {
		return tab[col + row * width];
	}
	error("*** Segmentation fault (function : get) ***\n", SEG_FAULT);
	exit(INT_MIN);
}

void get_bi_dimensional_index(int linear_index, int width, int height, int* row, int* col) {
	
	if (linear_index > width * height) {
		error("*** Segmentation fault : linear index out of range (function : get_bi_dimensional_index) ***\n", SEG_FAULT);
	}
	*row = linear_index / width;
	*col = linear_index % width;

	return;
}

int get_linear_index(int width, int height, int row, int col) {

	if (!verify_range_index(width, height, row, col)) {
		error("*** Segmentation fault (row, col) out of range (function : get_linear_index) ***", SEG_FAULT);
	}
	int linear_index = row + col * width;
	return linear_index;
}

void print_tab(double* tab, int width, int height, FILE* file) {

	if (tab == NULL) {
		error("*** Table pointng on NULL was given (function : print_tab) ***", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	int size = width * height;

	double value = 0.0f;

	if (file != NULL) {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				value = get(tab, width, height, i, j);
				fprintf_s(file, "%.15lf,", value);
			}
			fprintf_s(file, "\n");
		}
		// fprintf_s(file, "\n");

		return;
	}

	//========== Display of oversize tables ==========//
	if (height > MAX_HEIGHT && width > MAX_WIDTH){

		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				value = get(tab, width, height, i, j);
				printf("%lf \t", value);
			}
			printf("...\t%lf\n", get(tab, width, height, i, width - 1));
		}
		printf("\n\t\t\t\t...\n\t\t\t\t...\n\t\t\t\t...\n\n\n");
		for (int j = 0; j < MAX_WIDTH; j++) {
			value = get(tab, width, height, height - 1, j);
			printf("%lf\t", value);
		}
		printf("...\t%lf\n", get(tab, width, height, height - 1, width - 1));
		printf("\n");
	}

	else if (height > MAX_HEIGHT && width <= MAX_WIDTH) {

		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < width; j++) {
				value = get(tab, width, height, i, j);
				printf("%lf \t", value);
			}
			printf("\n");
		}
		printf("\n\t\t...\n\t\t...\n\t\t...\n\n");
		for (int j = 0; j < width; j++) {
			value = get(tab, width, height, height - 1, j);
			printf("%lf\t", value);
		}
		printf("\n");
	}

	else if (height <= MAX_HEIGHT && width > MAX_WIDTH) {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				value = get(tab, width, height, i, j);
				printf("%lf \t", value);
			}
			printf("...\t%lf\n", get(tab, width, height, i, width - 1));
		}
		printf("\n");
	}

	else {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				value = get(tab, width, height, i, j);
				printf("%lf \t", value);
			}
			printf("\n");
		}
		printf("\n");
	}
}

void copy_tab(double* source, double* destination, int destination_length, int start_index, int end_index) {

	if (source == NULL || destination == NULL) {
		error("*** Table pointng on NULL was given (function : copy_tab) ***", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}
	if (destination_length < end_index - start_index) {
		error("*** Segmentation fault ; index out of range (function : copy_tab) ***\n", SEG_FAULT);
	}

	int counter = 0;
	for (int i = start_index; i < end_index; i++) {
		destination[counter] = source[i];
		counter++;
	}
	return;
}

double* sub_tab(double* tab_1, double* tab_2, int width, int height) {

	if (tab_1 == NULL || tab_2 == NULL) {
		error("*** Table pointng on NULL was given (function : sub_tab) ***", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	int size = width * height;

	double* sub_tab = new_tab(width, height);

	for (int i = 0; i < size; i++) {
		sub_tab[i] = tab_1[i] - tab_2[i];
	}
	return sub_tab;
}

double max_tab(double* tab, int* i_err, int* j_err, int width, int height) {

	if (tab == NULL) {
		error("*** Table pointng on NULL was given (function : max_tab) ***", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	int length = width * height;

	double max = DBL_MIN;
	double current_value = 0.0f;

	for (int i = 0; i < length; i++) {

		current_value = tab[i];
		if (current_value > max) {
			max = current_value;
			get_bi_dimensional_index(i, width, height, i_err, j_err);
		}
	}

	return max;
}

void shuffle(double* array, size_t n){
	
	if (array == NULL) {
		error("*** Table pointng on NULL was given (function : shuffle) ***", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	if (n > 1){

		srand((unsigned int)time(NULL));

		for (int i = 0; i < n - 1; i++){
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			double t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

double rand_from(double min, double max) {
	double range = (max - min);
	double div = RAND_MAX / range;
	return min + (rand() / div);
}

double* random_tables(int length, double min, double max) {

	double* random_values = (double*)malloc(length * sizeof(double));
	if (random_values == NULL) {
		error("*** allocation error (function : random_tables) ***", ALLOCATION_ERROR);
		exit(INT_MIN);
	}

	for (int i = 0; i < length; i++) {
		random_values[i] = rand_from(min, max);
	}

	return random_values;
}

double* add_scalar(double* tab, int length, double alpha) {

	double* new_tab = (double*)malloc(length * sizeof(double));
	if (new_tab == NULL) {
		error("*** allocation error (function : add_scalar) ***", ALLOCATION_ERROR);
		exit(INT_MIN);
	}
	for (int i = 0; i < length; i++){
		new_tab[i] = tab[i] + alpha;
	}
	return new_tab;
}
