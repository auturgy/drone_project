#include "connection.hpp"
#include <boost/make_shared.hpp>
#include <boost/exception/all.hpp>

typedef singleton<logger> logger_singleton; 

// constructor 
//////////////////////////////////////////////////////////////////
connection::connection()
	:ios_()
	,work_(ios_) 
	,rcv_udp_buff_(nullptr)
	,rcv_buff_(nullptr)
	,rcv_buff_start_(0) 
{
	socket_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(ios_));
	
	set_connection_stat(SS_CLOSE);
} // end of contructor 


// socket instance 
//////////////////////////////////////////////////////////////////
inline boost::asio::ip::tcp::socket& connection::socket(){
	return *socket_.get(); 
} // end of socket()


// function init()
//////////////////////////////////////////////////////////////////
bool connection::init(){
	for(int i = 0; i < CLIENT_PACKET_POOL_COUNT ; i++ )
	{
		boost::shared_ptr<PKT_UNIT> pkt_unit_ptr = boost::make_shared<PKT_UNIT>(i);

		packet_list_.push_back(pkt_unit_ptr);
		packet_queue_.push(i);
		
	}
	return true;
} // end of init()

// function start()
//////////////////////////////////////////////////////////////////
void connection::start(){
	ios_.run();	
	tg_.join_all();
}

// shutdown socket
//////////////////////////////////////////////////////////////////
bool connection::shutdown(){

	logger_singleton::get().info() << "client controller is shutdown" << std::endl;

    if( socket().is_open() )
	{
		socket().close();
	}

	// in case of udp communication 
	if(!udp_socket_.get()) {
		ios_.stop();
	}
  
  	set_connection_stat(SS_CLOSE);
	return true;
} // end of shutdown()  


// allocate a new packet for receive/process/send operation in client controller  
//////////////////////////////////////////////////////////////////
boost::shared_ptr<PKT_UNIT>& connection::alloc_packet() {

	// update packet manager 
	unsigned short pkt_unit_id;
	assert(packet_queue_.pop(pkt_unit_id));

	return packet_list_[pkt_unit_id];

} // end of alloc_packet()  


// released packet get back to packet pool  
//////////////////////////////////////////////////////////////////
void connection::release_packet( unsigned short packet_unit_id ) {

	assert(packet_queue_.push(packet_unit_id));

} // end of release_packet() 


// connect to server
//////////////////////////////////////////////////////////////////
void connection::connect( boost::asio::ip::tcp::endpoint endpoint )
{	
	socket().async_connect( 
		endpoint,
		boost::bind(&connection::handle_connect, 
					this,
					boost::asio::placeholders::error)
	);
	set_connection_stat(SS_WAITING);

	tg_.create_thread(
		boost::bind(
			&boost::asio::io_service::run, 
			boost::ref(ios_)
		)
	);

} // end of connect 

// after trying to connect
//////////////////////////////////////////////////////////////////
void connection::handle_connect(const boost::system::error_code& error){

	if (!error) {	

		logger_singleton::get().info() << "connected successfully" << std::endl;
		post_recv();

		set_connection_stat(SS_OPEN);		

	} else {

		logger_singleton::get().warning() << "handle_connect error No: " << error.value() << std::endl;
		shutdown();
	}
} // end of handle_connect()


// preparation for receiving data from client 
//////////////////////////////////////////////////////////////////
bool connection::post_recv() {
	
	if(rcv_buff_ == nullptr) {
		rcv_buff_ = &alloc_packet();
	}

	rcv_buff_start_ = 0;

	// socket has to be opened first before doing this. 
	if(!socket().is_open()) return false;

	socket().async_read_some ( 
		boost::asio::buffer(rcv_buff_->get()->ptr_, MAX_PACKET_SIZE), 
		boost::bind( &connection::handle_receive, 
					this, 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred ) 
	);

	return true;
} // end fo post_recv()


// adjust packet 
//////////////////////////////////////////////////////////////////
bool connection::post_recv(unsigned short start) {
	
	// socket has to be opened first before doing this. 
	if(!socket().is_open()) return false;

	rcv_buff_start_ = start;

	socket().async_read_some ( 
		boost::asio::buffer(&rcv_buff_->get()->ptr_[rcv_buff_start_], MAX_PACKET_SIZE - rcv_buff_start_), 
		boost::bind( &connection::handle_receive, 
					this, 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred ) 
	);

	return true;
} // end of post_recv()

