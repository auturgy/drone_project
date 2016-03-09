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
} // end of session()


/*
session::session (
	boost::asio::io_service& ios,
	const boost::asio::ip::tcp::endpoint& endpoint
	)
		:socket_(boost::shared_ptr<boost::asio::ip::tcp::socket>(
			boost::ref(ios)))
{
	socket_->connect(endpoint);
} // end of session()
*/


// open session with client  
//////////////////////////////////////////////////////////////////
bool session::open() {

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	// update session statement into SS_OPEN
	set_session_stat(SS_OPEN);
	
	Logger::info() << "Session is sucessfully open" << std::endl;

	return true;
} // end of open()


// open session with client  
//////////////////////////////////////////////////////////////////
void session::shutdown() {

	Logger::info() << "session::shutdown() - BEGIN" << std::endl;

	// need to fix 
	//try {
	//	socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);	
	//} catch (int ) {}

	socket_->close();

	set_session_stat(SS_CLOSE);

	Logger::info() << "Session is shut down" << std::endl;

} // end of shutdonw()


// set the buffer into io service in order to receive incoming data   
//////////////////////////////////////////////////////////////////
bool session::post_recv() {

	Logger::info() << "session::post_recv() - BEGIN" << std::endl;

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	this->socket().async_read_some ( 
		boost::asio::buffer(rcv_buff_), 
		boost::bind( &session::handle_receive, 
					this, 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred ) 
	);

	Logger::info() << "Session is ready to read" << std::endl;

	return true;
} // end of post_recv()


// after receiving data, this function called. 
// do not forget to call post_recv() so that session can receive another data  
//////////////////////////////////////////////////////////////////
void session::handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	Logger::info() << "session::handle_receive() - BEGIN" << std::endl;

	if( error )
	{
		if( error == boost::asio::error::eof )
		{
			Logger::info() << "remote peer closed the connection" << std::endl;
		}
		else 
		{
			Logger::info() << "socket error is occured!!!" << std::endl;
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
} // end of handle_receive()


// send data asyncrously  
//////////////////////////////////////////////////////////////////
bool session::post_send(const char* data, const unsigned short size) {

	Logger::info() << "session::post_send() - BEGIN" << std::endl;

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	boost::asio::async_write( this->socket(), boost::asio::buffer( data, size ),
							 	boost::bind( &session::handle_send, this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred )
							);

	return true;
} // end of post_send()


// after sending is done, this function is called  
//////////////////////////////////////////////////////////////////
void session::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {

	/* do nothing at the moment */
	Logger::info() << "session::handle_send() - do nothing at the moment" << std::endl;

} // end of handle_send()


// analyze packet and do somehing like interacting with database  
//////////////////////////////////////////////////////////////////
void session::process_packet(std::size_t bytes_transferred) {

	/* example: echo server */
	Logger::info() << "session::process_packet()" << std::endl;

} // end of process_packet()


// end of file 