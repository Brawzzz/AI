//-------------------------- INCLUDES --------------------------//
#include "pch.h"
#include "framework.h"

#include <math.h>
#include <pthread.h>

#include "Static_Matrix_Lib.h"
#include "utilities.h"

//-------------------------- FUNCTIONS DEFINITIONS --------------------------//
PMatrix create_matrix(int width, int height, char name) {

    PMatrix matrix = (PMatrix)malloc(sizeof(Matrix));
    if (matrix == NULL) {
        error("\n*** allocation error (function : create_matrix) ***\n", ALLOCATION_ERROR);
        exit(INT_MIN);
    }

    double* matrix_tab = new_tab(width, height);
    if (matrix_tab == NULL) {
        error("\n*** allocation error (function : create_matrix) ***\n", ALLOCATION_ERROR);
        exit(INT_MIN);
    }

    matrix->width = width;
    matrix->height = height;
    matrix->tab = matrix_tab;
    matrix->name = name;

    return matrix;
}

void print_matrix(PMatrix m) {
    if (m != NULL) {
       print_tab(m->tab, m->width, m->height, NULL);
       return;
    }
    else {
        printf("\nNULL\n");
        return;
    }
}

void save_matrix(PMatrix m, const char* file_name, FILE* save_file) {

    if (file_name == NULL) {
        error("\n*** File name is unusable (function : save_matrix) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
    else {

        if (save_file == NULL) {

            FILE* matrix_file = NULL;
            errno_t err = fopen_s(&matrix_file, file_name, "w+");
            if (err != 0) {
                error("\n*** Error while opening the file : matrix_file (function : save_matrix) ***\n err = ", err);
                exit(INT_MIN);
            }

            if (m != NULL) {
                print_tab(m->tab, m->width, m->height, matrix_file);
                fclose(matrix_file);
                return;
            }
            else {
                fprintf_s(matrix_file, "\nNULL\n");
                fclose(matrix_file);
                return;
            }
        }
        else {

            if (m != NULL) {
                print_tab(m->tab, m->width, m->height, save_file);
                return;
            }
            else {
                fprintf_s(save_file, "\nNULL\n");
                fclose(save_file);
                return;
            }

        }
        
    }
}

double get_m(PMatrix m, int row, int col) {
    
    if (!verify_range_index(m->width, m->height, row, col)) {
        error("\n*** Segmentation fault : index out of range (function : get_m) ***\n", SEG_FAULT);
        exit(INT_MIN);
    }
    
    return get(m->tab, m->width, m->height, row, col);
}

void set_m(PMatrix m, int row, int col, double value) {

    if (!verify_range_index(m->width, m->height, row, col)) {
        error("\n*** Segmentation fault : index out of range (function : set_m) ***\n", SEG_FAULT);
        exit(INT_MIN);
    }
    set(m->tab, m->width, m->height, row, col, value);
    return;
}

void shape(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : fill_matrix) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
    printf("\n(height : %d; width: %d)\n", m->height, m->width);
    return;
}

int fill_matrix(PMatrix m, double* values, int size) {

    if (m == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : fill_matrix) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
    if (values == NULL) {

        double rand_value = 0.0f;

        for (int i = 0; i < m->height; i++) {
            for (int j = 0; j < m->width; j++) {
                rand_value = rand_from((double)(-size / 2.0f), (double)(size / 2.f));
                set_m(m, i, j, rand_value);
            }
        }
        return(EXIT_SUCCESS);
    }

    if (size > m->width * m->height) {
        return(EXIT_FAILURE);
    }
    else if (size == m->width * m->height) {
        for (int i = 0; i < size; i++) {
            m->tab[i] = values[i];
        }
        return(EXIT_SUCCESS);
    }
    else {
        for (int i = 0; i < size; i++) {
            m->tab[i] = values[i];
        }
        for (int j = size; j < m->width * m->height; j++) {
            m->tab[j] = 0.0f;
        }
        return(EXIT_SUCCESS);
    }
}

PMatrix matrix_copy(PMatrix m) {
    
    if (m == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : matrix_copy) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int m_length = m->width * m->height;

    PMatrix m_copy = create_matrix(m->width, m->height, 0);
    copy_tab(m->tab, m_copy->tab, m_length, 0, m_length);

    return m_copy;
}

PMatrix get_identity(int dim) {

    PMatrix I = create_matrix(dim, dim, 'I');

    int j = 0;

    for (int i = 0; i < dim; i++) {
        I->tab[j] = 1.0f;
        j += dim + 1;
    }
    return I;
}

PMatrix get_null(int width, int height) {
    return(create_matrix(width, height, 'O'));
}

bool is_null(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : is_null) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    for (int i = 0; i < m->height * m->width; i++) {
        if (m->tab[i] != 0.0f) {
            return false;
        }
    }
    return true;
}

bool is_square(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : is_square) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (m->width == m->height) {
        return true;
    }
    else {
        return false;
    }
}

bool is_scalar(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : is_scalar) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (m->width == m->height && m->width == 1) {
        return true;
    }
    else{
        return false;
    }
}

double trace(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : trace) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (is_square(m)) {

        int j = 0;
        int dim = m->width;

        double trace = 0.0f;

        for (int i = 0; i < dim; i++) {
            trace += m->tab[j];
            j += dim + 1;
        }
        return trace;
    }
    else {
        return NAN;
    }
}

void* add_vectors(void* args) {

    int current_col = ((PArgs)args)->col;

    double value = 0.0f;
    double m1_fk = 0.0f;
    double m2_fk = 0.0f;

    PMatrix m1 = ((PArgs)args)->m1;
    PMatrix m2 = ((PArgs)args)->m2;
    PMatrix res = ((PArgs)args)->res;

    for (int k = 0; k < res->height; k++) {
        m1_fk = get_m(m1, k, current_col);
        m2_fk = get_m(m2, k, current_col);

        value = m1_fk + m2_fk;
        set_m(res, k, current_col, value);
    }
    pthread_exit(NULL);

    return (void*)res;
}

