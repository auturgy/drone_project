#include "session.hpp"
#include "server_ctrl.hpp"
#include <boost/make_shared.hpp>

// session constructor
//////////////////////////////////////////////////////////////////
session::session( boost::asio::io_service& io_service, unsigned short session_id)
	: session_id_(session_id)
	, session_stat_(SS_CLOSE)
	, rcv_buff_start_(0)
{
	socket_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service));

	rcv_buff_ = nullptr;
	proc_packet_ = nullptr;

	for( int i = 0; i < MAX_BUFF_NUM_ON_EACH_SESSION; i++ )
	{	
		boost::shared_ptr< char[] > ss_ptr = boost::make_shared<char[MAX_PACKET_SIZE]>();
		buf_queue_.push_back(ss_ptr);
	}

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

	if(buf_queue_.empty()){
		Logger::error() << "out of buffer queue" << std::endl;
		return false;
	} else {
		rcv_buff_ = buf_queue_.front(); 
	}

	rcv_buff_start_ = 0;

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	this->socket().async_read_some ( 
		boost::asio::buffer(rcv_buff_.get(), MAX_PACKET_SIZE), 
		boost::bind( &session::handle_receive, 
					this, 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred ) 
	);

	Logger::info() << "Session is ready to read" << std::endl;

	return true;
} // end of post_recv()

// set the buffer into io service in order to receive incoming data   
//////////////////////////////////////////////////////////////////
bool session::post_recv(unsigned short start, unsigned short size) {

	Logger::info() << "session::post_recv() - start: " << start << " size: " << size << std::endl;

	// socket has to be opened first before doing this. 
	if(!this->socket().is_open()) return false;

	rcv_buff_start_ = start;

	this->socket().async_read_some ( 
		boost::asio::buffer(&rcv_buff_.get()[rcv_buff_start_], size), 
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

	Logger::info() << "session::handle_receive() bytes_transferred = " << bytes_transferred << std::endl;

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

#ifdef _SIMPLE_ECHO_TEST_
		proc_packet_ = rcv_buff_;
		rcv_buff_ = nullptr;
		post_recv();

		post_send( proc_packet_.get(), bytes_transferred );

#else /* _SIMPLE_ECHO_TEST_ */

		unsigned short packet_size_received = rcv_buff_start_ + bytes_transferred;
		unsigned short loop_limit = 0; 

		while (packet_size_received > 0) {
		
			if( packet_size_received < sizeof(PACKET_HEADER) ) 
			{
				Logger::warning() << "size of received packet is not enough to handle" << std::endl;
				post_recv(packet_size_received, MAX_PACKET_SIZE - packet_size_received);
				return;
			}

			PACKET_HEADER* header = (PACKET_HEADER*)rcv_buff_.get();

			// 0 < PACKET received < MAX_PACKET_SIZE 
			if( header->size_  <= 0 || header->size_ > MAX_PACKET_SIZE ) {
				Logger::error() << "protocol error!!!! id: " << session_id_ << std::endl; 
				server_ctrl::get().release_session( session_id_ );
				return;
			}

			// normal case 
			if(packet_size_received == header->size_) {

				proc_packet_ = rcv_buff_;
				rcv_buff_ = nullptr;
				post_recv();

				// test !!!!
				post_send( proc_packet_.get(), packet_size_received );

			// not enough data to handle 
			} else if (packet_size_received < header->size_ ) {

				Logger::warning() << "need more data to pocess" << std::endl;
				post_recv(packet_size_received, header->size_ - packet_size_received);
				return;

			// split data into two because it is bigger than size in header 
			} else { 

				Logger::warning() << "unexpectedly bigger data is comming." << std::endl;
				proc_packet_ = rcv_buff_;
				rcv_buff_ = nullptr;

				if(buf_queue_.empty()){
					Logger::error() << "out of buffer queue" << std::endl;
					return;
				} else {
					rcv_buff_ = buf_queue_.front(); 
				}

				// make new rcv_buff able to take rest of data that is needed... 
				memcpy(rcv_buff_.get(), &proc_packet_.get()[header->size_], packet_size_received - header->size_ );
				
				// test !!!!
				post_send( proc_packet_.get(), header->size_ );
			}

			packet_size_received -= header->size_;

			// have to find the most efficient value, not just 10 
			if(loop_limit >= 10) {
				Logger::error() << "something is wrong~!!!!! id: " << session_id_ << std::endl;
				server_ctrl::get().release_session( session_id_ );
				return;
			}
			loop_limit++;
		}

#endif /* not _SIMPLE_ECHO_TEST_ */
		
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
	buf_queue_.push_back(proc_packet_);
	proc_packet_.get()[0] = 0x00;
	proc_packet_ = nullptr;
	
	Logger::info() << "session::handle_send() - do nothing at the moment" << std::endl;

} // end of handle_send()


// analyze packet and do somehing like interacting with database  
//////////////////////////////////////////////////////////////////
void session::process_packet(std::size_t bytes_transferred) {

	/* example: echo server */
	Logger::info() << "session::process_packet()" << std::endl;

} // end of process_packet()


// end of file 