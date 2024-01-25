//-------------------------- INCLUDES --------------------------//
#include "Params.h"
#include "utilities.h"

//-------------------------- FUNCTIONS DEFINITIONS --------------------------//

PParams create_params(int W_width, int W_height, int b_width, int b_height) {

	PParams p = (PParams)malloc(sizeof(Params));
	if (p == NULL) {
		error("\n*** allocation error (function : create_params) ***\n", ALLOCATION_ERROR);
	}

	p->W = create_matrix(W_width, W_height, 0);
	p->b = create_matrix(b_width, b_height, 0);

	return p;
}

void delete_params(PParams p) {

	if (p == NULL) {
		printf("\n*** Params already NULL ***\n");
		return;
	}

	delete_matrix(p->W);
	delete_matrix(p->b);
	p = NULL;

	return;
}

PParams init_params(int W_width, int W_height, int b_width, int b_height, double min, double max) {

	int W_length = W_width * W_height;

	PParams p_params = create_params(W_width, W_height, b_width, b_height);
	(p_params->W)->tab = random_tables(W_length, min, max);

	return p_params;
}

PParams set_params(PMatrix W, PMatrix b) {

	if (W == NULL || b == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : set_params) ***\n", NULL_POINTER_ERROR);
	}

	PParams p = create_params(W->width, W->height, b->width, b->height);
	
	p->W = W;
	p->b = b;

	return p;
}

void get_params(const char* params_file_name, PParams p1, PParams p2) {

	if (params_file_name == NULL) {
		error("\n*** file name is unusable (function : get_params) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	int field_count = 0;
	int datas_index = 0;

	int length = FILE_WIDTH * FILE_HEIGHT;

	char* token = NULL;
	char* next_token = NULL;
	char data_row[PARAMS_LINE_SIZE] = { 0 };

	double* datas = (double*)malloc(length * sizeof(double));
	if (datas == NULL) {
		error("\n*** allocation error (function : get_params) ***\n", ALLOCATION_ERROR);
	}

	FILE* params_file = NULL;

	PMatrix data_array = create_matrix(FILE_WIDTH, FILE_HEIGHT, 0);
	PMatrix last_row = create_matrix(W1_HEIGHT, 1, 0);
	PMatrix data_array_rows = NULL;
	PMatrix* row_range = NULL;
	
	errno_t err = fopen_s(&params_file, params_file_name, "r");
	if (err != 0) {
		fprintf(stderr, "\n*** Error while opening the file (function : get_params) ***\n");
		exit(err);
	}

	while (fgets(data_row, sizeof(data_row), params_file) != NULL) {

		data_row[strcspn(data_row, "\n")] = '\0';
		field_count = 0;

		token = strtok_s(data_row, ",", &next_token);
		while (token != NULL && field_count < FILE_HEIGHT) {

			sscanf_s(token, "%lf", &datas[datas_index]);
			token = strtok_s(NULL, ",", &next_token);
			field_count++;
			datas_index++;
		}
	}
	fclose(params_file);
	params_file = NULL;

	fill_matrix(data_array, datas, length);

	free(datas);
	datas = NULL;

	row_range = get_row_range(data_array, 0, W1_WIDTH);
	data_array_rows = create_matrix(W1_HEIGHT, W1_WIDTH, 0);
	
	insert_rows(row_range, W1_HEIGHT, W1_WIDTH, data_array_rows);
	get_row(data_array, W1_WIDTH, last_row);

	transpose(data_array_rows, p1->W);
	transpose(last_row , p1->b);

	for (int i = 0; i < W1_WIDTH; i++){
		delete_matrix(row_range[i]);
	}
	free(row_range);
	row_range = NULL;
	
	delete_matrix(data_array_rows);
	delete_matrix(last_row);

	row_range = get_row_range(data_array, W1_WIDTH+1, FILE_HEIGHT);
	data_array_rows = create_matrix(W1_HEIGHT, W2_WIDTH, 0);

	insert_rows(row_range, W1_HEIGHT, W2_WIDTH, data_array_rows);
	get_row(data_array, FILE_HEIGHT - 1, last_row);

	transpose(data_array_rows, p2->W);
	transpose(last_row, p2->b);

	for (int i = 0; i < FILE_HEIGHT - (W1_WIDTH + 1); i++) {
		delete_matrix(row_range[i]);
	}
	free(row_range);
	row_range = NULL;

	delete_matrix(data_array_rows);
	delete_matrix(last_row);
	delete_matrix(data_array);

	return;
}

void print_params(PParams p) {

	if (p == NULL) {
		error("\n*** Params pointng on NULL was given (function : print_params) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	printf("\nW (width : %d, height : %d)\n", (p->W)->width, (p->W)->height);
	print_matrix(p->W);
	printf("\n");

	printf("\nb (width : %d, height : %d)\n", (p->b)->width, (p->b)->height);
	print_matrix(p->b);
	printf("\n\n");
}

void save_params(PParams p1, PParams p2, const char* file_name) {

	if (p1 == NULL || p2 == NULL) {
		error("\n*** Params pointng on NULL was given (function : save_params) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}
	if (file_name == NULL){
		error("\n*** File name is unusable (function : save_params) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	int brodcasted_matrix = -1;

	FILE* save_file = NULL;
	errno_t err = fopen_s(&save_file, file_name, "w+");
	if (err != 0) {
		error("\n*** Error while opening the file : matrix_file (function : save_params) ***\n err = ", err);
		exit(INT_MIN);
	}

	PMatrix transpose_p1_W = create_matrix(p1->W->height, p1->W->width, 0);
	transpose(p1->W, transpose_p1_W);
	
	PMatrix transpose_p1_b = create_matrix(p1->b->height, p1->b->width, 0);
	transpose(p1->b, transpose_p1_b);

	save_matrix(transpose_p1_W, file_name, save_file);
	save_matrix(transpose_p1_b, file_name, save_file);
	
	delete_matrix(transpose_p1_W);
	delete_matrix(transpose_p1_b);

	p2->b = broadcast(p1->b, p2->b, &brodcasted_matrix);

	PMatrix transpose_p2_W = create_matrix(p2->W->height, p2->W->width, 0);
	transpose(p2->W, transpose_p2_W);

	PMatrix transpose_p2_b = create_matrix(p2->b->height, p2->b->width, 0);
	transpose(p2->b, transpose_p2_b);

	save_matrix(transpose_p2_W, file_name, save_file);
	save_matrix(transpose_p2_b, file_name, save_file);

	delete_matrix(transpose_p2_W);
	delete_matrix(transpose_p2_b);

	fclose(save_file);
	save_file = NULL;

	return;
}
