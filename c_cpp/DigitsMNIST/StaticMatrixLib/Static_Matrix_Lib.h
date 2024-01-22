#pragma once
#pragma warning(disable:6011)

//-------------------------- PRE-PROCESSING --------------------------//
#ifndef MATRIX_H
#define MATRIX_H

//-------------------------- INCLUDES --------------------------//
#include "Static_Tables_Lib.h"

//-------------------------- DEFINES --------------------------//
#define _USE_MATH_DEFINES
#define STACK_SIZE 8192

//--------------------------STRUCTURES DEFINITIONS --------------------------//
typedef struct {

    double* tab;

    int width;
    int height;

    char name;

}Matrix;

typedef Matrix* PMatrix;

typedef struct {

    int row;
    int col;

    double scalar;

    PMatrix m1;
    PMatrix m2;
    PMatrix res;

}Args;

typedef Args* PArgs;

//-------------------------- FUNCTIONS PROTOTYPES --------------------------//

//=============== Allocate and Initialize a matrix ===============//
PMatrix     create_matrix(int width, int heigt, char name);

//=============== Display a matrix ===============//
void        print_matrix(PMatrix m);

//=============== Save a matrix in a file ===============//
void        save_matrix(PMatrix m, const char* file_name, FILE* save_file);

//=============== Return the (row, col) coficiant of m ===============//
double      get_m(PMatrix m, int row, int col);

//=============== Set the (row, col) coficiant of m at value ===============//
void        set_m(PMatrix m, int row, int col, double value);

//=============== Show the shape of a matrix ===============//
void        shape(PMatrix m);

//========================= Fill the matrix with the inputs values ====================//
//======= If values is NULL the matrix is fill with random values between -size/2 and size/2 ======//
int         fill_matrix( PMatrix m, double* values, int size);

//=============== Return a copy of the matrix m ===============//
PMatrix     matrix_copy(PMatrix m);

//=============== Get the identity matrix of dimension dim ===============//
PMatrix     get_identity(int dim);

//=============== Return a matrix full of zero ===============//
PMatrix     get_null(int width, int height);

//=============== Check if a matrix is full of zero ===============//
bool        is_null(PMatrix m);

//=============== Check if a matrix is a square matrix ===============//
bool        is_square(PMatrix m);

//=============== Check if a matrix is a square matrix ===============//
bool        is_scalar(PMatrix m);

//=============== Compute the trace of a square matrix ===============//
double      trace(PMatrix m);

//=============== Compute the sum of two matrixes with multi-threading ===============//
PMatrix     add_async(PMatrix m1, PMatrix m2);
void*       add_vectors(void* args);

//=============== Compute the sum of two matrixes sequentially ===============//
void     add(PMatrix m1, PMatrix m2, PMatrix res);

//=============== Compute the difference beteween two matrixes sequentially ===============//
void sub_m(PMatrix m1, PMatrix m2, PMatrix res);

//=============== Add a scalar to each term of m ===============//
void     PMatrix_add_scalar(PMatrix m, double alpha, PMatrix res);

//=============== Compute the product of two matrixes with multi-threading ===============//
PMatrix     mult_async(PMatrix m1, PMatrix m2);
void*       mult_vectors(void* args);

//=============== Compute the product of two matrixes sequentially ===============//
void        mult(PMatrix m1, PMatrix m2, PMatrix res);

//=============== Compute the term by term product of two matrixes ===============//
void        hadamard_product(PMatrix m1, PMatrix m2, PMatrix res);

//=============== Return the Kronecker product of two matrixes ===============//
void        kronecker_product(PMatrix m1, PMatrix m2, PMatrix res);

//=============== Compute the product between a scalar and a matrix with multi-threading ===============//
PMatrix     mult_by_scalar_async(PMatrix m1, double scalar);
void*       mult_scalar_vectors(void* args);

//=============== Compute the product between a scalar and a matrix sequentially ===============//
void        mult_by_scalar(PMatrix m1, double scalar, PMatrix res);

//=============== Return a broadcasted matrix with the numpy rules ===============//
//=============== if the two matrixes have the sam dimension m2 is return ===============//
PMatrix     broadcast(PMatrix m1, PMatrix m2, int* broadcasted_matrix);