PMatrix add_async(PMatrix m1, PMatrix m2) {

    if (m1 == NULL || m2 == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : add_async) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (m1->height != m2->height) {
        error("\n*** Impossible to add matrixes with different dimensions (function : add_async) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }
    else {

        int res_width = 0;
        int res_height = m1->height;
        
        int res_length = 0;
        int copy_length = 0;

        PMatrix res = NULL;

        if (m1->width >= m2->width) {
            res_width = m1->width;
            res_length = (res_width * res_height);

            copy_length = m1->height * m1->width;
            res = create_matrix(res_width, res_height, 'R');
            copy_tab(m1->tab, res->tab, res_length, 0, copy_length);

            pthread_t* thid = (pthread_t*)malloc(sizeof(pthread_t) * m1->width);
            if (thid == NULL) {
                error("\n*** allocation error (function : add_async) ***\n", ALLOCATION_ERROR);
                exit(INT_MIN);
            }

            for (int i = 0; i < m2->width; i++) {

                PArgs args = (PArgs)malloc(sizeof(Args));
                if (args == NULL) {
                    error("\n*** allocation error (function : add_async) ***\n", ALLOCATION_ERROR);
                    exit(INT_MIN);
                }
                args->row = 0;
                args->col = i;
                args->m1 = m1;
                args->m2 = m2;
                args->res = res;

                pthread_create(&thid[i], NULL, add_vectors, (void*)args);

                free(args);
                args = NULL;
            }
            for (int j = 0; j < m2->width; j++) {
                pthread_join(thid[j], NULL);
            }
            free(thid);
            thid = NULL;
        }
        else {
            res_width = m2->width;
            res_length = (res_width * res_height);

            copy_length = m2->height * m2->width;
            res = create_matrix(res_width, res_height, 'R');
            copy_tab(m2->tab, res->tab, res_length, 0, copy_length);

            pthread_t* thid = (pthread_t*)malloc(sizeof(pthread_t) * m1->width);
            if (thid == NULL) {
                error("\n*** allocation error (function : add_async) ***\n", ALLOCATION_ERROR);
                exit(INT_MIN);
            }

            for (int i = 0; i < m1->width; i++) {

                PArgs args = (PArgs)malloc(sizeof(Args));
                if (args == NULL) {
                    error("\n*** allocation error (function : add_async) ***\n", ALLOCATION_ERROR);
                    exit(INT_MIN);
                }
                args->row = 0;
                args->col = i;
                args->m1 = m1;
                args->m2 = m2;
                args->res = res;

                pthread_create(&thid[i], NULL, add_vectors, (void*)args);

                free(args);
                args = NULL;
            }
            for (int j = 0; j < m1->width; j++) {
                pthread_join(thid[j], NULL);
            }
            free(thid);
            thid = NULL;
        }

        return res;
    }
}

void add(PMatrix m1, PMatrix m2, PMatrix res) {

    if (m1 == NULL || m2 == NULL || res == NULL) {
        error("_n*** Matrix pointing on NULL was given (function : add) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int broadcasted = 0;

    double value = 0.f;

    PMatrix broadcast_m = NULL;
    broadcast_m = broadcast(m1, m2, &broadcasted);

    if (broadcasted == 1) {

        for (int j = 0; j < m2->width; j++) {
            for (int i = 0; i < m2->height; i++) {

                value = get_m(broadcast_m, i, j) + get_m(m2, i, j);
                set_m(res, i, j, value);
            }
        }

        free(broadcast_m);
        broadcast_m = NULL;

        return;
    }
    else if (broadcasted == 2) {

        for (int j = 0; j < m1->width; j++) {
            for (int i = 0; i < m1->height; i++) {

                value = get_m(broadcast_m, i, j) + get_m(m1, i, j);
                set_m(res, i, j, value);
            }
        }

        free(broadcast_m);
        broadcast_m = NULL;

        return;
    }
    else {
        error("\n*** Impossible to broadcast (function : add) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }
}

void sub_m(PMatrix m1, PMatrix m2, PMatrix res) {

    if (m1 == NULL || m2 == NULL || res == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : sub_m) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int broadcasted = 0;

    double value = 0.f;

    PMatrix broadcast_m = NULL;
    broadcast_m = broadcast(m1, m2, &broadcasted);

    if (broadcasted == 1) {

        for (int j = 0; j < m2->width; j++) {
            for (int i = 0; i < m2->height; i++) {

                value = get_m(broadcast_m, i, j) - get_m(m2, i, j);
                set_m(res, i, j, value);
            }
        }
        free(broadcast_m);
        broadcast_m = NULL;

        return;
    }
    else if (broadcasted == 2) {

        for (int j = 0; j < m1->width; j++) {
            for (int i = 0; i < m1->height; i++) {

                value = get_m(m1, i, j) - get_m(broadcast_m, i, j);
                set_m(res, i, j, value);
            }
        }
        free(broadcast_m);
        broadcast_m = NULL;

        return;
    }
    else {
        error("\n*** Impossible to broadcast (function : sub_m) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }
}

void PMatrix_add_scalar(PMatrix m, double alpha, PMatrix res) {

    if (m == NULL || res == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : PMatrix_add_scalar) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    res->tab = add_scalar(m->tab, (m->width * m->height), alpha);

    return;
}

void* mult_vectors(void* args) {

    int current_col = ((PArgs)args)->col;
    int current_row = ((PArgs)args)->row;

    double value = 0.0f;
    double m1_fk = 0.0f;
    double m2_fk = 0.0f;

    PMatrix m1 = ((PArgs)args)->m1;
    PMatrix m2 = ((PArgs)args)->m2;
    PMatrix res = ((PArgs)args)->res;

    for (int j = 0; j < m1->width; j++) {

        m1_fk = get_m(m1, current_row, j);
        m2_fk = get_m(m2, j, current_col);

        value += m1_fk * m2_fk;
    }
    set_m(res, current_row, current_col, value);

    pthread_exit(NULL);
    return (void*)res;
}

PMatrix mult_async(PMatrix m1, PMatrix m2) {

    if (m1 == NULL || m2 == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : mult_async) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (m1->width != m2->height) {
        error("\n*** Incompatible dimensions for matrix multiplication (function : mult_async) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }
    else {
        int linear_index = 0;

        int dim = (int)(m1->height * m2->width);

        PMatrix res = create_matrix(m2->width, m1->height, 'R');

        pthread_t* thid = (pthread_t*)malloc(sizeof(pthread_t) * dim);
        if (thid == NULL) {
            error("\n*** allocation error (function : mult_async) ***\n", ALLOCATION_ERROR);
            exit(INT_MIN);
        }

        for (int i = 0; i < m1->height; i++) {
            for (int j = 0; j < m2->width; j++) {

                PArgs args = (PArgs)malloc(sizeof(Args));
                if (args == NULL) {
                    error("\n*** allocation error (function : mult_async) ***\n", ALLOCATION_ERROR);
                    exit(INT_MIN);
                }
                args->row = i;
                args->col = j;
                args->m1 = m1;
                args->m2 = m2;
                args->res = res;

                pthread_create(&thid[linear_index], NULL, mult_vectors, (void*)args);
                linear_index++;
            }
        }

        for (int j = 0; j < dim; j++) {
            pthread_join(thid[j], NULL);
        }
        free(thid);

        return res;
    }
}

void mult(PMatrix m1, PMatrix m2, PMatrix res) {

    if (m1 == NULL || m2 == NULL || res == NULL) {
        error("\n*** Matrix pointing on NULL was given (function : mult) ***_n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
    else if (m1->width != m2->height) {
        error("\n*** Incompatible dimensions for matrix multiplication (function : mult) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }
    else {

        double value = 0.f;
        double m1_ik = 0.f;
        double m2_kj = 0.f;

        for (int i = 0; i < m1->height; i++) {
            for (int j = 0; j < m2->width; j++) {
                value = 0;
                for (int k = 0; k < m1->width; k++) {

                    m1_ik = get_m(m1, i, k);
                    m2_kj = get_m(m2, k, j);

                    value += m1_ik * m2_kj;
                }
                set_m(res, i, j, value);
            }
        }
        return;
    }
}

void hadamard_product(PMatrix m1, PMatrix m2, PMatrix res) {

    if (m1 == NULL || m2 == NULL || res == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : mult_pointwise) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
    else if (m1->width != m2->width || m1->height != m2->height) {
        error("\n*** Impossible operation dimensions problems (function : mult_pointwise) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }

    double value = 0.f;

    for (int i = 0; i < m1->height; i++){
        for (int j = 0; j < m1->width; j++) {

            value = get_m(m1, i, j) * get_m(m2, i, j);
            set_m(res, i, j, value);
        }
    }
    return;
}

void kronecker_product(PMatrix m1, PMatrix m2, PMatrix res) {

    if (m1 == NULL || m2 == NULL || res == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : mult_pointwise) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int res_width = m1->width * m2->width;
    int res_height = m1->height * m2->height;

    int row_index = 0;
    int col_index = 0;

    double m1_ij = 0.f;
    double value = 0.f;
  
    PMatrix m1_ij_m2 = create_matrix(m2->width, m2->height, 0);

    for (int i = 0; i < m1->height; i++) {
        for (int j = 0; j < m1->width; j++) {

            m1_ij = get_m(m1, i, j);
            mult_by_scalar(m2, m1_ij, m1_ij_m2);

            for (int k = 0; k < m1_ij_m2->height; k++) {
                for (int n = 0; n < m1_ij_m2->width; n++) {

                    value = get_m(m1_ij_m2, k, n);
                    set_m(res, i * m2->height + k, j * m1->width + n, value);

                }
            }
        }
    }
    free(m1_ij_m2);
    m1_ij_m2 = NULL;

    return;
}

void* mult_scalar_vectors(void* args) {

    int current_row = ((PArgs)args)->row;

    double scalar = ((PArgs)args)->scalar;
    double value = 0.0f;

    PMatrix m1 = ((PArgs)args)->m1;
    PMatrix res = ((PArgs)args)->res;

    for (int j = 0; j < res->height; j++) {
        value = scalar * get_m(m1, current_row, j);
        set_m(res, current_row, j, value);
    }

    pthread_exit(NULL);
    return (void*)res;
}

PMatrix mult_by_scalar_async(PMatrix m, double scalar) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : mult_by_scalar_async) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
    PMatrix res = create_matrix(m->width, m->height, 0);

    pthread_t* thid = (pthread_t*)malloc(sizeof(pthread_t) * m->height);
    if (thid == NULL) {
        error("\n*** allocation error (function : mult_by_scalar_async) ***\n", ALLOCATION_ERROR);
        exit(INT_MIN);
    }

    for (int i = 0; i < m->height; i++) {

        PArgs args = (PArgs)malloc(sizeof(Args));
        if (args == NULL) {
            error("\n*** allocation error (function : mult_by_scalar_async) ***\n", ALLOCATION_ERROR);
            exit(INT_MIN);
        }
        args->row = i;
        args->scalar = scalar;
        args->m1 = m;
        args->res = res;

        pthread_create(&thid[i], NULL, mult_scalar_vectors, (void*)args);
    }

    for (int j = 0; j < m->height; j++) {
        pthread_join(thid[j], NULL);
    }
    free(thid);

    return res;
}

void mult_by_scalar(PMatrix m, double scalar, PMatrix res) {

    if (m == NULL || res == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : mult_by_scalar) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    double m_ij = 0.f;

    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            
            m_ij = get_m(m, i, j);
            set_m(res, i, j, scalar * m_ij);
        }
    }

    return;
}

PMatrix broadcast(PMatrix m1, PMatrix m2, int* broadcasted_matrix) {
    
    if (m1 == NULL || m2 == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : broadcast) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if ( (m1->width != m2->width || m1->height != m2->height) 
      && (m1->width != 1 && m2->width != 1 && m1->height != 1 && m2->height != 1) ){
        error("\n*** Impossible to bradocast (function : broadcast) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }

    PMatrix broadcast_m = NULL;
    PMatrix m_copy = NULL;

    if (is_scalar(m1)) {

        broadcast_m = create_matrix(m2->width, m2->height, 0);
        
        int length = broadcast_m->width * broadcast_m->height;
        for (int i = 0; i < length; i++){
            broadcast_m->tab[i] = m1->tab[0];
        }

        *broadcasted_matrix = 1;
        return broadcast_m;
    }
    if (is_scalar(m2)) {

        broadcast_m = create_matrix(m1->width, m1->height, 0);

        int length = broadcast_m->width * broadcast_m->height;
        for (int i = 0; i < length; i++) {
            broadcast_m->tab[i] = m2->tab[0];
        }

        *broadcasted_matrix = 2;
        return broadcast_m;
    }

    else if (m1->width == m2->width && m1->height == m2->height) {
        broadcast_m = matrix_copy(m2);
        *broadcasted_matrix = 2;
        return broadcast_m;
    }

    else if (m1->width != m2->width && m1->height == m2->height) {

        if (m1->width == 1) {

            broadcast_m = create_matrix(m2->width, max(m1->height, m2->height), 0);

            PMatrix* column_matrixes = (PMatrix*)malloc(m2->width * sizeof(PMatrix));
            if (column_matrixes == NULL) {
                error("\n*** allocation error (function : broadcast) ***\n", ALLOCATION_ERROR);
                exit(INT_MIN);
            }

            for (int i = 0; i < m2->width; i++) {
                m_copy = matrix_copy(m1);
                column_matrixes[i] = m_copy;
            }
            broadcast_m = insert_columns(column_matrixes, broadcast_m->height, broadcast_m->width);

            free(m_copy);
            m_copy = NULL;

            free(column_matrixes);
            column_matrixes = NULL;

            *broadcasted_matrix = 1;
            return broadcast_m;
        }

        else if (m2->width == 1) {

            broadcast_m = create_matrix(m1->width, max(m1->height, m2->height), 0);

            PMatrix* column_matrixes = (PMatrix*)malloc(m1->width * sizeof(PMatrix));
            if (column_matrixes == NULL) {
                error("\n*** allocation error (function : broadcast) ***\n", ALLOCATION_ERROR);
                exit(INT_MIN);
            }

            for (int i = 0; i < m1->width; i++) {
                m_copy = matrix_copy(m2);
                column_matrixes[i] = m_copy;
            }
            broadcast_m = insert_columns(column_matrixes, broadcast_m->height, broadcast_m->width);

            free(m_copy);
            m_copy = NULL;

            free(column_matrixes);
            column_matrixes = NULL;

            *broadcasted_matrix = 2;
            return broadcast_m;
        }

        else {
            error("\n*** Impossible to bradocast (function : broadcast) ***\n", DIM_ERROR);
            exit(INT_MIN);
        }
    }

    else if (m1->width == m2->width && m1->height != m2->height) {

        if (m1->height == 1) {

            broadcast_m = create_matrix(max(m1->width, m2->width), m2->height, 0);

            PMatrix* row_matrixes = (PMatrix*)malloc(m2->height * sizeof(PMatrix));
            if (row_matrixes == NULL) {
                error("\n*** allocation error (function : broadcast) ***\n", ALLOCATION_ERROR);
                exit(INT_MIN);
            }

            for (int i = 0; i < m2->height; i++){
                m_copy = matrix_copy(m1);
                row_matrixes[i] = m_copy;
            }
            broadcast_m = insert_rows(row_matrixes, broadcast_m->width, broadcast_m->height);

            free(m_copy);
            m_copy = NULL;

            free(row_matrixes);
            row_matrixes = NULL;

            *broadcasted_matrix = 1;
            return broadcast_m;
        }

        else if (m2->height == 1) {

            broadcast_m = create_matrix(max(m1->width, m2->width), m1->height, 0);

            PMatrix* row_matrixes = (PMatrix*)malloc(m1->height * sizeof(PMatrix));
            if (row_matrixes == NULL) {
                error("\n*** allocation error (function : broadcast) ***\n", ALLOCATION_ERROR);
                exit(INT_MIN);
            }

            for (int i = 0; i < m1->height; i++){
                m_copy = matrix_copy(m2);
                row_matrixes[i] = m_copy;
            }
            broadcast_m = insert_rows(row_matrixes, broadcast_m->width, broadcast_m->height);
            
            free(m_copy);
            m_copy = NULL;

            free(row_matrixes);
            row_matrixes = NULL;

            *broadcasted_matrix = 2;
            return broadcast_m;
        }

        else {
            error("\n*** Impossible to bradocast (function : broadcast) ***\n", DIM_ERROR);
            exit(INT_MIN);
        }
    }

    else {
        error("\n*** Impossible to bradocast (function : broadcast) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }
}

void column_sum(PMatrix m, PMatrix column_sum_m) {

    if (m == NULL || column_sum_m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : column_sum) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    double column_sum = 0.f;

    for (int i = 0; i < m->width; i++){
        column_sum = 0.f;
        for (int j = 0; j < m->height; j++) {
            column_sum += get_m(m, j, i);
        }
        set_m(column_sum_m, 0, i, column_sum);
    }
    return;
}

double sum(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : sum) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    double sum = 0.f;

    for (int i = 0; i < m->width * m->height; i++){
        sum += m->tab[i];
    }
    return sum;
}

PMatrix get_standard_basis_vector(int index, int dim) {

    PMatrix e_i = create_matrix(1, dim, 'e');
    set_m(e_i, index, 0, 1.f);

    return e_i;
}

double euclidian_norm(PMatrix v) {
    
    if (v == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : euclidian_norm) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (v->width == 1) {

        double sum = 0.0f;
        double v_norm = 0.0f;

        for (int i = 0; i < v->height; i++) {
            sum += pow(fabs(v->tab[i]), 2);
        }
        v_norm = sqrt(sum);

        return v_norm;
    }
    else {
        error("\n*** Try to compute the norm of a non vector object (function : euclidian_norm) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
}

double det(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : det) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    double determinant = 0;

    if (!is_square(m)) {
        error("\n*** Square matrix's determinant do not exits (function : det) ***\n", EXIST_ERROR);
        exit(INT_MIN);
    }
    else if (m->width == 1 || m->height == 1) {

        determinant = m->tab[0];
        return determinant;
    }
    else {

        PMatrix m_ = NULL;

        for (int i = 0; i < m->width; i++) {

            m_ = create_matrix(m->width - 1, m->width - 1, 0);

            int x = 0;
            int y = 0;

            for (int j = 1; j < m->height; j++) {
                for (int k = 0; k < m->width; k++) {
                    if (i != k) {
                        set_m(m_, y, x, get_m(m, k, j));
                        y++;
                    }
                }
                y = 0;
                x++;
            }
            determinant += (pow((-1), (i)) * get_m(m, i, 0)) * det(m_);

            free(m_);
            m_ = NULL;
        }

        return determinant;
    }
}

PMatrix get_sub_matrix(PMatrix m, int row , int col) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : get_sub_matrix) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (!verify_range_index(m->width, m->height, row, col)) {
        error("\n*** Segmentation fault : (row, col) out of range (function : get_sub_matrix) ***\n", SEG_FAULT);
        exit(INT_MIN);
    }

    PMatrix sub_m = create_matrix(m->width - 1, m->height - 1, 0);

    int x = 0;
    int y = 0;

    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            if (i != row && j != col) {

                set_m(sub_m, x, y, get(m->tab, m->width, m->height, i, j));

                if (x == sub_m->width - 1 && y == sub_m->width - 1) {
                    x = 0;
                    y = 0;
                }
                else if (x == 0 && y == 0) {
                    y++;
                }
                else if (x < sub_m->width - 1 && y == sub_m->width - 1) {
                    x++;
                    y = 0;
                }
                else if ((x == sub_m->width - 1 && y < sub_m->width - 1)) {
                    y++;
                }
                else if (x < sub_m->width - 1 && y < sub_m->height - 1) {
                    y++;
                }
            }
        }
    }
    return sub_m;
}

PMatrix com(PMatrix m, PMatrix com_m, int index) {

    if (m == NULL || com_m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : com) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int col = 0;
    int row = 0;
    int sign = 0;

    double value = 0.0f;

    if (!is_square(m)) {
        error("\n*** Not square matrix's comatrix do not exists (function : com) ***\n", EXIST_ERROR);
        exit(INT_MIN);
    }
    if (com_m->width != m->width) {
        error("\n*** com_m and m have different dimmensions (function : com) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }
    
    if (index == (m->width * m->height)-1) {   
        get_bi_dimensional_index(index, m->width, m->height, &row, &col);
        sign = row + col;

        value = pow((-1), sign) * det(get_sub_matrix(m, row, col));
        set_m(com_m, row, col, value);

        return com_m;
    }
    else {
        get_bi_dimensional_index(index, m->width, m->height, &row, &col);
        sign = row + col;

        value = pow((-1), sign) * det(get_sub_matrix(m, row, col));
        set_m(com_m, row, col, value);

        return(com(m, com_m, index + 1));
    }
}

void transpose(PMatrix m, PMatrix transpose_m) {

    if (m == NULL || transpose_m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : transpose) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    // PMatrix transpose_m = create_matrix(m->height, m->width, 0);

    for (int i = 0; i < transpose_m->width; i++) {
        for (int j = 0; j < transpose_m->height; j++) {
            set_m(transpose_m, j , i, get_m(m, i , j));
        }
    }
    return;
}

PMatrix inverse(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : inverse) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (!is_square(m)) {
        error("\n*** Not square matrix's inverse do not exists (function : inverse) ***\n", EXIST_ERROR);
        exit(INT_MIN);
    }

    double det_m = (det(m));
    double r = 0;

    PMatrix com_m = NULL;
    PMatrix transpose_com_m = NULL;
    PMatrix m_ = NULL;

    if (det_m != 0) {
        
        r = (1 / (det_m));

        com_m = create_matrix(m->width, m->height, 0);
        com_m = com(m, com_m, 0);

        transpose_com_m = create_matrix(com_m->height, com_m->width, 0);
        transpose(com_m, transpose_com_m);

        m_ = create_matrix(transpose_com_m->width, transpose_com_m->height, 0);

        mult_by_scalar(transpose_com_m, r, m_);

        free(com_m);
        com_m = NULL;
        
        free(transpose_com_m);
        transpose_com_m = NULL;

        return m_;
    }
    else{
        error("\n*** This matrix is not inversible (det = 0) (function : inverse) ***\n", EXIST_ERROR);
        exit(INT_MIN);
    }
}

bool is_column(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : is_column) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (m->width == 1){
        return true;
    }
    return false;
}

bool is_row(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : is_row) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (m->height == 1){
        return true;
    }
    return false;
}

PMatrix* get_column_vectors(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : get_column_vectors) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    PMatrix* column_vectors = malloc(m->width * sizeof(PMatrix));
    if (column_vectors == NULL) {
        error("\n*** allocation error (function : get_column_vectors) ***\n", ALLOCATION_ERROR);
        exit(INT_MIN);
    }

    PMatrix v_i = NULL;

    for (int i = 0; i < m->width; i++) {

        v_i = create_matrix(1, m->height, 0);

        for (int j = 0; j < m->height; j++) {
            set_m(v_i, j, 0, get_m(m, j, i));
        }
        column_vectors[i] = v_i;
    }

    return column_vectors;
}

PMatrix* get_column_range(PMatrix m, int start_index, int end_index) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : get_column_range) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if ((end_index - start_index) < m->width &&
        (start_index >= 0 && start_index < m->width) &&
        (end_index >= 0 && end_index <= m->width) ) {

        int range = end_index - start_index;

        PMatrix m_copy = NULL;
        PMatrix* column_vectors = get_column_vectors(m);

        PMatrix* range_column_vectors = (PMatrix*)malloc(range * sizeof(PMatrix));
        if (range_column_vectors == NULL) {
            error("\n*** allocation error (function : get_column_range) ***\n", ALLOCATION_ERROR);
            exit(INT_MIN);
        }

        for (int i = 0; i < range; i++) {
            m_copy = matrix_copy(column_vectors[start_index + i]);
            range_column_vectors[i] = m_copy;
        }

        for (int i = 0; i < m->width; i++){
            free(column_vectors[i]);
            column_vectors[i] = NULL;
        }
        free(column_vectors);
        column_vectors = NULL;

        return range_column_vectors;
    }
    else {
        error("\n*** Segmentation fault : index out of range (function : get_column_range) ***\n", SEG_FAULT);
        exit(INT_MIN);
    }
}

PMatrix get_column(PMatrix m, int column_index) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : get_column) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (column_index < m->width && column_index >= 0) {
        PMatrix* column_vectors = get_column_vectors(m);
        PMatrix column = column_vectors[column_index];

        for (int i = 0; i < m->width; i++) {
            free(column_vectors[i]);
            column_vectors[i] = NULL;
        }

        free(column_vectors);
        column_vectors = NULL;

        return column;
    }
    else {
        error("\n*** Segmentation fault : index out of range (function : get_column) ***\n", SEG_FAULT);
        exit(INT_MIN);
    }
}

PMatrix* get_row_vectors(PMatrix m) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : get_row_vectors) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    PMatrix* row_vectors = malloc(m->height * sizeof(PMatrix));
    if (row_vectors == NULL) {
        error("\n*** allocation error (function : get_row_vectors) ***\n", ALLOCATION_ERROR);
        exit(INT_MIN);
    }

    PMatrix v_i = NULL;

    for (int i = 0; i < m->height; i++) {

         v_i = create_matrix(m->width, 1, 0);

        for (int j = 0; j < m->width; j++) {
            set_m(v_i, 0, j, get_m(m, i, j));
        }
        row_vectors[i] = v_i;
    }

    return row_vectors;
}

PMatrix* get_row_range(PMatrix m, int start_index, int end_index) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : get_row_range) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int range = end_index - start_index;
    int counter = 0;

    if (range < m->height) {

        PMatrix* row_vectors = get_row_vectors(m);

        PMatrix* range_row_vectors = malloc(range * sizeof(PMatrix));
        if (range_row_vectors == NULL) {
            error("\n*** allocation error (function : get_row_range) ***\n", ALLOCATION_ERROR);
            exit(INT_MIN);
        }

        for (int i = start_index; i < end_index; i++) {
            range_row_vectors[counter] = row_vectors[i];
            counter++;
        }

        for (int i = 0; i < m->height; i++) {
            free(row_vectors[i]);
            row_vectors[i] = NULL;
        }
        free(row_vectors);
        row_vectors = NULL;

        return range_row_vectors;
    }
    else {
        error("\n*** Segmentation fault : index out of range (function : get_row_range) ***\n", SEG_FAULT);
        exit(INT_MIN);
    }
}

void get_row(PMatrix m, int row_index, PMatrix row_i) {

    if (m == NULL || row_i == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : get_row) ***", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (!is_row(row_i)) {
        error("\n*** Wrong dimensions for row_i (function : get_row) ***", DIM_ERROR);
        exit(INT_MIN);
    }
    else if (row_i->width != m->width){
        error("\n*** Width of row_i is incorrect (function : get_row) ***", DIM_ERROR);
        exit(INT_MIN);
    }

    if (row_index < m->height) {

        PMatrix* row_vectors = get_row_vectors(m);
        
        for (int i = 0; i < m->width; i++){
            row_i->tab[i] = row_vectors[row_index]->tab[i];
        } 

        for (int i = 0; i < m->height; i++) {
            free(row_vectors[i]);
            row_vectors[i] = NULL;
        }
        free(row_vectors);
        row_vectors = NULL;

        return;
    }
    else {
        error("\n*** Segmentation fault : index out of range (function : get_row) ***\n", SEG_FAULT);
        exit(INT_MIN);
    }
}

PMatrix insert_rows(PMatrix* row_matrixes, int width, int length) {
    
    if (row_matrixes == NULL) {
        error("\n*** NULL pointer was given (function : insert_rows) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    double value = 0.f;

    PMatrix new_m = create_matrix(width, length, 0);

    for (int i = 0; i < length; i++) {
        if (!is_row(row_matrixes[i]) || !row_matrixes[i]->width > width) {
            error("\n*** row_matrixes[%d] is not a row / or out of width (function : insert_rows) ***\n", SEG_FAULT); 
            exit(INT_MIN);
        }
    }

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            value = row_matrixes[i]->tab[j];
            set_m(new_m, i, j, value);
        }
    }

    return new_m;
}

PMatrix insert_columns(PMatrix* column_matrixes, int height, int length) {

    if (column_matrixes == NULL) {
        error("\n*** NULL pointer was given (function : column_rows) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    double value = 0.f;

    PMatrix new_m = create_matrix(length, height, 0);

    for (int i = 0; i < length; i++) {
        if (!is_column(column_matrixes[i]) || !column_matrixes[i]->height > height) {
            error("\n*** column_matrixes[%d] is not a column / or out of height (function : column_rows) ***\n", SEG_FAULT);
            exit(INT_MIN);
        }
    }

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < height; j++) {
            value = column_matrixes[i]->tab[j];
            set_m(new_m, j, i, value);
        }
    }

    return new_m;
}

PMatrix truncate_matrix(PMatrix m, int row, int col, int sens) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : truncate_m) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
    if (!verify_range_index(m->width, m->height, row, col)) {
        error("\n*** Impossible to truncate : index out of range (function : truncate_m) ***\n", DIM_ERROR);
        exit(INT_MIN);
    }

    PMatrix truncate_m = NULL;
    
    if (sens < 0) {

        truncate_m = create_matrix(m->width - 1, m->height - 1, 0);

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                set_m(truncate_m, i, j, get_m(m, i, j));
            }
        }
        for (int i = 0; i < row; i++) {
            for (int j = col + 1; j < m->width; j++) {
                set_m(truncate_m, i, j - 1, get_m(m, i, j));
            }
        }
        for (int i = row + 1; i < m->height; i++) {
            for (int j = 0; j < col; j++) {
                set_m(truncate_m, i - 1, j, get_m(m, i, j));
            }
        }
        for (int i = row + 1; i < m->height; i++) {
            for (int j = col + 1; j < m->width; j++) {
                set_m(truncate_m, i - 1, j - 1, get_m(m, i, j));
            }
        }

        return truncate_m;
    }
    else if(sens == 0){

        if (col == 0) {

            truncate_m = create_matrix(m->width, m->height - (row+1), 0);

            for (int i = row+1; i < m->height; i++) {
                for (int j = 0; j < m->width; j++) {
                    set_m(truncate_m, i - (row+1), j, get_m(m, i, j));
                }
            }
            return truncate_m;

        }
        else if (row == 0){

            truncate_m = create_matrix(m->width - (col+1), m->height, 0);

            for (int i = 0; i < m->height; i++) {
                for (int j = col+1; j < m->width; j++) {
                    set_m(truncate_m, i, j - (col+1), get_m(m, i, j));
                }
            }
            return truncate_m;
        }
        else {
            error("\n*** Args are not interpretable (function : truncate_matrix) ***\n", NULL_POINTER_ERROR);
            exit(INT_MIN);
        }
    }
    else if (sens == 1) {

        if (col == 0) {

            truncate_m = create_matrix(m->width, row, 0);

            for (int i = 0; i < row; i++) {
                for (int j = 0; j < m->width; j++) {
                    set_m(truncate_m, i, j, get_m(m, i, j));
                }
            }
            return truncate_m;

        }
        else if (row == 0) {

            truncate_m = create_matrix(col, m->height, 0);

            for (int i = 0; i < m->height; i++) {
                for (int j = 0; j < col; j++) {
                    set_m(truncate_m, i, j, get_m(m, i, j));
                }
            }
            return truncate_m;
        }
        else {
            error("\n*** Args are not interpretable (function : truncate_matrix) ***\n", NULL_POINTER_ERROR);
            exit(INT_MIN);
        }
    }
    else {
        error("\n*** Args are not interpretable (function : truncate_matrix) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }
}

PMatrix shuffle_rows(PMatrix m) {
        
    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : shuffle_rows) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int random_index = 0;
    int shuffled_index = 0;

    PMatrix m_temp = NULL;
    PMatrix shuffled_matrix = NULL;
    
    PMatrix* m_rows = NULL;

    shuffled_matrix = create_matrix(m->width, m->height, 0);
    fill_matrix(shuffled_matrix, m->tab, (m->width * m->height));

    m_rows = get_row_vectors(m);

    srand((unsigned int)time(NULL));

    for (int i = 0; i < m->height; i++){

        random_index = rand() % (int)m->height;

        m_temp = m_rows[random_index];
        m_rows[random_index] = m_rows[i];
        m_rows[i] = m_temp;
    }
    free(m_temp);
    m_temp = NULL;

    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            shuffled_matrix->tab[shuffled_index] = m_rows[i]->tab[j];
            shuffled_index++;
        }
    }

    free(m_rows);
    m_rows = NULL;

    return shuffled_matrix;
}

