import socket;
import time;
import struct; 

#host = '192.168.1.101';
host = 'localhost';
port = 31400;
size = 1024;


# PACKET HEADER
protocol_id = 1;
#packet_size = PACKET_HEADER(4bytes) + length of data

# data
packet_data = '0123456789';
packet_data1 = '01234567890123456789';
packet_data2 = '01234567890abcdefghijklmnopqrstuvwxyz'; #37

# PACKET to server 
packeted_data1 = struct.pack('@HH10s', protocol_id, 4+len(packet_data), packet_data);
packeted_data2 = struct.pack('@HH20s', protocol_id, 4+len(packet_data1), packet_data1);
packeted_data3 = packeted_data1 + packeted_data2;
packeted_data4 = struct.pack('@HH37s', protocol_id, 4+len(packet_data2), packet_data2);

def server_stress_test_with_protocol():
	# test loop for connection and disconnection 
	for i in range(20000):	
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
		s.connect((host, port));
		# test
		for j in range(200):
			s.send(packeted_data1);	
			data = s.recv(size);
			print(repr(data));

			s.send(packeted_data2);	
			data = s.recv(size);
			print(repr(data));
	
			# two packets together 
			s.send(packeted_data3);	
			data = s.recv(size);
			print(repr(data));

			# separate packet test
			s.send(packeted_data4[0:10]);	
			time.sleep(0.01);
			s.send(packeted_data4[10:]);	
			data = s.recv(size);
			print(repr(data));

			time.sleep(0.01);
		s.close();
		print ("Socket is cloesd.");

def server_endurance_test_with_no_protocol():
	# test loop for connection and disconnection 
	for i in range(20000):	
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
		s.connect((host, port));
		# test
		for j in range(200):
			s.send(b'a1234567890');	
			data = s.recv(size);
			print(repr(data));

			s.send(b"abcdefghijklmnopqrstuvwxyz");	
			data = s.recv(size);
			print(repr(data));
			time.sleep(0.01);

		s.close();
		print ("Socket is cloesd.");

server_stress_test_with_protocol();
#server_endurance_test_with_no_protocol();



