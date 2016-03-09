import socket;

host = 'localhost';
port = 31400;
size = 1024;

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
s.connect((host, port));
s.send(b'test');

#data = s.recv(size);
s.close();
#print ('Received: ', repr(data));