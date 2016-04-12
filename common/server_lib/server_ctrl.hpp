#pragma once 

#include "../utils.hpp"

class session;
class tcp_listener;

// Server Controller Class
//////////////////////////////////////////////////////////////////
class server_ctrl : private boost::noncopyable {

//---------------- Type Defnitions, Enums and so on -------------//
	//typedef std::map<unsigned short, boost::shared_prt<de>>;

//---------------------- Member Functions ----------------------//
public:
	server_ctrl();												// constructor
	virtual ~server_ctrl() {}									// destructor (do nothing)

	std::size_t get_number_of_process();						// check the maximum number of CPUs

	bool init();												// initialize server 
	bool init( 
		const std::vector< boost::shared_ptr < session > >& sessions, 
		const unsigned short max_num 
		);

	virtual bool start();										// start server
	bool stop();												// stop server

	boost::shared_ptr<session>& alloc_session();				// allocate new session for incoming connection
	boost::shared_ptr<session>* alloc_session_p();
	// when connection is closed
	//void release_session( boost::shared_ptr<session> &ss_ptr );
	void release_session( unsigned short session_id );		

	// packet pool management 
	boost::shared_ptr<PKT_UNIT>& alloc_packet();					
	void release_packet( unsigned short packet_unit_id );

protected:

	// add listener with dedicated port number
	void add_listener(unsigned short port_num, std::size_t concur_listener_num );
	bool remove_listeners(unsigned short port_num);

//---------------------- Member Variables ----------------------//
public:
	boost::asio::io_service ios_;								// async io 

protected:
	boost::asio::io_service::work work_;						// asio work

	typedef std::map<unsigned short, boost::shared_ptr<tcp_listener>> listeners_map_t;
	listeners_map_t listeners_;									// listeners to accept in various port

	// session manager 
	std::vector<boost::shared_ptr<session>> session_list_;		// all session instances
	typedef boost::lockfree::queue< unsigned short, boost::lockfree::capacity< MAX_SESSION_COUNT > > session_queue_t;
	session_queue_t session_queue_;								// session IDs

	// packet manager 
	std::vector<boost::shared_ptr<PKT_UNIT>> packet_list_;		// all packet instances 
	typedef boost::lockfree::queue< unsigned short, boost::lockfree::capacity< PACKET_POOL_COUNT > > packet_queue_t;
	packet_queue_t packet_queue_;								// each packet id ==> PKT_UNIT.id_

};

// end of code 