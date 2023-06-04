from http.server import HTTPServer, BaseHTTPRequestHandler


from log import log
from handlers import router


HOST = "0.0.0.0"
PORT = 9950


handles = {}
    

class NeuralHTTP(BaseHTTPRequestHandler):
    def do_GET(self):
        log("handle!")
        for header in self.headers:
            handles[header] = self.headers[header]
            log(f"{header}: {self.headers[header]}")
        
        self.protocol_version = "HTTP/1.1"

        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Connection", "close")
        result = str(router(handles))
        
        log("resp:")
        log("Response 200")
        log(f"Content-type: text/html")
        log(f"Connection: close")
        log(f"Content-Length: {str(len(result))}")
        log(f"data: {result}")


        self.send_header("Content-Length", str(len(result)))
        self.send_header("data", result.replace("\n", ""))

        self.end_headers()
        self.wfile.write(bytes("OK", "utf-8")) 




if __name__ == '__main__':
    server = HTTPServer((HOST, PORT), NeuralHTTP)
    server.serve_forever()
    server.server_close()

