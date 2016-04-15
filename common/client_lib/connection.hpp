#pragma once

#include "../utils.hpp"

// const
//////////////////////////////////////////////////////////////////
const unsigned short CLIENT_PACKET_POOL_COUNT = 10; 			//10 might be enough to handle PACKET_POOL_COUNT

// Server Controller Class
//////////////////////////////////////////////////////////////////
class connection : private boost::noncopyable {

//---------------------- Member Functions ----------------------//
public:
	connection();
	~connection() {}

	unsigned short get_connection_stat(){
		return connection_stat_.load(boost::memory_order_acquire);
	}

	virtual bool init();
	void start();
	bool shutdown();									// shutdown socket

	// TCP/IP ////////////////////////////////////////////////////
	void connect(boost::asio::ip::tcp::endpoint endpoint);		// try to connect to server	
	bool post_send(const char* data, const unsigned short size);// send data to client
	bool post_send(boost::shared_ptr<PKT_UNIT>& packet, const unsigned short size);// send data to client

	// UDP/IP ////////////////////////////////////////////////////
	bool udp_on(std::string& addr, unsigned short port);
	bool post_udp_send(const char* data, const unsigned short size);// send data to client

	// packet pool management 
	boost::shared_ptr<PKT_UNIT>& alloc_packet();				//	allocate a packet
	void release_packet( unsigned short packet_unit_id );		//	release a packet 
	
protected:
	
	inline boost::asio::ip::tcp::socket& socket();

	// internal function to change session statement
	inline void set_connection_stat(unsigned short new_conn_stat) {	
		connection_stat_.store(new_conn_stat, boost::memory_order_release);
	}

	// TCP/IP ////////////////////////////////////////////////////
	bool post_recv();											// preparation for receiving data from client 
	bool post_recv(unsigned short start);						// adjust packet 

	virtual void handle_connect(const boost::system::error_code& error);// after trying to connect
	// handle received data after receiving data is done
	void handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred );
	// handle sonthing after sending data if needed
	void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred);

	// prcoess pacekt 
	virtual void process_packet(const char* data, const unsigned short size);

	// UDP/IP ////////////////////////////////////////////////////
	void post_udp_recv();
	virtual void handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred );
	//void handle_udp_send(const boost::system::error_code& error, std::size_t bytes_transferred);
	
//---------------------- Member Variables ----------------------//
public: 
	boost::asio::io_service ios_;								// async io 

protected:
	boost::asio::io_service::work work_;						// asio work

	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;	// socket 	
	boost::shared_ptr<boost::asio::ip::udp::socket> udp_socket_;// udp socket 

	boost::thread_group tg_;
	boost::atomic<unsigned short> connection_stat_;

	unsigned short udp_port_;
	std::string udp_addr_;
	boost::shared_ptr<PKT_UNIT> *rcv_udp_buff_;
	boost::asio::ip::udp::endpoint sender_endpoint_;

	// packet manager 
	std::vector<boost::shared_ptr<PKT_UNIT>> packet_list_;		// all packet instances 
	typedef boost::lockfree::queue< unsigned short, boost::lockfree::capacity< CLIENT_PACKET_POOL_COUNT > > packet_queue_t;
	packet_queue_t packet_queue_;								// each packet id ==> PKT_UNIT.id_

	boost::shared_ptr<PKT_UNIT> *rcv_buff_;
	unsigned short rcv_buff_start_;
};

// end of file 