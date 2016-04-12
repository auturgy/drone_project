#ifdef _CLIENT_TEST_

#include "connection.hpp"
typedef singleton<connection> conn_singleton; 

// upd connection test 
//////////////////////////////////////////////////////////////////
int udp_test(std::string& addr) {

	//conn_singleton::get.init();
	conn_singleton::get().init();
	conn_singleton::get().udp_on(addr, DEFAULT_PORT_NUMBER);

	std::string test = "test";
	conn_singleton::get().post_udp_send(test.c_str(),test.length());

	std::getchar();

	conn_singleton::get().shutdown();

	return 1;
}

// tcp connection test 
//////////////////////////////////////////////////////////////////
int tcp_test(std::string& addr) {

	const unsigned short MAX_MESSAGE_LEN = 100;
	char szInputMessage[MAX_MESSAGE_LEN * 2] = {0,};
	
	auto endpoint = boost::asio::ip::tcp::endpoint( 
						//boost::asio::ip::address::from_string("192.168.1.101"), 
						boost::asio::ip::address::from_string(addr), 
						DEFAULT_PORT_NUMBER);

	conn_singleton::get().init();
	conn_singleton::get().connect(endpoint);

	unsigned short try_connecting = 0;
	const unsigned short MAX_TRY = 5;

	while(1) {
		
		sleep(1);
		
		if(conn_singleton::get().get_connection_stat() == SS_CLOSE || try_connecting > MAX_TRY) {
			std::cout << "failed to connect ~!" << std::endl;
			return 1;
		}

		if(conn_singleton::get().get_connection_stat() == SS_OPEN) {
			std::cout << "=============================" << std::endl;
			std::cout << "Connection is established~   " << std::endl;
			std::cout << "=============================" << std::endl;
			break;
		}

		std::cout << "connecting..." << std::endl;
		try_connecting++;
	}

	do {

		unsigned short input_len = std::strlen(szInputMessage);
		//std::cout << input_len << std::endl;

		if(input_len){

			boost::shared_ptr<PKT_UNIT>& packet = conn_singleton::get().alloc_packet();
			PKT_REQ_TEST* packet_p = (PKT_REQ_TEST*)packet.get();

			packet_p->init();
			std::memcpy(packet_p->buffer_, szInputMessage, input_len);
			packet_p->size_ += input_len;

			//std::cout << "packet_p->size_: " << packet_p->size_ << std::endl;
			conn_singleton::get().post_send(packet, packet_p->size_);
		}
		sleep(1);
		std::cout << "[Input] "; 
	} while(std::cin.getline( szInputMessage, MAX_MESSAGE_LEN) );

	return 1;
}

// main
//////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	bool is_udp_test = false;
	std::string addr;

	if( argc > 1) {
		addr = argv[1]; 
	} else {
		addr = "127.0.0.1";
	}

	if(is_udp_test)
		udp_test(addr);
	else 
		tcp_test(addr);
	
	return 1;
}

#endif /* _CLIENT_TEST_ */

