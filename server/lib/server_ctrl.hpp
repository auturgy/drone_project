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
	void release_session( unsigned short session_id );			// when connection is closed

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
	std::vector<boost::shared_ptr<session>> session_list_;		// all session instances
	std::deque<unsigned short> session_queue_;					// session IDs not in use. 

private:
	static std::unique_ptr<server_ctrl> instance_;				// server controller

};

// end of code 