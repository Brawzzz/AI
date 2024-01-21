#pragma once

//-------------------------- PRE-PROCESSING --------------------------//
#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

//-------------------------- DEFINES --------------------------//
#define MAX_LINE_SIZE 8062
#define DATASET_WIDTH 785
#define DATASET_HEIGHT 42000

//-------------------------- INCLUDES --------------------------//
#include "Static_Matrix_Lib.h"

//-------------------------- FUNCTIONS PROTOTYPES --------------------------//

//=============== Return a matrix fill with pixels data ===============//
PMatrix get_train_examples(const char* data_file_name);

//=============== Normalize data from [min, max] to [new_min, new_max] ===============//
void normalize_data(PMatrix datas, double min, double max, double new_min, double new_max);

//=============== Return a part of the data set that will be used as train set ===============//
void train_set(PMatrix datas, int nb_train_examples, PMatrix* X_train, PMatrix* Y_train);

//=============== Return a part of the data set that will be used as dev set ===============//
void dev_set(PMatrix datas, int nb_dev_examples, PMatrix* X_dev, PMatrix* Y_dev);

//=============== Return an example image for a test ===============//
void get_example(const char* file_name, int example_index, PMatrix* pX, PMatrix* pY);

#endif
