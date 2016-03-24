#include "connection.hpp"

int main(int argc, char* argv[]) {

	const unsigned short MAX_MESSAGE_LEN = 100;
	char szInputMessage[MAX_MESSAGE_LEN * 2] = {0,};
	std::string addr;

	if( argc > 1) {
		addr = argv[1]; 
	} else {
		addr = "127.0.0.1";
	}

	auto endpoint = boost::asio::ip::tcp::endpoint( 
						//boost::asio::ip::address::from_string("192.168.1.101"), 
						boost::asio::ip::address::from_string(addr), 
						DEFAULT_PORT_NUMBER);

	connection::get().init();
	connection::get().connect(endpoint);

	unsigned short try_connecting = 0;
	const unsigned short MAX_TRY = 5;

	while(1) {
		
		sleep(1);
		
		if(connection::get().get_connection_stat() == SS_CLOSE || try_connecting > MAX_TRY) {
			std::cout << "failed to connect ~!" << std::endl;
			return 1;
		}

		if(connection::get().get_connection_stat() == SS_OPEN) {
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

			boost::shared_ptr<PKT_UNIT>& packet = connection::get().alloc_packet();
			PKT_REQ_TEST* packet_p = (PKT_REQ_TEST*)packet.get();

			packet_p->init();
			std::memcpy(packet_p->buffer_, szInputMessage, input_len);
			packet_p->size_ += input_len;

			//std::cout << "packet_p->size_: " << packet_p->size_ << std::endl;
			connection::get().post_send(packet, packet_p->size_);
		}
		sleep(1);
		std::cout << "[Input] "; 
	} while(std::cin.getline( szInputMessage, MAX_MESSAGE_LEN) );

	return 1;
}


