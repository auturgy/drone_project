#include "tcp_listener.hpp"
#include "session.hpp"
#include "server_ctrl.hpp"

// Post Accept methond to prepare for incoming connection 
//////////////////////////////////////////////////////////////////
bool tcp_listener::PostAccept() {
	if(!acceptor_.is_open()) {
		return false;
	}

	Logger::info() << "tcp_listener::PostAccept() - BEGIN" << std::endl;

	boost::shared_ptr<session> ss_ptr = server_ctrl::get().alloc_session();

	if(ss_ptr == nullptr) {

		Logger::warning() << "not enough session instance !!!" << std::endl;

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

	Logger::info() << "Listen...." << std::endl;

	return true;
} // end of PostAccept()


// this function is all after async_accpt is done
//////////////////////////////////////////////////////////////////
void tcp_listener::handle_accept(session* ss_ptr, const boost::system::error_code& error) {
	if (!error)
	{
		Logger::info() << "Connection is established~...." << std::endl;
		
		// session is working properly 
		ss_ptr->open();
		ss_ptr->post_recv();
		
		// waiting for next connection
		PostAccept();
	}
	else 
	{
		Logger::error() << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
} // end of handle_accept()

// end of file 