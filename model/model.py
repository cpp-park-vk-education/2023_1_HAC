import sys

import numpy as np

import tensorflow.keras
import pandas as pd
import sklearn as sk
import tensorflow as tf
import platform
from tensorflow.keras.models import load_model

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import *
from tensorflow.keras.callbacks import ModelCheckpoint
from tensorflow.keras.losses import MeanSquaredError
from tensorflow.keras.metrics import RootMeanSquaredError
from tensorflow.keras.optimizers import Adam

WINDOW_SIZE = 300

def stock_to_X_y(stock, window_size):
    stock_as_np = stock.to_numpy()
    X = []
    y = []
    for i in range(len(stock_as_np)-window_size):
        row = [[a] for a in stock_as_np[i:i+window_size]]
        X.append(row)
        label = stock_as_np[i+window_size]
        y.append(label)
    return np.array(X), np.array(y)


def parse_http_data_to_seq(json):
    json_data = json["data"]
    X = []
    for i in json_data.split():
        X.append(float(i))
    return X


class Model:
    def __init__(self):
        pass
#       self.download_model()

    def predict(self, X, lenpredict):
        X_data = np.array(X)


        for i in range(lenpredict):
            X_data = np.append(X_data, model.predict((X_data[i:]).reshape(1, WINDOW_SIZE, 1))[0][0])

        return X_data[WINDOW_SIZE:]
        

    def simple_predict(self, X):
        return self.model.predict(X)

    def fit(self, X_train, y_train, X_val, y_val, name_stock):
        model = Sequential()
        model.add(InputLayer((WINDOW_SIZE, 1)))
        model.add(LSTM(1200))
        model.add(Dense(100, 'relu'))
        model.add(Dense(1, 'linear'))

        model.summary()

        cp = ModelCheckpoint('model_' + name_stock + '/', save_best_only=True)
        model.compile(loss=MeanSquaredError(), optimizer=Adam(learning_rate = 0.0001), metrics=[RootMeanSquaredError()])

        model.fit(X_train, y_train, validation_data=(X_val, y_val), epochs = 10, callbacks=[cp])

    def download_model(self, name_stock):
        self.model = load_model('model_' + name_stock + '/')





if __name__ == '__main__':
    #X = pd.read_csv('data.csv')
    #y = pd.read_csv('labales.csv')
    #X_train, y_train = X[:2000], y[:2000]
    #X_val, y_val = X[2000:3000], y[2000:3000]
    #X_test, y_test = X[3000:], y[3000:]

    model = Model()
    #model.fit(X_train=X_train, y_train=y_train, X_val=X_val, y_val=y_val, window_size=8)
    model.download_model()
    from server import take_predict
    #print(result.shape)
    
    print(take_predict({"lenpredict": '10', 'data': '1 2 3 4 5 6 7 8'}))

    #print(model.predict(X_test))