// handle received data after receiving data is done
//////////////////////////////////////////////////////////////////
void connection::handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred ){
	if( error || bytes_transferred == 0 )
	{
		if( error == boost::asio::error::eof )
		{
			logger_singleton::get().warning() << "remote peer closed the connection " << std::endl;
		}
		else 
		{
			logger_singleton::get().warning()  << "socket error is occured!!!"  << std::endl;
		}

		shutdown();
	}
	else
	{
		//Logger::info() << "session::handle_receive() bytes_transferred = " << bytes_transferred << std::endl;

#ifdef _SIMPLE_ECHO_SERVER_

		post_send( rcv_buff_->get()->ptr_, bytes_transferred );
		post_recv();

#else /* _SIMPLE_ECHO_SERVER_ */

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

			if(packet_size_received > MAX_PACKET_SIZE) {
				shutdown();
				return;
			}

			boost::shared_ptr<PKT_UNIT> *buff =  &alloc_packet();
			std::memcpy(buff->get()->ptr_, &rcv_buff_->get()->ptr_[read_data], packet_size_received );
			std::memcpy(rcv_buff_->get()->ptr_, buff->get()->ptr_, packet_size_received );
			release_packet(buff->get()->id_);

			post_recv(packet_size_received);

		} else {

			post_recv();

		}
#endif /* _TEST_UNDER_NO_PROTOCOL_ */
	}
} // end of handle_receive()

// send data to client
//////////////////////////////////////////////////////////////////
bool connection::post_send(const char* data, const unsigned short size) {

	// socket has to be opened first before doing this. 
	if(!socket().is_open()) return false;

	boost::asio::async_write( 
		socket(), 
		boost::asio::buffer( data, size ),
	 	boost::bind( 
	 		&connection::handle_send, 
		 	this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred 
		)
	);

	return true;
}  // end of post_send()


// send data to client
//////////////////////////////////////////////////////////////////
bool connection::post_send(boost::shared_ptr<PKT_UNIT>& packet, const unsigned short size){

	post_send( packet.get()->ptr_, size);
	release_packet(packet.get()->id_);

	return true;

} // end of post_send()


// handle sonthing after sending data if needed
//////////////////////////////////////////////////////////////////
void connection::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
	// do nothing here
} // end of handle_send()


// analyze packet and do somehing like interacting with database  
//////////////////////////////////////////////////////////////////
void connection::process_packet(const char* data, const unsigned short size) {
	
	boost::shared_ptr<PKT_UNIT>& buff = alloc_packet();
	std::memset(buff.get()->ptr_, 0, MAX_RECEIVE_BUFFER_LEN);
	std::memcpy(buff.get()->ptr_, data, size);
	
	/* do something here !!! */

	PKT_REQ_TEST* packet_p = (PKT_REQ_TEST*)buff.get()->ptr_;

	std::cout << "[TCP-Recv] " << packet_p->buffer_ << std::endl;
	
	// send data 
	//post_send(buff.get()->ptr_, size);

	release_packet(buff.get()->id_);

} // end of process_packet()


// UDP/IP ////////////////////////////////////////////////////

// udp connection on 
//////////////////////////////////////////////////////////////////
bool connection::udp_on(std::string& addr, unsigned short port) {

	udp_port_ = port;
	udp_addr_ = addr;

	if(rcv_udp_buff_ == nullptr) {
		rcv_udp_buff_ = &alloc_packet();
	}

	try {
		udp_socket_ = boost::make_shared<boost::asio::ip::udp::socket>(
						boost::ref(ios_),
						boost::asio::ip::udp::endpoint( 
							boost::asio::ip::udp::v4(), 
							udp_port_
						) 
					);
	}
	catch (boost::exception &e)
	{
		logger_singleton::get().error()  << "UDP socket on port " << udp_port_ << " is already open" << std::endl;
		return false;
	}
	
	post_udp_recv();

	// make an another thread for faster udp operation 
	tg_.create_thread(
		boost::bind(
			&boost::asio::io_service::run, 
			boost::ref(ios_)
		)
	);

	return true;

} // end of udp_on function 


// handle sonthing after sending data if needed
//////////////////////////////////////////////////////////////////
void connection::post_udp_recv() {

	udp_socket_->async_receive_from( 
		boost::asio::buffer(rcv_udp_buff_->get()->ptr_, MAX_PACKET_SIZE), 
		sender_endpoint_, 	
		boost::bind( 
			&connection::handle_udp_receive, 
			this, 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred 
		) 
	); 

} // end of post_udp_recv function 


// handle sonthing after sending data if needed
//////////////////////////////////////////////////////////////////
void connection::handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	std::cout << "[UDP-Recv] " << rcv_udp_buff_->get()->ptr_ << std::endl;

	post_udp_recv();

} // end of handle_udp_receive function 


// send data through UDP
//////////////////////////////////////////////////////////////////
bool connection::post_udp_send(const char* data, const unsigned short size) {

	//logger_singleton::get().info() << "post_udp_send - address: " << udp_addr_ << "| port: " << udp_port_ << std::endl;
	
	if(!udp_socket_.get()) return false;

	udp_socket_->async_send_to( 
		boost::asio::buffer(data, size),
		boost::asio::ip::udp::endpoint( 
			boost::asio::ip::address::from_string(udp_addr_), 
			udp_port_ 
		),
		[](const boost::system::error_code& error, std::size_t bytes_transferred){}
		/*
		boost::bind( 
			&connection::handle_udp_send, 
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
			)
		*/
	);

	return true;

} // end of post_udp_send function 
/*
// after sending procedure is done 
//////////////////////////////////////////////////////////////////
void connection::handle_udp_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
	// do nothing
} // end of handle_udp_send function 
*/

// end of file