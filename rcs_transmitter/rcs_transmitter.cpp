#include <boost/lexical_cast.hpp>
#include "rcs_transmitter.hpp"
#include "serial_port.hpp"
typedef singleton<serial_port> sp_singleton; 
typedef singleton<logger> logger_singleton; 

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

	//rcv_udp_buff_->get()->ptr_[bytes_transferred] = 0x00;
	logger_singleton::get() << rcv_udp_buff_->get()->ptr_;
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


// function run in rcs transmitter  
//////////////////////////////////////////////////////////////////
bool rcst::run() {
	
	// [1] open serial port to get remote controlling signal 
	if(!sp_singleton::get().start(
				ios_,
				//sizeof(RC_SIGNAL), 
				1,		// length to read at once 
				serial_port_,
				9600,	// baud rate 
				false,	// sync mode 
				std::bind(
					&rcst::process_uart_data,
					this,
					std::placeholders::_1,
					std::placeholders::_2
				)))
		return false;

	// [2] for test without server operation 
	std::string default_drone_addr 	= "192.168.1.101";//"125.152.166.26";
	//std::string default_drone_addr 	= "175.253.177.194";
	udp_on(default_drone_addr, DEFAULT_PORT_NUMBER);

	// [3]connect to server in order to get drone ip 
	auto endpoint = boost::asio::ip::tcp::endpoint( 
						boost::asio::ip::address::from_string(server_addr_), 
						server_port_);

	connect(endpoint);

	// [4] start server and wait until ctrl+c
	start();

	return true;
}

// process uart data received 
//////////////////////////////////////////////////////////////////
void rcst::process_uart_data(unsigned char* pbuf, int size) {

	RC_SIGNAL *rcs_p = reinterpret_cast<RC_SIGNAL *>(pbuf);

	if( sizeof(RC_SIGNAL) == size ) {

		rcs_p->pin_1_ = rcs_p->pin_1_/10;
		rcs_p->pin_2_ = rcs_p->pin_2_/10;
		rcs_p->pin_3_ = rcs_p->pin_3_/10;
		rcs_p->pin_4_ = rcs_p->pin_4_/10;
		rcs_p->pin_5_ = rcs_p->pin_5_/10;
		rcs_p->pin_6_ = rcs_p->pin_6_/10;

		std::cout << rcs_p->pin_1_ << "\t" << rcs_p->pin_2_ << "\t" << rcs_p->pin_3_ << "\t" << rcs_p->pin_4_ << "\t" << rcs_p->pin_5_ << "\t"  << rcs_p->pin_6_ << std::endl;		

		post_udp_send(reinterpret_cast<const char*>(pbuf),size);
		
	} else {

		logger_singleton::get().error() << "Broken data is comming through UART" << std::endl;

	}

}

#ifdef _RCST_

typedef singleton<rcst> rcst_singleton; 

// const for test 
//////////////////////////////////////////////////////////////////
const std::string default_server_addr	= "192.168.1.100";

const std::string default_serial_port	= "/dev/cu.usbmodem1421";
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

	std::cout << "RC_SIGNAL structure size is :" << sizeof(RC_SIGNAL) << std::endl;

	if(!rcst_singleton::get().init(serial_port, sever_addr, DEFAULT_PORT_NUMBER, drone_id) ||
		!rcst_singleton::get().run())
		logger_singleton::get().error() << "failed to initiate" << std::endl;

	//rcst_singleton::get().shutdown();

	return 0;
}

#endif /* _RCST_ */


// end of file 

