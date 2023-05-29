from http.server import HTTPServer, BaseHTTPRequestHandler

from model import Model
from model import parse_http_data_to_seq
from model import stock_to_X_y

HOST = "0.0.0.0"
PORT = 9950

WINDOW_SIZE = 300

handles = {} # window_size, data

def router(json):
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
    model.download_model(json["stock_name"])
    try:
        model.fit(X_train, y_train, X_val, y_val, json["stock_name"])
    except:
        return "BAD"
    return "OK"

def take_predict(json): # time solution
    lenpredict = int(json["lenpredict"])
    X = parse_http_data_to_seq(json)
    model = Model()
    model.download_model(json["stock_name"])

    return model.predict(X, lenpredict)
    
    

class NeuralHTTP(BaseHTTPRequestHandler):
    def do_GET(self):
        for header in self.headers:
            handles[header] = self.headers[header]
            print(f"{header}: {self.headers[header]}")
        self.protocol_version = "HTTP/1.1" 
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Connection", "close") # добавить заголовок Connection
        result = str(router(handles))
        self.send_header("Content-Length", str(len(result)))
        self.send_header("data", result.replace("\n", ""))

        self.end_headers()
        self.wfile.write(bytes("OK", "utf-8")) 




if __name__ == '__main__':
    #print(take_predict({"window_size": '8', 'data': '1 2 3 4 5 6 7 8'}))
    server = HTTPServer((HOST, PORT), NeuralHTTP)
    server.serve_forever()
    server.server_close()

