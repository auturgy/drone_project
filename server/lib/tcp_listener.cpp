#include "tcp_listener.hpp"
#include "session.hpp"
#include "server_ctrl.hpp"

// Post Accept methond to prepare for incoming connection 
//////////////////////////////////////////////////////////////////
bool tcp_listener::PostAccept() {
	if(!acceptor_.is_open()) {
		return false;
	}

	boost::shared_ptr<session> ss_ptr = server_ctrl::get().alloc_session();

	if(ss_ptr == nullptr) {
		logger::warning("not enough session instance !!!");
		return false;	
	}

	acceptor_.async_accept(
		ss_ptr.get()->socket(),
		boost::bind(
			&tcp_listener::handle_accept,
			this->shared_from_this(),
			ss_ptr.get(),
			boost::asio::placeholders::error
			)
		);

	return true;
}

// this function is all after async_accpt is done
//////////////////////////////////////////////////////////////////
void tcp_listener::handle_accept(session* ss_ptr, const boost::system::error_code& error) {
	if (!error)
	{
		logger::info("Connection is established~" );
		
		// session is working properly 
		ss_ptr->open();
		ss_ptr->post_recv();
		
		// waiting for next connection
		PostAccept();
	}
	else 
	{
		logger::error("An error is occured during accepting new connection");
		//std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}


