#include "session.hpp"
#include "server_ctrl.hpp"
#include <boost/make_shared.hpp>

// session constructor
//////////////////////////////////////////////////////////////////
session::session( boost::asio::io_service& io_service, unsigned short session_id)
	: strand_(boost::ref(io_service))
	, session_id_(session_id)
	, session_stat_(SS_CLOSE)
	, rcv_buff_start_(0)
{
	socket_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service));

	rcv_buff_ = nullptr;

} // end of session()

// open session with client  
//////////////////////////////////////////////////////////////////
bool session::open() {

	// socket has to be opened first before doing this. 
	if(!socket().is_open()) return false;

	// update session statement into SS_OPEN
	set_session_stat(SS_OPEN);
	
	Logger::info() << "Session is sucessfully open ID:" << session_id_ <<")"<< std::endl;

	return true;
} // end of open()


// open session with client  
//////////////////////////////////////////////////////////////////
void session::shutdown() {

	Logger::info() << "session::shutdown (ID:" << session_id_ <<")" <<std::endl;

	// need to fix 
	//try {
	//	socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);	
	//} catch (int ) {}

	if(get_session_stat() == SS_CLOSE) {
		return;
	}

	socket_->close();

	set_session_stat(SS_CLOSE);

	if(rcv_buff_ != nullptr) {
		server_ctrl::get().release_packet(rcv_buff_->get()->id_);
		rcv_buff_ = nullptr;
	}

} // end of shutdonw()


// set the buffer into io service in order to receive incoming data   
//////////////////////////////////////////////////////////////////
bool session::post_recv() {

	//Logger::info() << "session::post_recv() - BEGIN (ID:" << session_id_ <<")" << std::endl;

	if(rcv_buff_ == nullptr) {
		rcv_buff_ = &server_ctrl::get().alloc_packet();
	}

	rcv_buff_start_ = 0;

	// socket has to be opened first before doing this. 
	if(!socket().is_open()) return false;

	socket().async_read_some ( 
		boost::asio::buffer(rcv_buff_->get()->ptr_, MAX_PACKET_SIZE), 
		boost::bind( &session::handle_receive, 
					this->shared_from_this(), 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred ) 
	);

	return true;
} // end of post_recv()

// set the buffer into io service in order to receive incoming data   
//////////////////////////////////////////////////////////////////
bool session::post_recv(unsigned short start) {

	//Logger::info() << "session::post_recv() - start: " << start << "(ID:" << session_id_ << ")" << std::endl;

	// socket has to be opened first before doing this. 
	if(!socket().is_open()) return false;

	rcv_buff_start_ = start;

	socket().async_read_some ( 
		boost::asio::buffer(&rcv_buff_->get()->ptr_[rcv_buff_start_], MAX_PACKET_SIZE - rcv_buff_start_), 
		boost::bind( &session::handle_receive, 
					this->shared_from_this(), 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred ) 
	);

	return true;
} // end of post_recv()

// after receiving data, this function called. 
// do not forget to call post_recv() so that session can receive another data  
//////////////////////////////////////////////////////////////////
void session::handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	if( error || bytes_transferred == 0 )
	{
		if( error == boost::asio::error::eof )
		{
			Logger::warning() << "remote peer closed the connection (ID: " << session_id_ << ")"  << std::endl;
		}
		else 
		{
			Logger::warning() << "socket error is occured!!! (ID: " << session_id_ << ")" << std::endl;
		}

		boost::shared_ptr<session> this_ss_ptr = get();
		server_ctrl::get().release_session( session_id_ );
	}
	else
	{
		//Logger::info() << "session::handle_receive() bytes_transferred = " << bytes_transferred << std::endl;

#ifdef _TEST_UNDER_NO_PROTOCOL_

		post_send( rcv_buff_->get()->ptr_, bytes_transferred );
		post_recv();

#else /* _TEST_UNDER_NO_PROTOCOL_ */

		unsigned short packet_size_received = rcv_buff_start_ + bytes_transferred;
		unsigned short read_data = 0;

		while( packet_size_received > 0 )
		{
			if( packet_size_received < sizeof(PACKET_HEADER) ) 
			{
			break;
			}

			const PACKET_HEADER* header = (PACKET_HEADER*)&rcv_buff_->get()->ptr_[read_data];

			if( header->size_ <= packet_size_received )
			{
				process_packet( &rcv_buff_->get()->ptr_[read_data], header->size_);

				packet_size_received -= header->size_;
				read_data += header->size_;
			}
			else
			{
				break;
			}
		}

		if(packet_size_received) {

			assert(packet_size_received < MAX_PACKET_SIZE);

			boost::shared_ptr<PKT_UNIT> *buff =  &server_ctrl::get().alloc_packet();
			std::memcpy(buff->get()->ptr_, &rcv_buff_->get()->ptr_[read_data], packet_size_received );
			std::memcpy(rcv_buff_->get()->ptr_, buff->get()->ptr_, packet_size_received );
			server_ctrl::get().release_packet(buff->get()->id_);

			post_recv(packet_size_received);

		} else {

			post_recv();

		}

#endif /* _TEST_UNDER_NO_PROTOCOL_ */
		
	}
} // end of handle_receive()


// send data asyncrously  
//////////////////////////////////////////////////////////////////
bool session::post_send(const char* data, const unsigned short size) {

	//Logger::info() << "session::post_send() - BEGIN (ID:" << session_id_ <<")" << std::endl;

	// socket has to be opened first before doing this. 
	if(!socket().is_open()) return false;

	boost::asio::async_write( socket(), boost::asio::buffer( data, size ),
								strand_.wrap(
								 	boost::bind( &session::handle_send, this->shared_from_this(),
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred )
								 )
							);
	return true;

} // end of post_send()

// after sending is done, this function is called  
//////////////////////////////////////////////////////////////////
void session::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
	
	/* do nothing */

} // end of handle_send()


// analyze packet and do somehing like interacting with database  
//////////////////////////////////////////////////////////////////
void session::process_packet(const char* data, const unsigned short size) {

	/* example: echo server */
	//Logger::info() << "session::process_packet()" << std::endl;

	
	boost::shared_ptr<PKT_UNIT> *buff =  &server_ctrl::get().alloc_packet();

	std::memcpy(buff->get()->ptr_, data, size);
	
	//const PACKET_HEADER* header = (PACKET_HEADER*)buff->get()->ptr_;
	/* do something here !!! */
	
	// send data received
	post_send(buff->get()->ptr_, size);

	server_ctrl::get().release_packet(buff->get()->id_);
	
} // end of process_packet()


// end of file 