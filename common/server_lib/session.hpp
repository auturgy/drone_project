#pragma once

#include "utils.hpp"
#include <boost/enable_shared_from_this.hpp>

// Session Class
//////////////////////////////////////////////////////////////////
class session : public boost::enable_shared_from_this<session> {

//---------------------- Member Functions ----------------------//
public:
	explicit session( boost::asio::io_service& io_service, unsigned short session_id);
	virtual ~session() {
		Logger::info() << "session destruction (ID:" << session_id_ << ")" << std::endl;
	}

	boost::shared_ptr<session> get()
    {
        return shared_from_this();
    }

	// return the socket which this connection gets
	boost::asio::ip::tcp::socket& socket() {
		return *socket_.get();
	}

	// return session ID
	unsigned short session_id() {
		return session_id_;
	}

	unsigned short port() {
		return port_;
	}

	unsigned short get_session_stat(){
		return session_stat_.load(boost::memory_order_acquire);
	}
	void set_waiting_mode(){
		set_session_stat(SS_WAITING);
	}

	bool open(unsigned short port);												// initailization after socket is on
	void shutdown();

	bool post_recv();											// preparation for receiving data from client 
	bool post_recv(unsigned short start);						// adjust packet 

	bool post_send(const char* data, const unsigned short size);// send data to client
	
protected:
	// internal function to change session statement
	inline void set_session_stat(unsigned short new_session_stat) {	
		Logger::info() << "Session(" << session_id_ <<") status has changed into " << new_session_stat <<std::endl;

		session_stat_.store(new_session_stat, boost::memory_order_release);
	}

	virtual void process_packet(const char* data, const unsigned short size);	// do process packet received from client 

	// handle received data after receiving data is done
	void handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred );

	// handle sonthing after sending data if needed
	void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred);

//---------------------- Member Variables ----------------------//	
protected:
	boost::asio::io_service::strand strand_;

	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
	unsigned short session_id_;									// max_num: 65,535
	unsigned short port_;

	boost::atomic<unsigned short> session_stat_;

	boost::shared_ptr<PKT_UNIT> *rcv_buff_;
	unsigned short rcv_buff_start_;
};