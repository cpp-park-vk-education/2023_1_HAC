from log import log

from model import Model
from model import parse_http_data_to_seq
from model import stock_to_X_y

import os

WINDOW_SIZE = 300

def router(json):
    try:
        log(f"In router: {json['stock_name']} and {json['action']}")
        if json["action"] == "predict":
            result = take_predict(json)
            log(str(result))
            return result
        elif json["action"] == "fit":
            result = make_fit(json)
            log(str(result))
            return result
        elif json["action"] == "refit":
            result = make_refit(json)
            log(str(result))
            return result
    except:
        log(f"In router: ERROR")
        return "ERROR"

def make_fit(json):
    log(f"BEFOR PARSE")
    X = parse_http_data_to_seq(json)
    X_ , y_ = stock_to_X_y(X[::-1], WINDOW_SIZE)
    X_train, y_train = X_[:3000], y_[:3000]
    X_val, y_val = X_[3000:], y_[3000:]
    log(f"BEFOR MODEL")
    model = Model()
    try:
        if os.path.exists('model/model_' + json["stock_name"] + '/'):
            return "OK"
        model.fit(X_train, y_train, X_val, y_val, json["stock_name"])
    except:
        return "BAD"
    return "OK"

def make_refit(json):
    X = parse_http_data_to_seq(json)
    X_ , y_ = stock_to_X_y(X[::-1], WINDOW_SIZE)
    X_train, y_train = X_[:2500], y_[:2500]
    X_val, y_val = X_[2500:], y_[2500:]
    model = Model()
    try:
        model.refit(X_train, y_train, X_val, y_val, json["stock_name"])
    except:
        return "BAD"
    return "OK"

def take_predict(json):
    
    lenpredict = int(json["lenpredict"])
    log(f"In takepredict: {json['stock_name']} and lenpredict: {lenpredict} ")
    X = parse_http_data_to_seq(json)
    model = Model()
    model.download_model(json["stock_name"])

    return model.predict(X[::-1], lenpredict)
