#pragma once

#include "../common/utils.hpp"
#include <boost/asio/serial_port.hpp>
#include <functional>

typedef std::function<void(unsigned char*,int)> ext_process;

// Serial Port Class
//////////////////////////////////////////////////////////////////
class serial_port
{
//---------------------- Member Functions ----------------------//
public:
	serial_port()
		: ext_process_(nullptr) {}
	~serial_port() {}

	bool start(
		boost::asio::io_service& io, 
		unsigned short rcv_buff_size,
		std::string port,
		unsigned short baud_rate, 
		bool async,
		ext_process proc);

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
	
	ext_process ext_process_;

}; // end of serial port class 



// End of File 