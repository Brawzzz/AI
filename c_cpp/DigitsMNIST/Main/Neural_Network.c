//-------------------------- INCLUDES --------------------------//
#include "Neural_Network.h"

//-------------------------- FUNCTIONS DEFINITION --------------------------//

double perceptron(PMatrix X, PMatrix W, double (*activation_function)(double)) {

	if (X == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : perceptron) ***\n", NULL_POINTER_ERROR);
	}
	if (!is_column(X) || !is_column(W)){
		error("\n*** A perceptron takes two columns vectors in arguments (function : perceptron) ***\n", DIM_ERROR);
	}

	double sum = 0.f;

	for (int i = 0; i < X->height; i++){
		sum += X->tab[i] * W->tab[i];
	}

	return (*activation_function)(sum);
}

double sigmoid_d(double x) {
	return(1.0f / (1.0f + exp(-x)));
}

double sigmoid_deriv_d(double x) {
	return( x * (1 - x) );
}

PMatrix sigmoid_m(PMatrix m) {

	if (m == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : sigmoid) ***\n", NULL_POINTER_ERROR);
	}

	int length = m->width * m->height;

	PMatrix sigmoid_m = create_matrix(m->width, m->height, 0);

	for (int i = 0; i < length; i++){
		sigmoid_m->tab[i] = sigmoid_d(m->tab[i]);
	}

	return sigmoid_m;
}

PMatrix sigmoid_deriv_m(PMatrix m) {

	if (m == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : sigmoid) ***\n", NULL_POINTER_ERROR);
	}

	int length = m->width * m->height;

	PMatrix sigmoid_m = create_matrix(m->width, m->height, 0);

	for (int i = 0; i < length; i++){
		sigmoid_m->tab[i] = sigmoid_deriv_d(m->tab[i]);
	}

	return sigmoid_m;
}

double ReLU_d(double x) {
	return max(EPSI_RELU * x, x);
}

double ReLU_deriv_d(double x) {
	return(1.f ? x > 0.f : EPSI_RELU);
}

void ReLU_m(PMatrix m, PMatrix relu_m) {

	if (m == NULL || relu_m == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : ReLU) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	for (int i = 0; i < m->width * m->height; i++){
		relu_m->tab[i] = ReLU_d(m->tab[i]);
	}

	return;
}

void ReLU_deriv_m(PMatrix m, PMatrix relu_deriv_m) {

	if (m == NULL || relu_deriv_m ==  NULL) {
		error("\n*** Matrix pointng on NULL was given (function : ReLU_deriv) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	for (int i = 0; i < m->width * m->height; i++){
		relu_deriv_m->tab[i] = ReLU_deriv_d(m->tab[i]);;
	}
	return;
}

void softmax(PMatrix m, PMatrix softmax_m) {

	if (m == NULL || softmax_m == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : softmax) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}
		
	double value = 0.f;
	
	PMatrix m_exp = create_matrix(m->width, m->height, 0);
	PMatrix column_sum_m_exp = create_matrix(m->width, 1, 0);

	matrix_exp(m, m_exp);
	column_sum(m_exp, column_sum_m_exp);

	for (int i = 0; i < softmax_m->height; i++) {
		for (int j = 0; j < softmax_m->width; j++) {
			value = get_m(m_exp, i, j) / get_m(column_sum_m_exp, 0, j);
			set_m(softmax_m, i, j, value);
		}
	}
	free(m_exp);
	m_exp = NULL;

	free(column_sum_m_exp);
	column_sum_m_exp = NULL;

	return;
}

PMatrix one_hot(PMatrix m) {

	if (m == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : one_hot) ***\n", NULL_POINTER_ERROR);
	}

	int i_err = 0;
	int j_err = 0;
	int one_hot_width = (int)max_tab(m->tab, &i_err, &j_err, m->width, m->height) + 1;
	int one_hot_height = m->width * m->height;

	int height_index = 0;
	int width_index = 0;
	
	PMatrix flatten_m = create_matrix(m->width * m->height, 1, 0);
	flatten(m, flatten_m);

	PMatrix hot_m = create_matrix(one_hot_width, one_hot_height, 0);
	for (int i = 0; i < m->width; i++) {
		set_m(hot_m, i, (int)ceil((flatten_m->tab[i])), 1);
	}

	free(flatten_m);
	flatten_m = NULL;

	PMatrix one_hot_m = create_matrix(one_hot_height, one_hot_width, 0);
	transpose(hot_m, one_hot_m);
	
	free(hot_m);
	hot_m = NULL;

	return one_hot_m;
}

