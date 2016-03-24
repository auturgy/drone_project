#pragma once

#include "../server_lib/utils.hpp"


// const
//////////////////////////////////////////////////////////////////
const unsigned short CLIENT_PACKET_POOL_COUNT = 10; 			//10 might be enough to handle PACKET_POOL_COUNT

// Server Controller Class
//////////////////////////////////////////////////////////////////
class connection : private boost::noncopyable {

//---------------------- Member Functions ----------------------//
public:
	~connection() {}
	static connection& get();

	unsigned short get_connection_stat(){
		return connection_stat_.load(boost::memory_order_acquire);
	}

	bool init();

	bool shutdown();											// shutdown socket
	void connect(boost::asio::ip::tcp::endpoint endpoint);		// try to connect to server
	
	bool post_send(const char* data, const unsigned short size);// send data to client
	bool post_send(boost::shared_ptr<PKT_UNIT>& packet, const unsigned short size);// send data to client

	// packet pool management 
	boost::shared_ptr<PKT_UNIT>& alloc_packet();				//	allocate a packet
	void release_packet( unsigned short packet_unit_id );		//	release a packet 

protected:
	connection();
	
	inline boost::asio::ip::tcp::socket& socket();

	// internal function to change session statement
	inline void set_connection_stat(unsigned short new_conn_stat) {	
		connection_stat_.store(new_conn_stat, boost::memory_order_release);
	}

	bool post_recv();											// preparation for receiving data from client 
	bool post_recv(unsigned short start);						// adjust packet 

	void handle_connect(const boost::system::error_code& error);// after trying to connect
	// handle received data after receiving data is done
	void handle_receive( const boost::system::error_code& error, std::size_t bytes_transferred );
	// handle sonthing after sending data if needed
	void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred);

	// prcoess pacekt 
	virtual void process_packet(const char* data, const unsigned short size);
	
//---------------------- Member Variables ----------------------//
public: 
	boost::asio::io_service ios_;								// async io 

protected:
	boost::asio::io_service::work work_;						// asio work

	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;	// socket 	
	boost::thread thread_;
	boost::atomic<unsigned short> connection_stat_;

	// packet manager 
	std::vector<boost::shared_ptr<PKT_UNIT>> packet_list_;		// all packet instances 
	typedef boost::lockfree::queue< unsigned short, boost::lockfree::capacity< CLIENT_PACKET_POOL_COUNT > > packet_queue_t;
	packet_queue_t packet_queue_;								// each packet id ==> PKT_UNIT.id_

	boost::shared_ptr<PKT_UNIT> *rcv_buff_;
	unsigned short rcv_buff_start_;

	static std::unique_ptr<connection> instance_;				// class instance
};

// end of file 