#include "connection.hpp"
#include <boost/make_shared.hpp>

// singletone implementation
//////////////////////////////////////////////////////////////////
std::unique_ptr<connection> connection::instance_;


// singletone 
//////////////////////////////////////////////////////////////////
connection& connection::get() {

	connection *p = instance_.get();

	if(!p) {
		instance_.reset(new connection);
		p = instance_.get();
	}

	return *p;
} // end of get()


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


// 
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


// shutdown socket
//////////////////////////////////////////////////////////////////
bool connection::shutdown(){

	std::cout << "client controller is shutdown" << std::endl;

	ios_.stop();

    if( socket().is_open() )
	{
		socket().close();
	}

	//thread_.join();
  
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

	std::cout << "client is running in single-thread mode" << std::endl;

	// run io service
	//ios_.run();
	thread_ = boost::thread( boost::bind(&boost::asio::io_service::run, &ios_) );

} // end of connect 

// after trying to connect
//////////////////////////////////////////////////////////////////
void connection::handle_connect(const boost::system::error_code& error){

	if (!error) {	

		std::cout << "connected successfully" << std::endl;
		post_recv();

		set_connection_stat(SS_OPEN);

	} else {

		std::cout << "handle_connect error No: " << error.value() << std::endl;
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
			std::cout << "remote peer closed the connection " << std::endl;
		}
		else 
		{
			std::cout  << "socket error is occured!!!"  << std::endl;
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

	std::cout << "[received] " << packet_p->buffer_ << std::endl;
	
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

	udp_socket_ = boost::make_shared<boost::asio::ip::udp::socket>(
					boost::ref(ios_),
					boost::asio::ip::udp::endpoint( 
						boost::asio::ip::udp::v4(), 
						udp_port_
					) 
				);

	post_udp_recv();

	// make another thread if tcp service already runs
	thread_ = boost::thread( boost::bind(&boost::asio::io_service::run, &ios_) );

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


// send data through UDP
//////////////////////////////////////////////////////////////////
bool connection::post_udp_send(const char* data, const unsigned short size) {

	std::cout << "post_udp_send - address: " << udp_addr_ << "| port: " << udp_port_ << std::endl;
	
	udp_socket_->async_send_to( 
		boost::asio::buffer(data, size),
		boost::asio::ip::udp::endpoint( 
			boost::asio::ip::address::from_string(udp_addr_), 
			udp_port_ 
		),
		boost::bind( 
			&connection::handle_udp_send, 
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
			)
	);

	return true;

} // end of post_udp_send function 


// send data through UDP with packet structure 
//////////////////////////////////////////////////////////////////
bool connection::post_udp_send(boost::shared_ptr<PKT_UNIT>& packet, const unsigned short size) {

	post_udp_send( packet.get()->ptr_, size);
	release_packet(packet.get()->id_);

	return true;

} // end of post_udp_send


// handle sonthing after sending data if needed
//////////////////////////////////////////////////////////////////
void connection::handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	std::cout << "[received] " << rcv_udp_buff_->get()->ptr_ << std::endl;

	// echo as an example 
	//post_udp_send(rcv_udp_buff_->get()->ptr_, bytes_transferred);

	post_udp_recv();

} // end of handle_udp_receive function 


// after sending procedure is done 
//////////////////////////////////////////////////////////////////
void connection::handle_udp_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
	// do nothing
} // end of handle_udp_send function 


// end of file