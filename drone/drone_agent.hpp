#pragma once 

#include "../common/client_lib/connection.hpp"

class gpio;

// RC Singal Transmitter Class 
//////////////////////////////////////////////////////////////////
class drone_agent : public connection {
//---------------------- Member Functions ----------------------//
public:
	bool init();

protected:
	void handle_connect(const boost::system::error_code& error);
	void process_packet(const char* data, const unsigned short size);
	void handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred );

	void gpio_set();
	void send_to_gpio(RC_SIGNAL *rcs);
//---------------------- Member Variables ----------------------//
protected:
	std::string server_addr_;
	unsigned short server_port_;
	std::string drone_ip_str_;
	unsigned short drone_id_;

	std::vector<std::unique_ptr<gpio>> gpio_2_r_pins_;

}; /* end of rcst class */


// end of file