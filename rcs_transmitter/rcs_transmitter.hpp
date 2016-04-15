#pragma once 

#include "../common/client_lib/connection.hpp"

// RC Singal Transmitter Class 
//////////////////////////////////////////////////////////////////
class rcst : public connection {
//---------------------- Member Functions ----------------------//
public:
	bool init(
		std::string sp, 
		std::string server_addr, 
		unsigned short server_port,
		unsigned short drone_id);

	bool run();
protected:
	void handle_connect(const boost::system::error_code& error);
	void process_packet(const char* data, const unsigned short size);
	void handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred );

	void process_uart_data(unsigned char* pbuf, int size);

	bool find_drone_ip(unsigned short drone_id /*lte sim?*/);	// ask if the drone is on

//---------------------- Member Variables ----------------------//
protected:
	std::string serial_port_;
	std::string server_addr_;
	unsigned short server_port_;
	std::string drone_ip_str_;
	unsigned short drone_id_;
}; /* end of rcst class */


// end of file