#pragma once

//-------------------------- PRE-PROCESSING --------------------------//
#ifndef TABLES_H
#define TABLES_H

//-------------------------- DEFINES --------------------------//
#define MAX_HEIGHT 784
#define MAX_WIDTH 7

//-------------------------- INCLUDES --------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <float.h>

//-------------------------- FUNCTIONS PROTOTYPES --------------------------//

//=============== Allocate and Initialize a 2D table ===============//
double* new_tab(int width, int height);

//=============== Check if (row, col) is the tab of (width, height) dimension ===============//
bool verify_range_index(int width, int height, int row, int col);

//=============== Set (row, col) to value ===============//
void set(double* tab, int width, int height, int row, int col, double value);

//=============== Return the (row, col) value ===============//
double get(double* tab, int width, int height, int row, int col);

//=============== Return (row, col) associated to a linear index ===============//
void get_bi_dimensional_index(int linear_index, int width, int height, int* row, int* col);

//=============== Return the linear index associated to (row, col) ===============//
int get_linear_index(int width, int height, int row, int col);

//=============== Display a 2D table ===============//
void print_tab(double* tab, int width, int height, FILE* file);

//=============== Copy the content of source in destination ===============//
void copy_tab(double* source, double* destination, int destination_length, int start_index, int end_index);

//=============== Return the term by term difference betwe two tables ===============//
double* sub_tab(double* tab_1, double* tab_2, int width, int height);

//=============== Return the maximum element of a table ===============//
double max_tab(double* tab, int* i_err, int* j_err, int width, int height);

//=============== Shuffle a table ===============//
void shuffle(double* array, size_t n);

//=============== Return a random double between min and max ===============//
double rand_from(double min, double max);

//=============== Return a table fill width random values betwen min and max ===============//
double* random_tables(int length, double min, double max);

//=============== Add a scalar to each element of a table ===============//
double* add_scalar(double* tab, int length, double alpha);

#endif