void forward_prop(PParams p1, PParams p2, PMatrix X, PMatrix Z1, PMatrix A1, PMatrix Z2, PMatrix A2) {

	PMatrix product = NULL;

	product = create_matrix(X->width, (p1->W)->height, 0);
	mult(p1->W, X, product);

	add(product, p1->b, Z1);
	ReLU_m(Z1, A1);

	free(product);
	product = NULL;

	product = create_matrix(A1->width, (p2->W)->height, 0);
	mult(p2->W, A1, product);

	add(product, p2->b, Z2);
	softmax(Z2, A2);

	free(product);
	product = NULL;

	return;
}

void backward_prop(PMatrix Z1, PMatrix A1, PMatrix Z2, PMatrix A2, PMatrix W1, PMatrix W2, PMatrix X, PMatrix Y, PParams dp1, PParams dp2) {

	if (X == NULL || Y == NULL || Z1 == NULL || A1 == NULL || Z2 == NULL || A2 == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : backward_prop) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	double ratio = 1.0f /(double)X->height;
	double value = 0.f;

	PMatrix one_hot_Y = NULL;
	PMatrix relu_deriv_Z1 = NULL;

	PMatrix dZ1 = create_matrix(Z1->width, Z1->height, 0);
	PMatrix dZ2 = create_matrix(Z2->width, Z2->height, 0);

	PMatrix product = NULL;

	one_hot_Y = one_hot(Y);

	relu_deriv_Z1 = create_matrix(Z1->width, Z1->height, 0);
	ReLU_deriv_m(Z1, relu_deriv_Z1);

	PMatrix transpose_X = create_matrix(X->height, X->width, 0);
	transpose(X, transpose_X);

	PMatrix transpose_A1 = create_matrix(A1->height, A1->width, 0);
	transpose(A1, transpose_A1);

	PMatrix transpose_W2 = create_matrix(W2->height, W2->width, 0);
	transpose(W2, transpose_W2);

	sub_m(A2, one_hot_Y, dZ2);

	product = create_matrix(transpose_A1->width, dZ2->height, 0);
	mult(dZ2, transpose_A1, product);

	mult_by_scalar(product, ratio, dp2->W);
	(dp2->b)->tab[0] = ratio * sum(dZ2);

	free(product);
	product = NULL;

	product = create_matrix(dZ2->width, transpose_W2->height, 0);
	mult(transpose_W2, dZ2, product);

	hadamard_product(product, relu_deriv_Z1, dZ1);

	free(product);
	product = NULL;

	product = create_matrix(transpose_X->width, dZ1->height, 0);
	mult(dZ1, transpose_X, product);

	mult_by_scalar(product, ratio, dp1->W);
	(dp1->b)->tab[0] = ratio * sum(dZ1);

	free(product);
	product = NULL;

	free(transpose_X);
	transpose_X = NULL;

	free(transpose_A1);
	transpose_A1 = NULL;

	free(transpose_W2);
	transpose_W2 = NULL;

	free(dZ1);
	dZ1 = NULL;

	free(dZ2);
	dZ2 = NULL;

	free(one_hot_Y);
	one_hot_Y = NULL;

	free(relu_deriv_Z1);
	relu_deriv_Z1 = NULL;

	return;
}

void update_params(PParams p1, PParams p2, PParams dp1, PParams dp2, double alpha) {

	double gradient_step = 0.f;

	PMatrix product = NULL;

	gradient_step = (dp1->b)->tab[0] * (-alpha);

	product = create_matrix((dp1->W)->width, (dp1->W)->height, 0);
	mult_by_scalar(dp1->W, alpha, product);

	sub_m(p1->W, product, p1->W);
	PMatrix_add_scalar(p1->b, gradient_step, p1->b);

	free(product);
	product = NULL;

	gradient_step = (dp2->b)->tab[0] * (-alpha);
	
	product = create_matrix((dp2->W)->width, (dp2->W)->height, 0);
	mult_by_scalar(dp2->W, alpha, product);

	sub_m(p2->W, product, p2->W);
	PMatrix_add_scalar(p2->b, gradient_step, p2->b);

	free(product);
	product = NULL;

	return;
}

