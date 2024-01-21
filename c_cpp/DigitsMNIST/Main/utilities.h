#pragma once

//-------------------------- PRE-PROCESSING --------------------------//
#ifndef UTILITIES_H
#define UTILITIES_H

//-------------------------- DEFINES --------------------------//
#define CLEAR_TERMINAL 70

#define ALLOCATION_ERROR -1
#define SEG_FAULT -2
#define EXIST_ERROR -3
#define DIM_ERROR -4
#define NULL_POINTER_ERROR -5

//-------------------------- FUNCTIONS PROTOTYPES --------------------------//

//=============== Clear a specific number of rows on stdout ===============//
void clear(int nmb_rows);

//=============== Display a progression bar in a ceratin state ===============//
void progression_bar(int state, int end);

//=============== Display an error message in stderr and exit the code ===============//
void error(char* error_msg, int error_number);

#endif