void flatten(PMatrix m, PMatrix flatten_m) {
    
    if (m == NULL || flatten_m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : flatten) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    if (m->height == 1) {
        for (int i = 0; i < m->width; i++){
            flatten_m->tab[i] = m->tab[i];
        }
        return;
    }
    else if (m->width == 1) {
        transpose(m, flatten_m);
        return;
    }
    else {
        
        int count = 0;
        double value = 0.f;

        PMatrix* m_rows = get_row_vectors(m);

        for (int i = 0; i < m->height; i++){
            for (int j = 0; j < m->width; j++){
                value = m_rows[i]->tab[j];
                flatten_m->tab[count] = value;
                count++;
            }
        }

        for (int i = 0; i < m->height; i++){
            free(m_rows[i]);
            m_rows[i] = NULL;
        }
        free(m_rows);
        m_rows = NULL;

        return;
    }
}

PMatrix maximum(PMatrix m1, PMatrix m2) {

    if (m1 == NULL || m2 == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : maximum) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int max_width = max(m1->width, m2->width);
    int max_height = max(m1->height, m2->height);
    int min_width = min(m1->width, m2->width);
    int min_height = min(m1->height, m2->height);

    double x1_ij = 0.f;
    double x2_ij = 0.f;

    PMatrix max = create_matrix(max_width, max_height, 0);

    for (int i = 0; i < min_height; i++) {
        for (int j = 0; j < min_width; j++) {

            x1_ij = get_m(m1, i, j);
            x2_ij = get_m(m2, i, j);

            if (x1_ij < x2_ij) {
                set_m(max, i, j, x2_ij);
            }
            else {
                set_m(max, i, j, x1_ij);
            }
        }
    }
    return max;
}

