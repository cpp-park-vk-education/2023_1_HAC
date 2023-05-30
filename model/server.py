from http.server import HTTPServer, BaseHTTPRequestHandler

import os
import sys
from model import Model
from model import parse_http_data_to_seq
from model import stock_to_X_y
from datetime import datetime

import logging

HOST = "0.0.0.0"
PORT = 9950

WINDOW_SIZE = 300

handles = {} # window_size, data

def router(json):
    print(json["stock_name"])
    if json["action"] == "predict":
        return take_predict(json)
    elif json["action"] == "fit":
        return make_fit(json)
    elif json["action"] == "refit":
        return make_refit(json)


def make_fit(json):
    X = parse_http_data_to_seq(json)
    X_ , y_ = stock_to_X_y(X, WINDOW_SIZE)
    X_train, y_train = X_[:3000], y_[:3000]
    X_val, y_val = X_[3000:], y_[3000:]
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
    X_ , y_ = stock_to_X_y(X, WINDOW_SIZE)
    X_train, y_train = X_[:500], y_[:500]
    X_val, y_val = X_[:500:], y_[500:]
    model = Model()
    try:
        model.refit(X_train, y_train, X_val, y_val, json["stock_name"])
    except:
        return "BAD"
    return "OK"

def take_predict(json): # time solution
    lenpredict = int(json["lenpredict"])
    X = parse_http_data_to_seq(json)
    model = Model()
    model.download_model(json["stock_name"])

    return model.predict(X, lenpredict)

def log(message):
    with open('model/output.txt', 'a') as file:
            current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            file.write(f"{current_time}: {message}\n")

    

class NeuralHTTP(BaseHTTPRequestHandler):
    def do_GET(self):
        log("handle!")
        for header in self.headers:
            handles[header] = self.headers[header]
            log(f"{header}: {self.headers[header]}")
        
        self.protocol_version = "HTTP/1.1"

        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Connection", "close") # добавить заголовок Connection
        result = str(router(handles))
        
        log("resp:")
        log("Response 200")
        log(f"Content-type: text/html")
        log(f"Connection: close")
        log(f"Content-Length: {str(len(result))}")
        log(f"data: {result}")
        
        
        # Записываем информацию об успешной отправке ответа
        log(f"{current_time}: Response sent: OK\n")

        self.send_header("Content-Length", str(len(result)))
        self.send_header("data", result.replace("\n", ""))

        self.end_headers()
        self.wfile.write(bytes("OK", "utf-8")) 




if __name__ == '__main__':
    #print(take_predict({"window_size": '8', 'data': '1 2 3 4 5 6 7 8'}))
    server = HTTPServer((HOST, PORT), NeuralHTTP)
    server.serve_forever()
    server.server_close()

