#include "tcp_listener.hpp"
#include "session.hpp"
#include "server_ctrl.hpp"

// Post Accept methond to prepare for incoming connection 
//////////////////////////////////////////////////////////////////
bool tcp_listener::PostAccept() {
	if(!acceptor_.is_open()) {
		return false;
	}

	session ss = server_ctrl::get().alloc_session();

	acceptor_.async_accept(
		ss.socket(),
		boost::bind(
			&tcp_listener::handle_accept,
			this->shared_from_this(),
			&ss,
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
		
		//pSession->Init();
		//pSession->PostReceive();
		
		// waiting for next connection 
		PostAccept();
	}
	else 
	{
		logger::error("An error is occured during accepting new connection");
		//std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}


