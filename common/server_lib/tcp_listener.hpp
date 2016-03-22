#pragma once 

#include "utils.hpp"
#include <boost/enable_shared_from_this.hpp>

class session;

// TCP Listener Class
//////////////////////////////////////////////////////////////////
class tcp_listener : public boost::enable_shared_from_this<tcp_listener> {

//---------------- Type Defnitions, Enums and so on -------------//
	typedef boost::asio::ip::tcp::acceptor acceptor_t;

//---------------------- Member Functions ----------------------//
public:
	tcp_listener(boost::asio::io_service& io_service, unsigned short port)
		: acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
		, port_(port)
	{}

	bool start_listening(std::size_t concur_listener_num) {		//	start listening 	
		bool ret = true;
		for( std::size_t i = 0 ; i < concur_listener_num ; i++) {
			ret = ret & PostAccept(nullptr);
		}
		return ret;
	}

	bool PostAccept(boost::shared_ptr<session>* ss_ptr);		// waiting for client connection 
protected:
	void handle_accept(
		session* ss_ptr, 
		const boost::system::error_code& error);

//---------------------- Member Variables ----------------------//
protected:
	acceptor_t acceptor_;
	unsigned short port_;
};