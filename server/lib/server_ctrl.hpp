#pragma once 

#include "utils.hpp"

class session;
class tcp_listener;

// Server Controller Class
//////////////////////////////////////////////////////////////////
class server_ctrl : private boost::noncopyable {

//---------------- Type Defnitions, Enums and so on -------------//
	//typedef std::map<unsigned short, boost::shared_prt<de>>;

//---------------------- Member Functions ----------------------//
private:
	server_ctrl();												// constructor

public:
	virtual ~server_ctrl() {}									// destructor (do nothing)
	static server_ctrl& get();									// singleton method
	std::size_t get_number_of_process();						// check the maximum number of CPUs

	bool init();												// initialize server 
	bool init( 
		const std::vector< boost::shared_ptr < session > >& sessions, 
		const unsigned short max_num 
		);

	virtual bool start();										// start server
	bool stop();												// stop server

	boost::shared_ptr<session> alloc_session();					// allocate new session for incoming connection
	// when connection is closed
	void release_session( boost::shared_ptr<session> ss_ptr );
	//void release_session( unsigned short session_id );		


	// packet pool management 
	boost::shared_ptr<char[]> alloc_packet();					
	void release_packet( boost::shared_ptr<char[]> packet );

	void add_listener(unsigned short port_num);					// add listener with dedicated port number 								

protected:

	void add_listeners(unsigned short port_num);
	bool remove_listeners(unsigned short port_num);

//---------------------- Member Variables ----------------------//
protected:
	boost::asio::io_service ios_;								// async io 
	boost::asio::io_service::work work_;						// asio work

	//boost::shared_ptr<tcp_listener> listener_;				// listener 

	typedef std::map<unsigned short, boost::shared_ptr<tcp_listener>> listeners_map_t;
	listeners_map_t listeners_;									// listeners to accept in various port

	// session manager 
	//std::vector<boost::shared_ptr<session>> session_list_;		// all session instances
	//std::deque<unsigned short> session_queue_;					// session IDs not in use.

	ringbuffer<boost::shared_ptr<session>, MAX_SESSION_COUNT+1> session_new_list_;

	ringbuffer<boost::shared_ptr<char[]>, PACKET_POOL_COUNT+1> packet_pool_;

private:
	static std::unique_ptr<server_ctrl> instance_;				// server controller

};

// end of code 