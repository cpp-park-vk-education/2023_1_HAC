import sys

import finnhub
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


def stock_to_X(stock, window_size=8):
    #stock_as_np = stock.to_numpy()
    X = []
    #y = []
    for i in range(len(stock)+1-window_size):
        row = [[a] for a in stock[i:i+window_size]]
        X.append(row)
        # label = stock_as_np[i+window_size]
        # y.append(label)
    return np.array(X)


def parse_http_to_seq(json):
    window_size = int(json["window_size"])
    json_data = json["data"]
    X = []
    for i in json_data.split():
        X.append(float(i))
    X = np.array(X).reshape(-1, window_size)
    return X


class Model:
    def __init__(self):
        pass
#       self.download_model()

    def predict(self, X):
        X_data = X.copy()
        X_data = X_data[0]

        for i in range(X.shape[1]):
            X_data = np.append(X_data, self.model.predict((X_data[i:]).reshape(X.shape[1], -1))[0][0])

        return X_data[X.shape[1]:]
        

    def simple_predict(self, X):
        return self.model.predict(X)

    def fit(self, X_train, y_train, X_val, y_val, window_size):
        model = Sequential()
        model.add(InputLayer((window_size, 1)))
        model.add(LSTM(64))
        model.add(Dense(32, 'relu'))
        model.add(Dense(16, 'relu'))
        model.add(Dense(1, 'linear'))

        model.summary()

        cp = ModelCheckpoint('model/', save_best_only=True)
        model.compile(loss=MeanSquaredError(), optimizer=Adam(learning_rate = 0.0001), metrics=[RootMeanSquaredError()])

        model.fit(X_train, y_train, validation_data=(X_val, y_val), epochs = 100, callbacks=[cp])

    def download_model(self):
        self.model = load_model('model/')


if __name__ == '__main__':
    #X = pd.read_csv('data.csv')
    #y = pd.read_csv('labales.csv')
    #X_train, y_train = X[:2000], y[:2000]
    #X_val, y_val = X[2000:3000], y[2000:3000]
    #X_test, y_test = X[3000:], y[3000:]

    model = Model()
    #model.fit(X_train=X_train, y_train=y_train, X_val=X_val, y_val=y_val, window_size=8)
    model.download_model()
    
    result = parse_http_to_seq({"window_size": '8', 'data': '1 2 3 4 5 6 7 8'})
    #print(result.shape)
    
    print(model.predict(result))

    #print(model.predict(X_test))

