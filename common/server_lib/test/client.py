import socket;
import time;
import struct; 
import threading;

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

# very basic test for test 
# to do this, you should uncomment _SIMPLE_ECHO_SERVER_ in protocol.hpp
#//////////////////////////////////////////////////////////////////
def echo_server_stress_test():
	# test loop for connection and disconnection 
	for i in range(20000):	
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
		s.connect((host, port));
		# test
		for j in range(10):
			s.send(b'a1234567890');	
			data = s.recv(size);
			print(repr(data));

			s.send(b"abcdefghijklmnopqrstuvwxyz");	
			data = s.recv(size);
			print(repr(data));
			time.sleep(0.01);

		s.close();
		print ("Socket is cloesd.");

# send normal size packet 
#//////////////////////////////////////////////////////////////////
def send_packet_with_protocol(s):
	s.send(packeted_data1);	
	data = s.recv(size);
	print(repr(data));

# send longer size packet 
#//////////////////////////////////////////////////////////////////
def send_longer_packet_with_protocol(s):
	s.send(packeted_data2);	
	data = s.recv(size);
	print(repr(data));

# send the longest size packet 
#//////////////////////////////////////////////////////////////////
def send_longest_packet_with_protocol(s):
	s.send(packeted_data4);	
	data = s.recv(size);
	print(repr(data));

# send two packets together 
#//////////////////////////////////////////////////////////////////
def send_two_packets_together_with_protocol(s):
	s.send(packeted_data3);	
	data = s.recv(size);
	print(repr(data));

# send the packet divided into two 
#//////////////////////////////////////////////////////////////////
def send_separated_packet_with_protocol(s):
	s.send(packeted_data4[0:10]);	
	time.sleep(0.01);
	s.send(packeted_data4[10:]);	
	data = s.recv(size);


# send normal size packet 
#//////////////////////////////////////////////////////////////////
alive = True;
def server_stress_test_with_protocol(id):
	print ("server_stress_test_with_protocol, id: ",id);
	# test loop for connection and disconnection 
	for i in range(2000):	
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
		s.connect((host, port));
		if alive == False:
				return;

		for j in range(20):
			send_packet_with_protocol(s);
			send_longer_packet_with_protocol(s);
			send_longest_packet_with_protocol(s);
			send_two_packets_together_with_protocol(s);
			send_separated_packet_with_protocol(s);
			time.sleep(0.01);
			if alive == False:
				return;


# keep connecting until time is done. 
#//////////////////////////////////////////////////////////////////
def keep_connecting(id):
	print ("keep_connecting, id: ",id);
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
	s.connect((host, port));
	send_packet_with_protocol(s);
	time.sleep(1);



# check maximum connection availability
#//////////////////////////////////////////////////////////////////
def maximum_connection_num_check():

	th = threading.Thread();

	for i in range(200):
		th = threading.Thread(target=keep_connecting, args=(i,));
		th.daemon = True;
		th.start();
	
	th.join();

# function like getchar in c++
#//////////////////////////////////////////////////////////////////
def getchar():

   #Returns a single character from standard input
   import tty, termios, sys;
   fd = sys.stdin.fileno();
   old_settings = termios.tcgetattr(fd);
   try:
      tty.setraw(sys.stdin.fileno());
      ch = sys.stdin.read(1);
   finally:
      termios.tcsetattr(fd, termios.TCSADRAIN, old_settings);
   return ch;

# check maximum connection availability
#//////////////////////////////////////////////////////////////////
def main_stress_test_with_multiple_connection():

	th = threading.Thread();
	for i in range(200):	
		th = threading.Thread(target=server_stress_test_with_protocol, args=(i,));
		th.daemon = True;
		th.start();
		
	#th.join();
	ch = getchar();
	alive = False;
    #print ('You pressed', ch);
    #getchar();
   

# main function
#//////////////////////////////////////////////////////////////////
def main():

	# Do unit tests here~~~
	#//////////////////////////////////////////////////////////////////
	#echo_server_test_with_no_protocol();
	#server_stress_test_with_protocol();
	#keep_connecting();
	#maximum_connection_num_check();
	main_stress_test_with_multiple_connection();


if __name__ == '__main__':

	main();



# end of file


