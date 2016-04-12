#include "serial_port.hpp"
#include <boost/make_shared.hpp>
#include "rcs_transmitter.hpp"

typedef singleton<logger> logger_singleton; 

// start function   
// choose whether mode is asyncrounous 
//////////////////////////////////////////////////////////////////
bool serial_port::start(	
	boost::asio::io_service& io, 
	unsigned short rcv_buff_size,
	std::string port,
	unsigned short baud_rate, 
	bool async)
{
	boost::system::error_code error;
	serial_port_ = boost::make_shared<boost::asio::serial_port>(boost::ref(io));
	rcv_buff_size_= rcv_buff_size;
	read_buf_raw_ = boost::make_shared<unsigned char[]>(rcv_buff_size_);

	serial_port_->open(port, error);

	if(error){
		logger_singleton::get().error() << "can not open " << port << " port" << std::endl;
		logger_singleton::get().info() << "change UART port and try again!!!! " << std::endl;
		return false;
	}

	// default baud rate : 9600
	serial_port_->set_option(boost::asio::serial_port_base::baud_rate(baud_rate)); 
	serial_port_->set_option(boost::asio::serial_port_base::character_size(8));
	serial_port_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	serial_port_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
	serial_port_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));


	if(!async) {
		boost::thread th(&serial_port::sync_process, this);
	} else {
		boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
		post_recv();
	}
	
	return true;
} // end of start function 

// stop function   
//////////////////////////////////////////////////////////////////
void serial_port::stop(){

	if (serial_port_.get()) {

		logger_singleton::get().info() << "serial_port::stop" << std::endl;

		serial_port_->cancel();
		serial_port_->close();
		serial_port_.reset();
	}
} // end of stop function 

// preparation for receiving data via serial port 
//////////////////////////////////////////////////////////////////
bool serial_port::post_recv() {

	if (serial_port_.get() == NULL || !serial_port_->is_open()){
		logger_singleton::get().info() << "serial_port::post_recv - error" << std::endl;
		return false;	
	} 

	serial_port_->async_read_some( 
		boost::asio::buffer(read_buf_raw_.get(), rcv_buff_size_),
		boost::bind(
			&serial_port::handle_receive,
			this, 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred));

	return true;
} // end of post_recv function 


// handle received data after receiving data is done
//////////////////////////////////////////////////////////////////
void serial_port::handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred ){

	if (serial_port_.get() == NULL || !serial_port_->is_open()) {
		logger_singleton::get().error() << "port is closed" << std::endl;
		return;
	}

	if(!error) 
	{
		// do something 
		typedef singleton<rcst> rcst_singleton; 
		if(!rcst_singleton::get().post_udp_send(reinterpret_cast<const char*>(read_buf_raw_.get()),rcv_buff_size_)) {	
			// no udp connection, then just print out 
			logger_singleton::get() << read_buf_raw_.get();	
		}
	}

	post_recv();

} // end of handle_receive function 


// write to arduino with std::string  
//////////////////////////////////////////////////////////////////
int serial_port::write_some(const std::string &buf)
{
	return write_some(buf.c_str(), buf.size());
} // end of write_some function 


// write to arduino with char* 
//////////////////////////////////////////////////////////////////
int serial_port::write_some(const char *buf, const int &size)
{
	boost::system::error_code ec;

	if (!serial_port_) return -1;
	if (size == 0) return 0;

	return serial_port_->write_some(boost::asio::buffer(buf, size), ec);
} // end of write_some function 


// process syncronously 
//////////////////////////////////////////////////////////////////
void serial_port::sync_process(){

	while(serial_port_->is_open()) {

		// recevie 
		serial_port_->read_some(boost::asio::buffer(read_buf_raw_.get(),rcv_buff_size_));

		// do something 
		typedef singleton<rcst> rcst_singleton; 
		if(!rcst_singleton::get().post_udp_send(reinterpret_cast<const char*>(read_buf_raw_.get()),rcv_buff_size_)) {	
			// no udp connection, then just print out 
			logger_singleton::get() << read_buf_raw_.get();	
		}
	}

} // end of sync_process function 


// end of file 