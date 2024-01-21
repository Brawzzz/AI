#pragma once

//-------------------------- PRE-PROCESSING --------------------------//
#ifndef PARAMS_H
#define PARAMS_H

//-------------------------- DEFNES & CONSTANT --------------------------//
#define W1_WIDTH 784
#define W1_HEIGHT 10
#define b1_WIDTH 1
#define b1_HEIGHT 10

#define W2_WIDTH 10
#define W2_HEIGHT 10
#define b2_WIDTH 1
#define b2_HEIGHT 1

#define PARAMS_LINE_SIZE 512

#define FILE_WIDTH 10
#define FILE_HEIGHT 796

//-------------------------- INCLUDES --------------------------//
#include "Static_Matrix_Lib.h"

//-------------------------- STRUCTURES DEFINITIONS --------------------------//
typedef struct {
	PMatrix W;
	PMatrix b;
}Params;

typedef Params* PParams;

//-------------------------- FUNCTIONS PROTOTYPES --------------------------//

//=============== Return a PParams with W and b NULL ===============//
PParams new_params();

//=============== Return a PParams and initialize W, b  ===============//
PParams create_params(int W_width, int W_height, int b_width, int b_height) ;

//=============== Return a PParams which is initialize with random values in [min, max]  ===============//
PParams init_params(int W_width, int W_height, int b_width, int b_height, double min, double max);

//=============== Return a PParams set with W and b given  ===============//
PParams set_params(PMatrix W, PMatrix b);

//=============== Return the network's params from a file  ===============//
void get_params(const char* params_file_name, PParams p1, PParams p2);

//=============== Display a PParams  ===============//
void print_params(PParams p);

//=============== Save two PParams in a file  ===============//
void save_params(PParams p1, PParams p2, const char* file_name);

#endif
