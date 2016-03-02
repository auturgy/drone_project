#pragma once 

#include "utils.hpp"


class session;

// Server Controller Class
//////////////////////////////////////////////////////////////////
class server_ctrl : private boost::noncopyable {

//---------------- Type Defnitions, Enums and so on -------------//
	//typedef std::map<unsigned short, boost::shared_prt<de>>;

//---------------------- Member Functions ----------------------//
private:
	server_ctrl()
		:ios_()
		,work_(ios_) {}											// constructor

public:
	virtual ~server_ctrl() {}									// destructor (do nothing)
	static server_ctrl& get();									// singleton method
	std::size_t get_number_of_process();						// check the maximum number of CPUs

	bool init( const std::vector< boost::shared_ptr < session > >& sessions, const unsigned short max_num );
	bool start();
	bool stop();


	session& alloc_session();									// allocate new session for incoming connection
	void release_session();										// when connection is closed 

protected:

	void add_listeners(unsigned short port_num);
	bool remove_listeners(unsigned short port_num);

//---------------------- Member Variables ----------------------//
protected:
	boost::asio::io_service ios_;								// async io 
	boost::asio::io_service::work work_;						// asio work

	std::vector< boost::shared_ptr < session > > session_list_;
	std::deque< unsigned short > session_queue_;

private:
	static std::unique_ptr<server_ctrl> instance_;				// server controller

};

// end of code 