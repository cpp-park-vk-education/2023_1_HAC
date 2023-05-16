from http.server import HTTPServer, BaseHTTPRequestHandler

from model import Model
from model import parse_http_to_pandas

HOST = "25.21.238.202"
PORT = 9950

handles = {} # window_size, data


def take_predict(json): # time solution
    X_predict = parse_http_to_pandas(json)
    model = Model()
    model.download_model()
    #print(X_predict)
    return model.predict(X_predict)
    
    

class NeuralHTTP(BaseHTTPRequestHandler):
    def do_GET(self):
        for header in self.headers:
            handles[header] = self.headers[header]
        self.protocol_version = "HTTP/1.1" 
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Connection", "close") # добавить заголовок Connection
        self.send_header("Content-Length", str(len(str(take_predict(handles)))))
        self.send_header("data", str(take_predict(handles)))
        
        self.end_headers()
        self.wfile.write(bytes("OK", "utf-8")) 




if __name__ == '__main__':
    server = HTTPServer((HOST, PORT), NeuralHTTP)
    server.serve_forever()
    server.server_close()

