#pragma once

#include "utils.hpp"

// Session Class
//////////////////////////////////////////////////////////////////
class session /*: public boost::enalbed_shread_from_this<session>*/ {

//---------------------- Member Functions ----------------------//
public:
	explicit session( boost::asio::io_service& io_service, unsigned short session_id);
	virtual ~session() {}

	// return the socket which this connection gets
	boost::asio::ip::tcp::socket& socket() {
		return *socket_.get();
	}

	// return session ID
	unsigned short sesson_id() {
		return session_id_;
	}

	unsigned short get_session_stat(){
		return session_stat_;
	}
	void set_waiting_mode(){
		set_session_stat(SS_WAITING);
	}

	bool open();												// initailization after socket is on
	void shutdown();

	bool post_recv();											// preparation for receiving data from client 
	bool post_recv(unsigned short start, unsigned short size);	// adjust packet 

	bool post_send(const char* data, const unsigned short size);// send data to client
	//bool post_send(const boost::asio::const_buffers_1& buff);

	
protected:
	// internal function to change session statement
	inline void set_session_stat(unsigned short new_session_stat) {	
		session_stat_ = new_session_stat;
	}

	virtual void process_packet(std::size_t bytes_transferred);	// do process packet received from client 

	// handle received data after receiving data is done
	void handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred );

	// handle sonthing after sending data if needed
	void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred);

//---------------------- Member Variables ----------------------//	
protected:
	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
	unsigned short session_id_;									// max_num: 65,535

	unsigned short session_stat_;

	std::deque< boost::shared_ptr< char[] > > buf_queue_;
	boost::shared_ptr< char[] >	rcv_buff_;
	boost::shared_ptr< char[] >	proc_packet_;

	unsigned short rcv_buff_start_;
};