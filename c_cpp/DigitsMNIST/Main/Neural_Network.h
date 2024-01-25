#pragma once

//-------------------------- PRE-PROCESSING --------------------------//
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

//-------------------------- INCLUDES --------------------------//
#include <windows.h>
#include <math.h>

#include "utilities.h"
#include "Static_Matrix_Lib.h"
#include "Params.h"

//-------------------------- INCLUDES --------------------------//
#define EPSI_RELU 0.000001
//-------------------------- FUNCTIONS PROTOTYPES --------------------------//

//=============== Return the output of a perceptron with v as input and W for the weights ===============//
double perceptron(PMatrix X, PMatrix W, double (*activation_function)(double));

//=============== Return the image of a real number x by the sigmoid function ===============//
double sigmoid_d(double x);

//=============== Return the image of a real number x by the derivative of th sigmoid function ===============//
double sigmoid_deriv_d(double x);

//=============== Return a matrix which is the image of m by the sigmoid function ===============//
PMatrix sigmoid_m(PMatrix m);

//=============== Return a matrix which is the image of m by the derivative of the sigmoid function ===============//
PMatrix sigmoid_deriv_m(PMatrix m);

//=============== Return the image of x by the ReLU function ===============//
double ReLU_d(double x);

//=============== Return the image of x by the derivative of the ReLU function ===============//
double ReLU_deriv_d(double x);

//=============== Return a matrix which is the image of m by the ReLU function ===============//
void ReLU_m(PMatrix m, PMatrix relu_m);

//=============== Return a matrix which is the image of m by the derivative of the ReLU function ===============//
void ReLU_deriv_m(PMatrix m, PMatrix relu_deriv_m);

//=============== Return a matrix which is the image of m by the softmax function ===============//
void softmax(PMatrix m, PMatrix softmax_m);

//=============== Return the one hot encoding of m ===============//
PMatrix one_hot(PMatrix m);

//=============== Compute the forward propagation ===============//
void forward_prop(PParams p1, PParams p2, PMatrix X, PMatrix Z1, PMatrix A1, PMatrix Z2, PMatrix A2);

//=============== Compute the backward propagation ===============//
void backward_prop(	PMatrix Z1,	PMatrix A1, PMatrix Z2,	PMatrix A2, PMatrix W1, PMatrix W2, PMatrix X, PMatrix Y, PParams dp1,	PParams dp2);

//=============== Update the differents parameters ===============//
void update_params(PParams p1, PParams p2, PParams dp1, PParams dp2, double alpha);

//=============== Return the prdictions of the model ===============//
PMatrix get_prediction(PMatrix A2);

//=============== Return the accuracy of the model ===============//
double get_accuracy(PMatrix prediction, PMatrix Y);

//=============== Compute the gradient descent algorithm ===============//
void gradient_descent(PMatrix X, PMatrix Y, double precision, int iterations, PParams* p1, PParams* p2);

//=============== Make a prediction on the examples X ===============//
PMatrix make_prediction(PMatrix X, PParams p1, PParams p2);

#endif
