#!/usr/bin/python
import http.server
import os
import configparser

#
# Temporary HTTP server created to design and test frontend
# side of the application.
#


class baseHTTPHandler(http.server.BaseHTTPRequestHandler):

        # Handler for the GET requests
    def do_GET(self):

        # Handle index.html
        if self.path == "/":
            self.path = "/index.html"

        # Handle files' types
        sendReply = False
        if self.path.endswith(".html"):
            folder = "html"
            mimetype = 'text/html'
            sendReply = True
        if self.path.endswith(".css"):
            folder = "css"
            mimetype = 'text/css'
            sendReply = True
        if self.path.endswith(".js"):
            folder = "js"
            mimetype = 'application/javascript'
            sendReply = True

        # try to send response
        try:
            with open(folder + self.path, "r") as f:
                if sendReply == True:
                    self.send_response(200)
                    self.send_header('Content-type', mimetype)
                    self.end_headers()
                    self.wfile.write(bytes(f.read(), "utf-8"))
                    f.close()
                return
        except IOError:
            self.send_error(404, 'File Not Found: %s' % self.path)


if __name__ == "__main__":
    
    # Change dir to the known location
    loaclpath_path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(loaclpath_path)

    # Load server's config
    config = configparser.ConfigParser()
    config.read("../config/python_server.conf")
    PORT = config['DEFAULT']['Port']

    # Change dir to the web content
    os.chdir(os.path.join( \
        loaclpath_path, \
        "../web/simple_client" \
    ))
    
    try:
        # Create a web server and define the handler to manage the
        # incoming request
        server = http.server.HTTPServer(('', int(PORT)), baseHTTPHandler)
        print('Started httpserver on port ', PORT)

        # Wait forever for incoming htto requests
        server.serve_forever()

    except KeyboardInterrupt:
        print('^C received, shutting down the web server')
        server.socket.close()
