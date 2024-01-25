import subprocess
import csv
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from random import randrange


def show_number(number_img, prediction):
    number_img = number_img.reshape((28, 28)) * 255
    plt.gray()
    plt.imshow(number_img, interpolation='nearest')
    plt.title(f"Predictions : {prediction}")
    plt.show()

TRUE = "1"
FALSE = "0"

numbers = ((0, "zero"), (1, "one"), (2, "two"), (3, "three"), (4, "four"),
            (5, "five"), (6, "six"), (7, "seven"), (8, "eight"), (9, "nine") )

train_config = False
test_config = not(train_config)
train_test_config = False

if(test_config):
    test = True
    example = not(test)

#---------------------------- IMPORTANT FILES ----------------------------#
exe_path = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/StaticTablesLib/x64/Release/Main.exe"

params_file_name = "model_final.csv"
data_file_name = "train.csv"

test_file_name = "test.csv"
example_file_name = numbers[randrange(10)][1] + ".csv"

#---------------------------- ARGUMENTS BREAKPOINTS ----------------------------#
iteration_bp = "-i"
precision_bp = "-p"

data_file_bp = "-Df"
params_file_bp = "-Pf"
    
train_model_bp = "-tr"
test_model_bp = "-tst"

test_file_bp = "-Tf"
test_index_bp = "-Ti"

example_file_bp = "-Ef"
example_index_bp = "-Ei"

#---------------------------- PARAMS FOR DISPLAY THE NUMBER ----------------------------#
if(test):
    test_set = pd.read_csv('./set/test.csv')
    test_set = np.array(test_set)

    test_index = randrange(len(test_set))
    number_img = test_set[test_index-1]

if(example):
    example_img = pd.read_csv('./examples/csv/' + example_file_name)
    example_img = np.array(example_img)

    len_example = len(example_img)

    for i in range(len_example):
        example_img[i][-1] = 0

    example_index = randrange(len_example)
    number_img = example_img[example_index]

#---------------------------- SET DIFFERENT CONFIG ----------------------------#
if(train_config):
    TRAIN_MODEL = TRUE
    TEST_MODEL = FALSE

    DATA_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/set/" + data_file_name
    PARAMS_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/models/" + params_file_name

    ITERATIONS = 250
    PRECISION = 0.025

    BOOL_TEST_FILE = FALSE
    TEST_FILE_PATH = "0"

    TEST_INDEX = -1
    EXAMPLE_INDEX = -1

    BOOL_EXAMPLE_FILE = FALSE
    EXAMPLE_FILE_PATH = "0"

    print("\n************** Train Config **************\n")
    print(f"Iterations : {ITERATIONS} ; Precision : {PRECISION}\n")
    print(f"Data File Path : {DATA_FILE_PATH}\n")
    print(f"Parameters File Path : {PARAMS_FILE_PATH}\n")

elif(test_config):

    TRAIN_MODEL = FALSE
    TEST_MODEL = TRUE

    DATA_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/set/" + data_file_name
    PARAMS_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/models/" + params_file_name

    ITERATIONS = -1
    PRECISION = -1

    if(test):
        BOOL_TEST_FILE = TRUE
        BOOL_EXAMPLE_FILE = FALSE

        TEST_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/set/" + test_file_name
        EXAMPLE_FILE_PATH = "0"

        TEST_INDEX = test_index
        EXAMPLE_INDEX = -1

    if(example):
        BOOL_TEST_FILE = FALSE
        BOOL_EXAMPLE_FILE = TRUE

        TEST_FILE_PATH = "0"
        EXAMPLE_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/csv/" + example_file_name

        TEST_INDEX = -1
        EXAMPLE_INDEX = example_index

    print("\n************** Test Config **************\n")
    print(f"Parameters File Path : {PARAMS_FILE_PATH}\n")
    print(f"Test Set : {test}")
    print(f"Test Set File Path : {TEST_FILE_PATH}\n") 
    print(f"Example : {example}")
    print(f"Example File Path : {EXAMPLE_FILE_PATH}\n")

elif(train_test_config):
    TRAIN_MODEL = TRUE
    TEST_MODEL = TRUE

    DATA_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/set/" + data_file_name
    PARAMS_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/models/" + params_file_name

    ITERATIONS = 250
    PRECISION = 0.025

    if(test):
        BOOL_TEST_FILE = TRUE
        BOOL_EXAMPLE_FILE = FALSE

        TEST_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/set/" + test_file_name
        EXAMPLE_FILE_PATH = "0" 

        TEST_INDEX = test_index
        EXAMPLE_INDEX = -1

    if(example):
        BOOL_TEST_FILE = FALSE
        BOOL_EXAMPLE_FILE = TRUE

        TEST_FILE_PATH = "0"
        EXAMPLE_FILE_PATH = "C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/csv/" + example_file_name

        TEST_INDEX = -1
        EXAMPLE_INDEX = example_index

    print("\n************** Train & Test Config **************\n")
    print(f"Iterations : {ITERATIONS} ; Precision : {PRECISION}\n")
    print(f"Data File Path : {DATA_FILE_PATH}\n")
    print(f"Parameters File Path : {PARAMS_FILE_PATH}\n")
    print(f"Test Set : {test} ; Example : {example}\n")
    print(f"Test Set File Path : {TEST_FILE_PATH} ; Example File Path : {EXAMPLE_FILE_PATH}\n")


#---------------------------- LAUNCH EXE WITH CORRECT CONFIG ----------------------------#
try:
    result_c = subprocess.run([exe_path , train_model_bp, TRAIN_MODEL, test_model_bp, TEST_MODEL, 
                                data_file_bp, DATA_FILE_PATH, params_file_bp, PARAMS_FILE_PATH,
                                iteration_bp, str(ITERATIONS), precision_bp, str(PRECISION),
                                test_file_bp, BOOL_TEST_FILE, TEST_FILE_PATH, test_index_bp, str(TEST_INDEX),
                                example_file_bp, BOOL_EXAMPLE_FILE, EXAMPLE_FILE_PATH, example_index_bp, str(EXAMPLE_INDEX)],
                                check=True, text=True, capture_output=True)
        
    if result_c.returncode != 0:
        print("\n********** C SCRIPT ERROR **********\n")
        print(result_c.stderr)
    else:
        print(result_c.stdout)

except FileNotFoundError:
    print("\n********** FILE NOT FOUND **********\n")


if(test_config):

    prediction = []

    with open('./predictions/predictions.csv', 'r') as prediction_file:
        prediction_reader = csv.reader(prediction_file)

        for row in prediction_reader:
            for i in range (len(row)):
                if(i % 18 == 0):    
                    prediction.append(float(row[i]))

    show_number(number_img, prediction[0])
