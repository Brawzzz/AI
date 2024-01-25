//-------------------------- INCLUDES --------------------------//
#include "Data_Process.h"
#include "Neural_Network.h"

//-------------------------- DEFINES & CONSTANT --------------------------//
#define _CRT_SECURE_NO_WARNINGS

const int nb_train_examples = 100;
const char* predictions_file = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/predictions/predictions.csv";

double ACCURACY_MIN = 0.95f;

//-------------------------- MAIN --------------------------//
int main(int argc, char* argv[]) {

    //---------------------------- ARGUMENTS ----------------------------//
    int             ITERATIONS              = 0;
    double          PRECISION               = 0.f;

    char            DATA_FILE_NAME[100]      = { 0 };
    char            PARAMS_FILE_NAME[100]    = { 0 };
        
    bool            TRAIN_MODEL             = FALSE;
    bool            USE_TRAIN_SET           = FALSE;
    bool            TEST_MODEL              = FALSE;

    bool            BOOL_TEST_FILE          = FALSE;
    bool            BOOL_EXAMPLE_FILE       = FALSE;

    int             TEST_INDEX              = 0;
    int             EXAMPLE_INDEX           = 0;

    char            TEST_FILE[100]          = { 0 };
    char            EXAMPLE_FILE[100]       = { 0 };

    //---------------------------- CHECK ARGUMENTS ----------------------------//
    if (argc == 1) {

        fprintf_s(stdout, "\n****** DEFAULT ARGUMENTS SETTING ******\n\n");
        fflush(stdout);

        ITERATIONS          = 450;
        PRECISION           = 0.0025f;

        strcpy_s(DATA_FILE_NAME, 100, "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/set/train.csv");
        strcpy_s(PARAMS_FILE_NAME, 100, "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/models/model_1.3.csv");

        TRAIN_MODEL         = TRUE;
        USE_TRAIN_SET       = TRUE;
        TEST_MODEL          = FALSE;

        BOOL_TEST_FILE      = FALSE;
        BOOL_EXAMPLE_FILE   = TRUE;

        TEST_INDEX          = 18989;
        EXAMPLE_INDEX       = 0;

        strcpy_s(TEST_FILE, 100, "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/set/test.csv");
        strcpy_s(EXAMPLE_FILE, 100, "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/csv/four.csv");
    }
    else {

         for (int i = 1; i < argc; i++) {

            //--------------- ITERATIONS ---------------//
            if (strcmp((const char*)argv[i], "-i") == 0) {
                if (i + 1 < argc) {
                    ITERATIONS = atoi((const char*)argv[i + 1]);
                }
            }

            //--------------- PRECISON ---------------//
            if (strcmp((const char*)argv[i], "-p") == 0) {
                if (i + 1 < argc) {
                    sscanf_s((const char*)argv[i + 1], "%lf", &PRECISION);
                }
            }

            //--------------- DATA FILE ---------------//
            if (strcmp((const char*)argv[i], "-Df") == 0) {
                if (i + 1 < argc) {
                    strcpy_s(DATA_FILE_NAME, 100, (const char*)argv[i + 1]);
                }
            }

            //--------------- PARAMS FILE ---------------//
            if (strcmp((const char*)argv[i], "-Pf") == 0) {
                if (i + 1 < argc) {
                    strcpy_s(PARAMS_FILE_NAME, 100, (const char*)argv[i + 1]);
                }
            }

            //--------------- TRAIN OPTION ---------------//
            if (strcmp((const char*)argv[i], "-tr") == 0) {
                if (i + 1 < argc) {
                    TRAIN_MODEL = atoi((const char*)(argv[i + 1]));
                    USE_TRAIN_SET = TRUE;
                }
            }
            //--------------- TEST OPTION ---------------//
            if (strcmp((const char*)argv[i], "-tst") == 0) {
                if (i + 1 < argc) {
                    TEST_MODEL = atoi((const char*)argv[i + 1]);
                }
            }
            //--------------- TEST FILE ---------------//
            if (strcmp((const char*)argv[i], "-Tf") == 0) {
                if (i + 1 < argc) {
                    BOOL_TEST_FILE = atoi((const char*)argv[i + 1]);
                    strcpy_s(TEST_FILE, 100, (const char*)argv[i + 2]);
                }
            }
            //--------------- TEST INDEX ---------------//
            if (strcmp((const char*)argv[i], "-Ti") == 0) {
                if (i + 1 < argc) {
                    TEST_INDEX = atoi((const char*)argv[i + 1]);
                }
            }
            //--------------- EXAMPLE FILE ---------------//
            if (strcmp((const char*)argv[i], "-Ef") == 0) {
                if (i + 1 < argc) {
                    BOOL_EXAMPLE_FILE = atoi((const char*)argv[i + 1]);
                    strcpy_s(EXAMPLE_FILE, 100, (const char*)argv[i + 2]);
                }
            }
            //--------------- EXAMPLE INDEX ---------------//
            if (strcmp((const char*)argv[i], "-Ei") == 0) {
                if (i + 1 < argc) {
                    EXAMPLE_INDEX = atoi((const char*)argv[i + 1]);
                }
            }
         }
    }
    
    //---------------------------- TRAIN MODEL ----------------------------// 
    if(TRAIN_MODEL){

        int nb_dev_examples = DATASET_HEIGHT - (nb_train_examples + 1);

        PMatrix     data_array          = NULL;

        PMatrix     X_dev               = NULL;
        PMatrix     Y_dev               = NULL;
        PMatrix     X_train             = NULL;
        PMatrix     Y_train             = NULL;

        PParams     p1                  = NULL;
        PParams     p2                  = NULL;

        X_dev = create_matrix(nb_dev_examples, DATASET_WIDTH - 1, 0);
        Y_dev = create_matrix(nb_dev_examples, 1, 0);
        X_train = create_matrix(nb_train_examples, DATASET_WIDTH - 1, 0);
        Y_train = create_matrix(nb_train_examples, 1, 0);

        //-------------------------- Import Datas --------------------------//
        printf("- Importing datas...");
        fflush(stdout);

        data_array = get_train_examples(DATA_FILE_NAME);

        printf("DONE !\n");
        fflush(stdout);

        //-------------------------- Shuffle Datas --------------------------//
        printf("- Shuffle the datas...");
        fflush(stdout);

        data_array = shuffle_rows(data_array);

        printf("DONE !\n");
        fflush(stdout);

        //-------------------------- Spliting Datas Train and Dev set --------------------------//
        printf("- Spliting data in dev and train set...\n");
        fflush(stdout);

        if (USE_TRAIN_SET) {
            train_set(data_array, nb_train_examples, X_train, Y_train);
        }
        else{
            dev_set(data_array, nb_dev_examples, X_dev, Y_dev);
        }

        delete_matrix(data_array);

        printf("DONE !\n");
        fflush(stdout);

        //-------------------------- Gradient Descent --------------------------//
        printf("- Launch gradient descent...\n");
        fflush(stdout);

        if (USE_TRAIN_SET) {
            gradient_descent(X_train, Y_train, PRECISION, ITERATIONS, &p1, &p2); 

            delete_matrix(X_train);
            delete_matrix(Y_train);
        }
        else {
            gradient_descent(X_dev, Y_dev, PRECISION, ITERATIONS, &p1, &p2);

            delete_matrix(X_dev);
            delete_matrix(Y_dev);
        }

        printf("Gradient descent...DONE !\n");
        fflush(stdout);

        save_params(p1, p2, (const char*)PARAMS_FILE_NAME);

        delete_params(p1);
        delete_params(p2);

        return 0;
    }

    //------------------------------- TESTS -------------------------------// 
    if(TEST_MODEL){
        
        int label = 0;

        PMatrix X = NULL;
        PMatrix Y = NULL;

        PMatrix predictions = NULL;

        PParams p1 = create_params(W1_WIDTH, W1_HEIGHT, b1_WIDTH, b1_HEIGHT);
        PParams p2 = create_params(W2_WIDTH, W2_HEIGHT, b2_WIDTH, b2_HEIGHT);
        
        get_params(PARAMS_FILE_NAME, p1, p2);

        if (BOOL_TEST_FILE)
            get_example(TEST_FILE, TEST_INDEX, &X, NULL);

        else if(BOOL_EXAMPLE_FILE)
            get_example(EXAMPLE_FILE, EXAMPLE_INDEX, &X, NULL);

        normalize_data(X, 0.f, 255.f, 0.f, 1.f);

        if (Y != NULL) {
            label = (int)Y->tab[0];
            printf("Label : %d\n", label);
        }

        predictions = make_prediction(X, p1, p2);

        printf("Prediction : \n");
        print_matrix(predictions);
        save_matrix(predictions, predictions_file, NULL);

        delete_params(p1);
        delete_params(p2);

        delete_matrix(predictions);

        return 0;
    }

    return 0;
}

