#include <boost/lexical_cast.hpp>
#include "rcs_transmitter.hpp"
#include "serial_port.hpp"
typedef singleton<serial_port> sp_singleton; 

// function init
// do this function first!!!
//////////////////////////////////////////////////////////////////
bool rcst::init(
		std::string sp, 
		std::string server_addr, 
		unsigned short server_port,
		unsigned short drone_id) {

	serial_port_ = sp;
	server_addr_ = server_addr;
	server_port_ = server_port;
	drone_id_ = drone_id;

	return connection::init();
}

// overriding function : handle_connect
//////////////////////////////////////////////////////////////////
void rcst::handle_connect(const boost::system::error_code& error) {

	if(!error) {

		// do something here!!

	}

	connection::handle_connect(error);
}

// overriding function : process_packet
//////////////////////////////////////////////////////////////////
void rcst::process_packet(const char* data, const unsigned short size) {

	// do something here!

}


void rcst::handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	std::cout << "[RCST] " << rcv_udp_buff_->get()->ptr_ << std::endl;
	post_udp_recv();
}


// find drone ip in order to connect & control 
// ask to server if the drone is on
//////////////////////////////////////////////////////////////////
bool rcst::find_drone_ip(unsigned short drone_id /*lte sim?*/) {

	// have to make a scenario to get drone id before connecting server 
	//const unsigned short drone_id = 0;

	return true;
}


bool rcst::run() {

	// [1] open serial port to get remote controlling signal 
	if(!sp_singleton::get().start(
				ios_,
				1,		// length to read at once 
				serial_port_,
				9600,	// baud rate 
				false	// sync mode 
				))
		return false;

	// [2] connect to server
	auto endpoint = boost::asio::ip::tcp::endpoint( 
						boost::asio::ip::address::from_string(server_addr_), 
						server_port_);

	connect(endpoint);

	return true;
}

#ifdef _RCST_

typedef singleton<rcst> rcst_singleton; 

// const  
//////////////////////////////////////////////////////////////////
const std::string default_server_addr	= "192.168.1.101";

const std::string default_serial_port	= "/dev/cu.usbmodem1411";
const std::string default_drone_addr 	= "127.0.0.1";

const unsigned short default_drone_id	= 2016;

// main
//////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	std::string serial_port, sever_addr;
	unsigned short drone_id;

	if( argc > 3) {
		serial_port = argv[1];
		sever_addr = argv[2]; 
		drone_id = boost::lexical_cast<unsigned short>(argv[3]);
	} else {
		serial_port = default_serial_port;
		sever_addr = default_server_addr;
		drone_id = default_drone_id;
	}

	if(!rcst_singleton::get().init(serial_port, sever_addr, DEFAULT_PORT_NUMBER, drone_id) ||
		!rcst_singleton::get().run())
		std::cout << "failed to initialte" << std::endl;

	std::getchar();
	rcst_singleton::get().shutdown();

	return 1;
}

#endif /* _RCST_ */


// end of file 

