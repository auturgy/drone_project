#include <boost/lexical_cast.hpp>
#include "drone_agent.hpp"

typedef singleton<logger> logger_singleton; 

// overriding function : handle_connect
//////////////////////////////////////////////////////////////////
void drone_agent::handle_connect(const boost::system::error_code& error) {

	if(!error) {

		// do something here just after connection is established. 

	}

	connection::handle_connect(error);
}

// overriding function : process_packet
//////////////////////////////////////////////////////////////////
void drone_agent::process_packet(const char* data, const unsigned short size) {

	// do something here!

}

void drone_agent::handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	// example codes
	unsigned char c = *reinterpret_cast<unsigned char*>(rcv_udp_buff_->get()->ptr_);
	//logger_singleton::get() << rcv_udp_buff_->get()->ptr_;

	// do something here!!!

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
	agent_singleton::get().start();

	//agent_singleton::get().shutdown();

	return 0;
}

#endif /* _DRONE_AGENT_ */


// end of file 