PMatrix get_prediction(PMatrix A2) {

	if (A2 == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : get_prediction) ***\n", NULL_POINTER_ERROR);
	}
	return arg_max(A2, 0);
}

double get_accuracy(PMatrix prediction, PMatrix Y) {
	
	if (prediction == NULL || Y == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : get_accuracy) ***\n", NULL_POINTER_ERROR);
	}

	double accuracy = 0.f;

	PMatrix accuracy_matrix = boolean_matrix(Y, prediction);

	printf("\nprediction : \n");
	print_matrix(prediction);

	printf("\nLabel : \n");
	print_matrix(Y);

	accuracy = (sum(accuracy_matrix) / (double)Y->width);

	free(accuracy_matrix);
	accuracy_matrix = NULL;

	return accuracy;
}

void gradient_descent(PMatrix X, PMatrix Y, double precision, int iterations, PParams p1, PParams p2) {

	if (X == NULL || Y == NULL) {
		error("\n*** Matrix pointng on NULL was given (function : gradient_descent) ***\n", NULL_POINTER_ERROR);
	}

	double accuracy = 0.f;

	PMatrix Z1 = create_matrix(X->width, 10, 0);
	PMatrix A1 = create_matrix(X->width, 10, 0);
	PMatrix Z2 = create_matrix(X->width, 10, 0);;
	PMatrix A2 = create_matrix(X->width, 10, 0);;
	
	PMatrix dW1 = create_matrix(W1_WIDTH, W1_HEIGHT, 0);
	PMatrix db1 = create_matrix(1, 1, 0);
	PMatrix dW2 = create_matrix(W2_WIDTH, W2_HEIGHT, 0);
	PMatrix db2 = create_matrix(1, 1, 0);

	PMatrix prediction = NULL;

	PParams dp1 = NULL;
	PParams dp2 = NULL;

	printf("\n\tInitializing parameters...");
	fflush(stdout);

	p1 = init_params(W1_WIDTH, W1_HEIGHT, b1_WIDTH, b1_HEIGHT, -0.5f, 0.5f);
	p2 = init_params(W2_WIDTH, W2_HEIGHT, b2_WIDTH, b2_HEIGHT, -0.5f, 0.5f);

	dp1 = set_params(dW1, db1);
	dp2 = set_params(dW2, db2);

	printf("DONE !\n");
	fflush(stdout);

	for (int i = 0; i <= iterations; i++) {
		
		if (accuracy > 0.95) {
			return;
		}

		printf("\nforward_prop...");
		fflush(stdout);
		forward_prop(p1, p2, X, Z1, A1, Z2, A2);
		printf("DONE\n");
		fflush(stdout);

		printf("\nbackward_prop...");
		fflush(stdout);
		backward_prop(Z1, A1, Z2, A2, p1->W, p2->W, X, Y, dp1, dp2);
		printf("DONE\n");
		fflush(stdout);

		printf("\nupdate_params...");
		fflush(stdout);
		update_params(p1, p2, dp1, dp2, precision);;
		printf("DONE\n");
		fflush(stdout);

		if (i % 10 == 0) {

			printf("\nIterations : %d\n", i);
			fflush(stdout);

			prediction = get_prediction(A2);
			accuracy = get_accuracy(prediction, Y);

			printf("accuracy : %.15lf\n", accuracy);
			fflush(stdout);

			free(prediction);
			prediction = NULL;
		}
	}
	printf("\n\n");

	free(dW1);
	dW1 = NULL;

	free(db1);
	db1 = NULL;

	free(dW2);
	dW2 = NULL;

	free(db2);
	db2 = NULL;

	free(dp1);
	dp1 = NULL;

	free(dp2);
	dp2 = NULL;

	return;
}

PMatrix make_prediction(PMatrix X, PParams p1, PParams p2) {

	if (p1 == NULL || p2 == NULL) {
		error("\n*** Params pointng on NULL was given (function : make_prediction) ***\n", NULL_POINTER_ERROR);
		exit(INT_MIN);
	}

	PMatrix Z1 = NULL;
	PMatrix A1 = NULL;
	PMatrix Z2 = NULL;
	PMatrix A2 = NULL;

	PMatrix prediction = NULL;
	
	forward_prop(p1, p2, X, Z1, A1, Z2, A2);
	prediction = get_prediction(A2);

	free(Z1);
	free(A1);
	free(Z2);
	free(A2);

	return prediction;
}