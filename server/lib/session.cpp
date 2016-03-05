#include "session.hpp"
#include "server_ctrl.hpp"
#include <boost/make_shared.hpp>


// session constructor
//////////////////////////////////////////////////////////////////
session::session( boost::asio::io_service& io_service, unsigned short session_id)
	: session_id_(session_id)
	, session_stat_(SS_CLOSE)
{
	socket_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service));
}

/*
session::session (
	boost::asio::io_service& ios,
	const boost::asio::ip::tcp::endpoint& endpoint
	)
		:socket_(boost::shared_ptr<boost::asio::ip::tcp::socket>(
			boost::ref(ios)))
{
	socket_->connect(endpoint);
}
*/

// open session with client  
//////////////////////////////////////////////////////////////////
bool session::open() {

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	// update session statement into SS_OPEN
	set_session_stat(SS_OPEN);

	logger::info("Session is sucessfully open." );	
	return true;
}

void session::shutdown() {
	socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket_->close();

	set_session_stat(SS_CLOSE);

	logger::info("Session is shut down" );
}

// set the buffer into io service in order to receive incoming data   
//////////////////////////////////////////////////////////////////
bool session::post_recv() {

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	this->socket().async_read_some ( 
		boost::asio::buffer(rcv_buff_), 
		boost::bind( &session::handle_receive, 
					this, 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred ) 
	);


	logger::info("Session is ready to read" );
	return true;
}

//  
//////////////////////////////////////////////////////////////////
void session::handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {
	if( error )
	{
		if( error == boost::asio::error::eof )
		{
			logger::info("remote peer closed the connection");
		}
		else 
		{
			logger::error("socket error is occured!!!");
		}

		server_ctrl::get().release_session( session_id_ );
	}
	else
	{
		/* do somehing here */
		process_packet(bytes_transferred);

		// reset io service to receive data
		post_recv();
	}
}

//  
//////////////////////////////////////////////////////////////////
bool session::post_send(const char* data, const unsigned short size) {

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	boost::asio::async_write( this->socket(), boost::asio::buffer( data, size ),
							 	boost::bind( &session::handle_send, this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred )
							);

	return true;
}

//  
//////////////////////////////////////////////////////////////////
void session::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {

	/* do nothing at the moment */
	logger::info("do nothing at the moment");
}

//  
//////////////////////////////////////////////////////////////////
void session::process_packet(std::size_t bytes_transferred) {

	/* example: echo server */
	logger::info("process packet");


}