void matrix_exp(PMatrix m, PMatrix m_exp) {

    if (m == NULL || m_exp == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : matrix_exp) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    for (int i = 0; i < m->width * m->height; i++){
        m_exp->tab[i] = exp(m->tab[i]);
    }
    return;
}

PMatrix boolean_matrix(PMatrix m, PMatrix prediction) {

    if (m == NULL || prediction == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : boolean_matrix) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int length = min((m->width * m->height), (prediction->width * prediction->height)) ;
    double epsi = 0.1f;

    PMatrix boolean_m = create_matrix(m->width, m->height, 0);

    for (int i = 0; i < length; i++){
        if (m->tab[i] > prediction->tab[i] - epsi && m->tab[i] < prediction->tab[i] + epsi) {
            boolean_m->tab[i] = 1.f;
        }
    }
    return boolean_m;
}

PMatrix arg_max(PMatrix m, int axis) {

    if (m == NULL) {
        error("\n*** Matrix pointng on NULL was given (function : arg_max) ***\n", NULL_POINTER_ERROR);
        exit(INT_MIN);
    }

    int max_index = 0;

    double value = 0.f;
    double max = FLT_MIN;

    PMatrix max_axis = NULL;

    if (axis == 0) {

        max_axis = create_matrix(m->width, 1, 0);

        for (int i = 0; i < m->width; i++){
            max_index = 0;
            max = FLT_MIN;
            for (int j = 0; j < m->height; j++){
                value = get_m(m, j, i);
                if (value > max) {
                    max = value;
                    max_index = j;
                }
            }
            set_m(max_axis, 0, i, max_index);
        }
    }
    else if (axis == 1) {

        max_axis = create_matrix(1, m->height, 0);

        for (int i = 0; i < m->height; i++) {
            for (int j = 0; j < m->width; j++) {

                value = get_m(m, i, j);
                if (value > max) {
                    value = max;
                    max_index = j;
                }
            }
            set_m(max_axis, i, 0, max_index);
        }
    }

    return max_axis;
}

