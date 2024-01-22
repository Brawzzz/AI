//-------------------------- INCLUDES --------------------------//
#include "Data_Process.h"
#include "utilities.h"

//-------------------------- FUNCTIONS DEFINITIONS --------------------------//
PMatrix get_train_examples(const char* data_file_name) {

	if (data_file_name == NULL) {
		error("\n*** *** File name is unusable (function : get_pixels_data) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	FILE* data_file = NULL;

	errno_t err = fopen_s(&data_file, data_file_name, "r");
	if (err != 0) {
		fprintf(stderr, "\n*** Error while opening the file ***\n");
		exit(INT_MIN);
	}

	int rows = 0;
	int field_count = 0;
	int datas_index = 0;
	int end_index = 0;
	int length = DATASET_WIDTH * DATASET_HEIGHT;

	char* token = NULL;
	char* next_token = NULL;
	char data_row[MAX_LINE_SIZE] = { 0 };
	char* fields[DATASET_WIDTH] = { 0 };

	double* datas = (double*)malloc(length * sizeof(double));
	if (datas == NULL) {
		error("\n*** allocation error ***\n", ALLOCATION_ERROR);
	}

	PMatrix data_array = create_matrix(DATASET_WIDTH, DATASET_HEIGHT, 0);

	while (fgets(data_row, sizeof(data_row), data_file) != NULL) {
		
		data_row[strcspn(data_row, "\n")] = '\0';
		field_count = 0;

		token = strtok_s(data_row, ",", &next_token);
		while (token != NULL && field_count < DATASET_WIDTH) {

			datas[datas_index] = (double)atoi(token);
			token = strtok_s(NULL, ",", &next_token);
			field_count++;
			datas_index ++;
		}
	}
	fclose(data_file);
	data_file = NULL;

	fill_matrix(data_array, datas, length);

	free(datas);
	datas = NULL;

	return data_array;
}

void normalize_data(PMatrix datas, double min, double max, double new_min, double new_max) {

	if (datas == NULL) {
		error("\n*** Matrix pointing on NULL was given (function : normalize_data) ***\n", NULL_POINTER_ERROR);
	}

	int length = datas->width * datas->height;

	for (int  i = 0; i < length; i++){
		datas->tab[i] = new_min + (new_max - new_min) * ((datas->tab[i] - min) / (max - min));
	}

	return;
}

void train_set(PMatrix datas, int nb_train_examples, PMatrix X_train, PMatrix Y_train) {

	if (datas == NULL) {
		error("\n*** Matrix pointing on NULL was given (function : train_set) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}
	if (nb_train_examples > DATASET_HEIGHT) {
		error("\n*** Cannot split with nb_train_examples larger than the number of datas (function : train_set) ***\n", DIM_ERROR);
		exit(INT_MIN);
	}

	int nb_pixels = DATASET_WIDTH - 1;

	PMatrix X = create_matrix(nb_pixels, nb_train_examples, 0);
	PMatrix Y = create_matrix(1, nb_train_examples, 0);
	
	PMatrix labels = create_matrix(1, nb_train_examples, 0);
	PMatrix train_datas = truncate_matrix(datas, nb_train_examples, 0, 1);

	PMatrix* pixels_columns = get_column_range(train_datas, 1, train_datas->width);

	get_column(train_datas, 0, labels);
	fill_matrix(Y, labels->tab, nb_train_examples);
	insert_columns(pixels_columns, nb_train_examples, nb_pixels, X);

	normalize_data(X, 0.f, 255.f, 0.f, 1.f);

	transpose(X, X_train);
	transpose(Y, Y_train);

	for (int i = 0; i < nb_pixels; i++){
		free(pixels_columns[i]);
		pixels_columns[i] = NULL;
	}
	free(pixels_columns);
	pixels_columns = NULL;

	free(labels);
	labels = NULL;

	free(X);
	X = NULL;

	free(Y);
	Y = NULL;

	free(train_datas);
	train_datas = NULL;

	return;
}

void dev_set(PMatrix datas, int nb_dev_examples, PMatrix X_dev, PMatrix Y_dev) {
	
	if (datas == NULL) {
		error("\n*** Matrix pointing on NULL was given (function : train_set) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}
	if (nb_dev_examples > DATASET_HEIGHT) {
		error("\n*** Cannot split with nb_train_examples larger than the number of datas (function : train_set) ***\n", DIM_ERROR);
		exit(INT_MIN);
	}

	int nb_pixels = DATASET_WIDTH - 1;

	PMatrix X = create_matrix(nb_pixels, nb_dev_examples, 0);
	PMatrix Y = create_matrix(1, nb_dev_examples, 0);

	PMatrix labels = create_matrix(1, nb_dev_examples, 0);
	PMatrix dev_datas = truncate_matrix(datas, nb_dev_examples, 0, 1);

	PMatrix* pixels_columns = get_column_range(dev_datas, 1, dev_datas->width);

	get_column(dev_datas, 0, labels);
	fill_matrix(Y, labels->tab, nb_dev_examples);
	insert_columns(pixels_columns, nb_dev_examples, nb_pixels, X);

	normalize_data(X, 0.f, 255.f, 0.f, 1.f);

	transpose(X, X_dev);
	transpose(Y, Y_dev);

	for (int i = 0; i < nb_pixels; i++) {
		free(pixels_columns[i]);
		pixels_columns[i] = NULL;
	}
	free(pixels_columns);
	pixels_columns = NULL;

	free(labels);
	labels = NULL;

	free(X);
	X = NULL;

	free(Y);
	Y = NULL;

	free(dev_datas);
	dev_datas = NULL;

	return;
}

void get_example(const char* file_name, int example_index, PMatrix* pX, PMatrix* pY) {

	if (file_name == NULL) {
		error("\n*** File name is unusable (function : get_example) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	const int EXAMPLE_WIDTH = 784;

	FILE* example_file = NULL;

	PMatrix X = create_matrix(EXAMPLE_WIDTH, 1, 0);
	PMatrix transpose_X = NULL;
	PMatrix Y = create_matrix(1, 1, 0);

	errno_t err = fopen_s(&example_file, file_name, "r");
	if (err != 0) {
		fprintf(stderr, "\n***Error while opening the file (function : get_example) ***\n");
		exit(err);
	}

	int field_count = 0;
	int datas_index = 0;

	char* token = NULL;
	char* next_token = NULL;
	char data_row[2 * MAX_LINE_SIZE] = { 0 };

	double* datas = (double*)malloc(EXAMPLE_WIDTH * sizeof(double));
	if (datas == NULL) {
		error("\n*** allocation error *** (function : get_example)\n", ALLOCATION_ERROR);
		exit(INT_MIN);
	}

	if (example_index >= 0){
		int counter = 0;
		do {
			fgets(data_row, sizeof(data_row), example_file);
			counter++;
		} while (counter <= example_index);
		fclose(example_file);
		example_file = NULL;
	}	

	data_row[strcspn(data_row, "\n")] = '\0';
	field_count = 0;

	if (pY != NULL) {
		token = strtok_s(data_row, ",", &next_token);
		sscanf_s(token, "%lf", &(Y->tab[0]));
		token = strtok_s(NULL, ",", &next_token);
	}
	else {
		token = strtok_s(data_row, ",", &next_token);
	}
	
	while (token != NULL && field_count < EXAMPLE_WIDTH) {

		sscanf_s(token, "%lf", &datas[datas_index]);
		token = strtok_s(NULL, ",", &next_token);
		field_count++;
		datas_index++;
	}
	fill_matrix(X, datas, EXAMPLE_WIDTH);

	free(datas);
	datas = NULL;

	transpose_X = create_matrix(X->height, X->width, 0);
	transpose(X, transpose_X);

	*pX = matrix_copy(transpose_X);
	if (pY != NULL) {
		*pY = matrix_copy(Y);
	}

	free(X);
	X = NULL;

	free(Y);
	Y = NULL;

	return;
}