//=============== Returns a row matrix where each element is the sum of the column vectors of m  ===============//
void        column_sum(PMatrix m, PMatrix column_sum_m);

//=============== Return the sum of all the element of m ===============//
double      sum(PMatrix m);

//=============== Return vectors of the standard basis ===============//
PMatrix     get_standard_basis_vector(int index, int dim);

//=============== Compute the euclidian norm of a vector ===============//
double      euclidian_norm(PMatrix v);

//=============== Return the determinant of a square matrix ===============//
double      det(PMatrix m);

//=============== Return the sub matrix associated to (row, col) coefficiant ===============//
PMatrix     get_sub_matrix(PMatrix m, int row, int col);

//=============== Return commatrice of a square matrix ===============//
PMatrix     com(PMatrix m, PMatrix com_m, int index);

//=============== Return transpose of a square matrix ===============//
void        transpose(PMatrix m, PMatrix transpose_m);

//=============== Return inverse of a square matrix ===============//
PMatrix     inverse(PMatrix m);

//=============== Check if a matrix is a column ===============//
bool        is_column(PMatrix m);

//=============== Check if a matrix is a row ===============//
bool        is_row(PMatrix m);

//=============== Retur the columns vectors of a matrix ===============//
PMatrix*    get_column_vectors(PMatrix m);

//=============== Return a range of the column vectors of a matrix ===============//
PMatrix*    get_column_range(PMatrix m, int start_index, int end_index);

//=============== Return a specific column vector of a matrix ===============//
void        get_column(PMatrix m, int column_index, PMatrix column_i);

//=============== Return the rows vectors of a matrix ===============//
PMatrix*    get_row_vectors(PMatrix m);

//=============== Return a range of the row vectors of a matrix ===============//
PMatrix*    get_row_range(PMatrix m, int start_index, int end_index);

//=============== Return a specific row vector of a matrix ===============//
void        get_row(PMatrix m, int row_index, PMatrix row_i);

//=============== Fill a matrix with given row matrixes ===============//
void        insert_rows(PMatrix* row_matrixes, int width, int length, PMatrix new_m);

//=============== Fill a matrix with given column matrixes ===============//
void        insert_columns(PMatrix* column_matrixes, int height, int length, PMatrix new_m);

//=============== Truncate a matrix by removing the row with index "row" ===============//
//=================       and the coluumn with index "column"       ===================//
//================= If sens and col are 0 then it's cut the matrix an return the part under row  ===================//
PMatrix     truncate_matrix(PMatrix m, int row, int col, int sens);

//=============== Shuffle the rows of a matrix ===============//
PMatrix     shuffle_rows(PMatrix m);

//=============== Transform a random matrix into a row matrix ===============//
void        flatten(PMatrix m, PMatrix flatten_m);

//=============== Return a matrix where each element is the max between the corresponding element of x1 and x2 ===============//
PMatrix     maximum(PMatrix m1, PMatrix m2);

//=============== Return the exp of m ===============//
void        matrix_exp(PMatrix m, PMatrix m_exp);

//=============== Return a boolean matrix which is true where m_ij = prediction_ij ===============//
PMatrix     boolean_matrix(PMatrix m, PMatrix prediction);

//=============== Return a row matrix where each element is the index of the maximum in this column or the row ===============//
PMatrix     arg_max(PMatrix m, int axis);

//bool is_upper_triangular(Matrix m, float* e);
//
//bool is_lower_triangular(Matrix m, float* e);
//
//bool is_diagonal(Matrix m);
//
//float* get_diagonal(Matrix m);
//
//Matrix minor_matrix(Matrix m, int col_index, int row_index);
//
//int search_pivot(Matrix v);
//
//Matrix swap_rows(Matrix m, int old_row_index, int new_row_index);
//
//Matrix mult_row_by_scalar(Matrix m, int row_index, float scalar);
//
//Matrix add_linear_combination(Matrix m, int receiver_row_index, int adding_row_index, float scalar);
//
//Matrix gauss_jordan(Matrix m);
//
//Matrix resolve_linear_system(Matrix A, Matrix B);
//
//Matrix make_householder_matrix(Matrix m, int diff);
//
//void QR_decomposition(Matrix m, Matrix* Q, Matrix* R);
//
//float* eigenvalues(Matrix m);
//
//Matrix* eigenvectors(Matrix m);

#endif


