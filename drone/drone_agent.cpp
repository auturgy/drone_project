#include <boost/lexical_cast.hpp>
#include "drone_agent.hpp"

// overriding function : handle_connect
//////////////////////////////////////////////////////////////////
void drone_agent::handle_connect(const boost::system::error_code& error) {

	if(!error) {

		// do something here!!

	}

	connection::handle_connect(error);
}

// overriding function : process_packet
//////////////////////////////////////////////////////////////////
void drone_agent::process_packet(const char* data, const unsigned short size) {

	// do something here!

}

void drone_agent::handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	std::cout << "[DRONE] " << rcv_udp_buff_->get()->ptr_ << std::endl;
	post_udp_recv();
}

#ifdef _DRONE_AGENT_

typedef singleton<drone_agent> agent_singleton; 
// main
//////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	std::string addr;

	if( argc > 1) {
		addr = argv[1]; 
	} else {
		addr = "127.0.0.1";
	}

	agent_singleton::get().init();
	agent_singleton::get().udp_on(addr, DEFAULT_PORT_NUMBER);

	std::string test = "test";
	agent_singleton::get().post_udp_send(test.c_str(),test.length());

	std::getchar();

	agent_singleton::get().shutdown();


	return 1;
}

#endif /* _DRONE_AGENT_ */