////=============== Check if a matrix is traingular ===============//
//bool is_upper_triangular(PMatrix m, float* e) {
//
//    bool upper;
//
//    if (is_square_matrix(m)) {
//
//        if (e == NULL) {
//
//            for (int i = 0; i < m->height; i++) {
//                for (int j = 0; j < m->width; j++) {
//
//                    if (i > j && m.tab[j][i] != 0) {
//                        upper = false;
//                        return upper;
//                    }
//                }
//            }
//            upper = true;
//            return upper;
//        }
//        else {
//            for (int i = 0; i < m->height; i++) {
//                for (int j = 0; j < m->width; j++) {
//
//                    if (i > j && (m.tab[j][i] > *e || m.tab[j][i] < -*e)) {
//                        upper = false;
//                        return upper;
//                    }
//                }
//            }
//            upper = true;
//            return upper;
//        }
//    }
//    else {
//        printf("Une matrice non carré ne peut pas etre triangulaire\n");
//        exit(1);
//    }
//
//}
//bool is_lower_triangular(PMatrix m, float* e) {
//
//    bool lower;
//
//    if (is_square_matrix(m)) {
//
//        if (e == NULL) {
//
//            for (int i = 0; i < m->height; i++) {
//                for (int j = 0; j < m->width; j++) {
//
//                    if (i < j && m.tab[j][i] != 0) {
//                        lower = false;
//                        return lower;
//                    }
//                }
//            }
//            lower = true;
//            return lower;
//        }
//        else {
//            for (int i = 0; i < m->height; i++) {
//                for (int j = 0; j < m->width; j++) {
//
//                    if (i < j && (m.tab[j][i] > *e || m.tab[j][i] < -*e)) {
//                        lower = false;
//                        return lower;
//                    }
//                }
//            }
//            lower = true;
//            return lower;
//        }
//    }
//    else {
//        printf("Une matrice non carré ne peut pas etre triangulaire\n");
//        exit(1);
//    }
//}
//
////=============== Check if a matrix is diagonal ===============//
//bool is_diagonal(PMatrix m) {
//
//    bool diagonal;
//
//    if (is_square_matrix(m)) {
//        for (int i = 0; i < m->height; i++) {
//            for (int j = 0; j < m->width; j++) {
//
//                if ((i < j && m.tab[j][i] != 0) || (i > j && m.tab[j][i] != 0)) {
//                    diagonal = false;
//                    return diagonal;
//                }
//            }
//        }
//        diagonal = true;
//        return diagonal;
//    }
//    else {
//        printf("Une matrice non carré ne peut pas etre diagonale\n");
//        exit(1);
//    }
//
//}
//
////=============== Check if a matrix is symetrical ===============//
//bool is_symetrical(PMatrix m) {
//
//    bool symetrical;
//
//    if (is_square_matrix(m)) {
//
//        for (int i = 0; i < m->height; i++) {
//            for (int j = 0; j < m->width; j++) {
//
//                if (m.tab[j][i] == m.tab[i][j]) {
//                    symetrical = false;
//                    return symetrical;
//                }
//            }
//        }
//
//        symetrical = true;
//        return symetrical;
//    }
//    else {
//        printf("Une matrice non carré ne peut pas etre symetrique\n");
//        exit(1);
//    }
//}
//
////=============== Get is diagonal element of a square matrix ===============//
//float* get_diagonal(PMatrix m) {
//
//    if (is_square_matrix(m)) {
//
//        float* diagonal_element = malloc(m->height * sizeof(float));
//
//        for (int i = 0; i < m->height; i++) {
//            diagonal_element[i] = m.tab[i][i];
//        }
//        return diagonal_element;
//    }
//    else {
//        printf("Une matrice non carré ne ppossede pas de diagonal\n");
//        return NULL;
//    }
//}
//
////=============== Compute the minor matrix from a specific index of the matrix ===============//
//PMatrix minor_matrix(PMatrix m, int col_index, int row_index) {
//
//    if (col_index < 0 || col_index > m->width ||
//        row_index < 0 || row_index > m->height) {
//        printf("Incorrect index\n");
//        exit(1);
//    }
//    else {
//
//        PMatrix minor = create_matrix(m->width - 1, m->height - 1, "");
//
//        int i_ = 0;
//        int j_ = 0;
//
//        for (int i = 0; i < m->height; i++) {
//            for (int j = 0; j < m->width; j++) {
//
//                if (i != row_index && j != col_index) {
//                    minor.tab[j_][i_] = m.tab[j][i];
//                    if (j_ == minor->width - 1) {
//                        i_++;
//                        j_ = 0;
//                    }
//                    else {
//                        j_++;
//                    }
//                }
//            }
//        }
//        return minor;
//    }
//}
//
////=============== Different function for resolve linear system ===============//
//int search_pivot(PMatrix v) {
//
//    if (v->width == 1) {
//
//        float max = -__FLT_MAX__;
//        int row_index = 0;
//
//        for (int i = 0; i < v->height; i++) {
//            if (fabs(v.tab[0][i]) > max) {
//                max = fabs(v.tab[0][i]);
//                row_index = i;
//            }
//        }
//        return row_index;
//    }
//    else {
//        printf("Un pivot se recherche dans une matrice colonne \n");
//        exit(1);
//    }
//}
//PMatrix swap_rows(PMatrix m, int old_row_index, int new_row_index) {
//
//    if (old_row_index < 0 || old_row_index > m->height ||
//        new_row_index < 0 || new_row_index > m->height) {
//        printf("Incorrect index\n");
//        exit(1);
//    }
//    else {
//        PMatrix current_row = create_matrix(m->width, 1, "");
//        PMatrix temp_row = create_matrix(m->width, 1, "");
//
//        for (int i = 0; i < m->width; i++) {
//            current_row.tab[i][0] = m.tab[i][old_row_index];
//            temp_row.tab[i][0] = m.tab[i][new_row_index];
//        }
//
//        for (int j = 0; j < m->width; j++) {
//            m.tab[j][new_row_index] = current_row.tab[j][0];
//        }
//
//        for (int k = 0; k < m->width; k++) {
//            m.tab[k][old_row_index] = temp_row.tab[k][0];
//        }
//
//        return m;
//    }
//}
//PMatrix mult_row_by_scalar(PMatrix m, int row_index, float scalar) {
//
//    if (row_index < 0 || row_index > m->height || scalar == 0) {
//        printf("Incorrect index\n");
//        exit(1);
//    }
//    else {
//
//        for (int i = 0; i < m->width; i++) {
//            m.tab[i][row_index] *= scalar;
//        }
//
//        return m;
//    }
//}
//PMatrix add_linear_combination(PMatrix m, int receiver_row_index, int adding_row_index, float scalar) {
//
//    if (receiver_row_index < 0 || receiver_row_index > m->height ||
//        adding_row_index < 0 || receiver_row_index > m->height) {
//        printf("Incorrect index\n");
//        exit(1);
//    }
//    else {
//
//        for (int i = 0; i < m->width; i++) {
//            m.tab[i][receiver_row_index] += (scalar * m.tab[i][adding_row_index]);
//        }
//        return m;
//    }
//}
//PMatrix gauss_jordan(PMatrix m) {
//
//    int last_pivot_index = 0;
//    int current_pivot_index = 0;
//
//    float pivot;
//
//    PMatrix* column_vectors = get_column_vectors_from(m);
//    PMatrix m_ = m;
//    PMatrix before_minor = m;
//    PMatrix v_i;
//
//    // printf("m_ = \n");
//    // print_matrice(m_);
//    // printf("\n");
//
//    for (int j = 0; j < m->width - 1; j++) {
//
//        // printf("//===== %d ITERATIONS =====//\n\n" , j);
//
//        before_minor = m_;
//        v_i = column_vectors[0],
//
//            current_pivot_index = search_pivot(v_i);
//        pivot = v_i.tab[0][current_pivot_index];
//
//        current_pivot_index += j;
//
//        // printf("k = %d\n", current_pivot_index);
//        // printf("r = %d\n", last_pivot_index);
//        // printf("j = %d\n", j);
//        // printf("Pivot = %f\n", pivot);
//
//        if (pivot != 0) {
//
//            float ratio = 1 / pivot;
//            m_ = mult_row_by_scalar(m_, current_pivot_index, ratio);
//
//            if (current_pivot_index != last_pivot_index) {
//                m_ = swap_rows(m_, current_pivot_index, last_pivot_index);
//            }
//
//            for (int i = 0; i < m_->height; i++) {
//                if (i != last_pivot_index) {
//                    m_ = add_linear_combination(m_, i, last_pivot_index, -m_.tab[j][i]);
//                }
//            }
//
//            last_pivot_index += 1;
//        }
//
//        // printf("m_ = \n");
//        // print_matrice(m_);
//
//        if (j != m->height) {
//            for (int i = 0; i <= j; i++) {
//                before_minor = minor_matrix(before_minor, 0, 0);
//            }
//        }
//
//        // printf("before_minor = \n");
//        // print_matrice(before_minor);
//
//        column_vectors = NULL;
//        column_vectors = get_column_vectors_from(before_minor);
//    }
//
//    // printf("\n\nFIN : m_ = \n");
//    // print_matrice(m_);
//
//    return m_;
//}
//
//PMatrix resolve_linear_system(PMatrix A, PMatrix B) {
//
//    if (B->width == 1 && B->height == A->height) {
//
//        PMatrix X = create_matrix(1, A->height, "");
//        PMatrix AB = create_matrix(A->width + 1, A->height, "");
//        PMatrix resolve;
//
//        for (int i = 0; i < A->height; i++) {
//            for (int j = 0; j < A->width; j++) {
//                AB.tab[j][i] = A.tab[j][i];
//            }
//        }
//        for (int k = 0; k < B->height; k++) {
//            AB.tab[A->width][k] = B.tab[0][k];
//        }
//
//        // printf("AB = \n");
//        // print_matrice(AB);
//        // printf("\n");
//
//        resolve = gauss_jordan(AB);
//        // printf("resolve = \n");
//        // print_matrice(resolve);
//        // printf("\n");
//
//        X = get_column_vectors_from(resolve)[A->width];
//
//        return X;
//    }
//    else {
//        printf("Le systeme n'est pas correct\n");
//        exit(1);
//    }
//}
//
////=============== Make an Householder matrix with a default matrix ===============//
//PMatrix make_householder_matrix(PMatrix m, int diff) {
//
//    if (diff > 0) {
//
//        PMatrix householder_matrix = null_matrix(m->width + diff, m->height + diff);
//        for (int i = 0; i < diff; i++) {
//            householder_matrix.tab[i][i] = 1;
//        }
//
//        for (int i = diff; i < householder_matrix->height; i++) {
//            for (int j = diff; j < householder_matrix->width; j++) {
//                householder_matrix.tab[j][i] = m.tab[j - diff][i - diff];
//            }
//        }
//
//        return householder_matrix;
//    }
//    else {
//        return m;
//    }
//}
//
////=============== Compute the QR decomposition of a matrix  ===============//
//void QR_decomposition(PMatrix m, PMatrix* Q, PMatrix* R) {
//
//    PMatrix* column_vectors;
//
//    PMatrix R_;
//    PMatrix Q_ = get_identity(m->height);
//    PMatrix Q_i;
//    PMatrix m_;
//    PMatrix e1;
//    PMatrix x;
//    PMatrix u;
//    PMatrix v;
//    PMatrix product;
//
//    float a;
//
//    m_ = m;
//
//    for (int i = 0; i < m->height - 1; i++) {
//
//        PMatrix In = get_identity(m_->height);
//
//        column_vectors = get_column_vectors_from(m_);
//
//        x = column_vectors[0];
//        e1 = get_standard_basis_vector(1, m_->height);
//        a = euclidian_norm(x);
//        u = add(x, mult_by_scalar(e1, -a));
//
//        if (!is_null(u)) {
//
//            float r = (float)(1 / euclidian_norm(u));
//            v = mult_by_scalar(u, r);
//
//            product = mult(v, transpose(v));
//            Q_i = add(In, mult_by_scalar(product, -2));
//
//            int diff = m->height - Q_i->height;
//
//            PMatrix new_Q_i = make_householder_matrix(Q_i, diff);
//
//            Q_ = mult(Q_, new_Q_i);
//            R_ = mult(transpose(Q_), m);
//
//            m_ = minor_matrix(R_, 0, 0);
//
//        }
//        else {
//
//            Q_i = In;
//
//            int diff = m->height - Q_i->height;
//
//            PMatrix new_Q_i = make_householder_matrix(Q_i, diff);
//
//            Q_ = mult(Q_, new_Q_i);
//            R_ = mult(transpose(Q_), m);
//
//            m_ = minor_matrix(R_, 0, 0);
//        }
//    }
//
//    *R = R_;
//    *Q = Q_;
//}
//
////=============== Compute the eigen values of a matrix ===============//
//float* eigenvalues(PMatrix m) {
//
//    if (is_square_matrix(m)) {
//
//        int i = 0;
//
//        float* eigen_values = malloc(m->height * sizeof(float));
//
//        float e = 0.05;
//
//        PMatrix m0 = m;
//        PMatrix m_i = m0;
//        PMatrix Q_i;
//        PMatrix R_i;
//
//        do {
//
//            QR_decomposition(m_i, &Q_i, &R_i);
//
//            m_i = mult(R_i, Q_i);
//            i++;
//
//            // printf("m_i = \n");
//            // print_matrice(m_i);
//
//        } while (!is_upper_triangular(m_i, &e));
//
//        eigen_values = get_diagonal(m_i);
//
//        return eigen_values;
//    }
//    else {
//        printf("Une matrice non carré ne possede pas de valeurs propres\n");
//        return NULL;
//    }
//}
//
////=============== Compute the eigen vectors of a matrix ===============//
//PMatrix* eigenvectors(PMatrix m) {
//
//    if (is_square_matrix(m)) {
//
//        int k = 0;
//
//        float* eigen_values = eigenvalues(m);
//        float lambda_i;
//        float previous_lambda = 0;
//        float e = 0.1;
//        float ratio;
//
//        PMatrix* eigenvectors = malloc(m->height * sizeof(PMatrix));
//        PMatrix x0 = create_matrix(1, m->height, "");
//        PMatrix x = x0;
//
//        PMatrix A;
//        PMatrix B;
//
//        PMatrix product;
//
//        for (int i = 0; i < m->height; i++) {
//            x0.tab[0][i] = rand() % 10;
//        }
//        // printf("x0 = \n");
//        // print_matrice(x0);
//
//        // printf("lambda_i = %f \n\n", lambda_i);
//
//        for (int i = 0; i < m->height; i++) {
//
//            lambda_i = eigen_values[i];
//            x = x0;
//            previous_lambda = 0;
//
//            // printf("lambda_i = %f\n", lambda_i); 
//
//            while (k < 50 && fabs(lambda_i - previous_lambda) > e) {
//
//                previous_lambda = lambda_i;
//
//                A = add(m, mult_by_scalar(get_identity(m->height), -lambda_i));
//
//                ratio = (float)(1 / euclidian_norm(x));
//                B = mult_by_scalar(x, ratio);
//
//                x = resolve_linear_system(A, B);
//
//                // printf("X = \n");
//                // print_matrice(x);
//
//                // printf("B = \n");
//                // print_matrice(B);
//
//                product = mult(transpose(x), B);
//                // printf("product = \n");
//                // print_matrice(product);
//
//                lambda_i = (1 / product.tab[0][0]);
//                // printf("lambda_i = %f\n", lambda_i);
//
//                k++;
//            }
//            k = 0;
//            eigenvectors[i] = x;
//            // printf("lambda_i = %f\n\n", lambda_i); 
//        }
//        return eigenvectors;
//    }
//    else {
//        printf("Une matrice non carré ne possede pas d'élément propres !\n");
//        exit(1);
//    }
//}