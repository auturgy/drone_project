#pragma once

#include "../common/server_lib/utils.hpp"
#include <boost/asio/serial_port.hpp>

// Serial Port Class
//////////////////////////////////////////////////////////////////
class serial_port
{
//---------------------- Member Functions ----------------------//
public:
	serial_port() {}
	~serial_port() {}

	bool start(
		boost::asio::io_service& io, 
		unsigned short rcv_buff_size,
		std::string port,
		unsigned short baud_rate, 
		bool async);

	void stop();

	void sync_process();
	
	int write_some(const std::string &buf);
	int write_some(const char *buf, const int &size);

protected:
	bool post_recv();
	void handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred );

//---------------------- Member Variables ----------------------//
protected:
	boost::shared_ptr<boost::asio::serial_port> serial_port_;
	boost::shared_ptr<unsigned char[]> read_buf_raw_;
	unsigned short rcv_buff_size_;
	//rcst_base* rcst_base_;

}; // end of serial port class 



// End of File 