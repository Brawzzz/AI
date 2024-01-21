# ================================== INCLUDES ================================== #
import numpy as np 
import matplotlib.pyplot as plt 
import warnings

from sklearn.datasets import make_blobs
from sklearn.metrics import accuracy_score
from utilities import *
from typing import List, Any

warnings.filterwarnings('ignore')

# ================================== FUNCTIONS ================================== #
def init(X):
    W = np.random.randn(X.shape[1] , 1)
    b = np.random.randn(1)

    return(W,b)
 
def model(X , W , b):
    Z = X.dot(W) + b
    A = 1 / (1 + np.exp(-Z))
    return A
 
def log_loss(A , y):
    e = 1e-15
    loss = 1/len(y) * np.sum( y * np.log(A + e) - (1 - y) * np.log(1 - A + e))
    return loss

def gradients(A , X , y):
    # gradient Log loss by w dL/dw
    dW = 1 / len(y) * np.dot(X.T , A - y)
    
    # gradient Log loss by b dL/db
    db = 1 / len(y) * np.sum(A - y)

    return(dW , db)

def update(dW , db, W , b , learning_rate):

    W -= learning_rate * dW
    b -= learning_rate * db

    return(W , b)

def predict(X , W , b):
    A = model(X , W , b)
    return A >= 0.5

def artificial_neuron(X , y , learning_rate , iteration_max):

    loss_values = []

    W , b = init(X)
    
    for i in range(iteration_max):
        A = model(X , W , b)
        loss = log_loss(A , y)
        loss_values.append(loss)

        (dW , db) = gradients(A , X , y)
        (W , b) = update(dW , db , W , b , learning_rate)

    prediction = predict(X , W , b)
    print(accuracy_score(y , prediction))

    # print(np.array(loss_values))
    plt.plot(loss_values)

    return (W , b)

def print_img(img : List[List[Any]]):
    
    width = np.shape(img)[1]
    height = np.shape(img)[0]

    print(width , height)

    for i in range(height):
        print(i, end=" : ")
        for j in range(width):
            print(img[i][j], end=" ")
        print("\n")

def normalize_img(img):

    normalize_range_min = 0
    normalize_range_max = 1
    normalize_dist_max = normalize_range_max - normalize_range_min

    bits_range_min = 0
    bits_range_max = 255
    bits_range_dist_max = bits_range_max - bits_range_min

    for i in range(len(img)):

            pixel_value = img[i]
            normalized_value = (pixel_value - bits_range_min) * (normalize_dist_max / bits_range_dist_max) + normalize_range_min
            img[i] = normalized_value

# ================================== MAIN ================================== #
# ================================== #
# TO DO 
# 1. Normaliser le train_set et le test_set (0-255 -> 0-1)
# 2. flatten() les variables du train_set et du test_set (64x64 -> 4096)
# 3. Entrainer le modele sur le train_set (tracer la courbe d'apprentissage, trouver les bons hyper-params)
# (si vous rencontrez un probleme avec le log_loss, utiliser la fonction de sklearn a la place !)
# 4. Évaluer le modele sur le test_set (tracer également la courbe de Loss pour le test_set)
# 5. Partager vos conclusions dans les commentaires !
# ================================== #

learning_rate = 0.01
iteration_max = 100

X_train , y_train, X_test, y_test = load_data()

reshape_X_train = X_train.reshape(X_train.shape[0] , X_train.shape[1] * X_train.shape[2])
for i in range(reshape_X_train.shape[0]):
    normalize_img(reshape_X_train[i])

reshape_X_test = X_test.reshape(X_test.shape[0] , X_test.shape[1] * X_test.shape[2])
for i in range(X_test.shape[0]):
    normalize_img(reshape_X_test[i])

(W , b) = artificial_neuron(reshape_X_test , y_test , learning_rate , iteration_max)

plt.show()





