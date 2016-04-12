#pragma once 

#include "../common/client_lib/connection.hpp"

// RC Singal Transmitter Class 
//////////////////////////////////////////////////////////////////
class drone_agent : public connection {
//---------------------- Member Functions ----------------------//
public:

protected:
	void handle_connect(const boost::system::error_code& error);
	void process_packet(const char* data, const unsigned short size);
	void handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred );

//---------------------- Member Variables ----------------------//
protected:
	std::string server_addr_;
	unsigned short server_port_;
	std::string drone_ip_str_;
	unsigned short drone_id_;
}; /* end of rcst class */


// end of file