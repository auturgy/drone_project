#include "server_ctrl.hpp"
#include "session.hpp"
#include "tcp_listener.hpp"
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>


// get the number of process on server 
//////////////////////////////////////////////////////////////////
std::unique_ptr<server_ctrl> server_ctrl::instance_;

server_ctrl& server_ctrl::get(){

	server_ctrl *p = instance_.get();
	
	if(!p) {
		instance_.reset(new server_ctrl);
		p = instance_.get();
	}

	return *p;
}


// constructor
//////////////////////////////////////////////////////////////////
server_ctrl::server_ctrl()
	:ios_()
	,work_(ios_) 
{

}											


// get the number of process on server 
//////////////////////////////////////////////////////////////////
std::size_t server_ctrl::get_number_of_process() {

	std::size_t max_thread_cnt = 0;

	max_thread_cnt =  (std::max) (static_cast<int>(
		boost::thread::hardware_concurrency()),1
		);

#ifdef OPTIMIZATION_FLAG 	
	max_thread_cnt = max_thread_cnt * 2 + 1; // recommended by a network programming expert... not sure 
#else
	--max_thread_cnt; // main process is already running 
#endif 

	return max_thread_cnt;

} // end of get_number_of_process()


// default init() function 
//////////////////////////////////////////////////////////////////
bool server_ctrl::init(){

	Logger::info() << "Server init - Default session class is used to handle each connection" << std::endl;

	int i;

	for( i = 0 ; i < MAX_SESSION_COUNT ; i++ )
	{
		boost::shared_ptr<session> ss_ptr = boost::make_shared<session>(ios_, i);
		//session_list_.push_back(ss_ptr);
		//session_queue_.push_back(i);
		session_new_list_.push(ss_ptr);
	}

	for( i = 0; i < PACKET_POOL_COUNT ; i++ )
	{
		boost::shared_ptr<char[]> packet_ptr = boost::make_shared<char[MAX_PACKET_SIZE]>();
		packet_pool_.push(packet_ptr);
	}

	return true;
} // end of init()


// init() function to take extend session class in 
//////////////////////////////////////////////////////////////////
bool server_ctrl::init( const std::vector< boost::shared_ptr < session > >& sessions, 
						const unsigned short max_num) 
{
	Logger::info() << "Server Init - To process instances inhereted from session class" << std::endl;

	// add codes to handle this~~~!!! 

	return true;
} // end of init()


// start server
//////////////////////////////////////////////////////////////////
bool server_ctrl::start() {

	Logger::info() << "server_ctrl::start" << std::endl;
	
	// run listener
	add_listener(DEFAULT_PORT_NUMBER);

	Logger::info() << "Server is running" << std::endl;
	// run io service
	ios_.run();	

	return true;
} // end of start()


// turn off server
//////////////////////////////////////////////////////////////////
bool server_ctrl::stop() {

	//logger::info("Server is turning off");
	Logger::info() << "Server is shutdown" << std::endl;

	// do something 	

	return true;
} // end fo stop()


// allocate new session for incoming connection
//////////////////////////////////////////////////////////////////
boost::shared_ptr<session> server_ctrl::alloc_session() {

	/*
	// update session manager 
	unsigned short session_id = session_queue_.front();
	Logger::info() << "allocate session, ID = " << session_id << std::endl;
	session_queue_.pop_front();									// redundant 
	
	// session statement must be SS_CLOSE 
	assert(session_list_[session_id].get()->get_session_stat() == SS_CLOSE);

	session_list_[session_id].get()->set_waiting_mode();

	return session_list_[session_id];
	*/
	boost::shared_ptr<session> ss_ptr = session_new_list_.pop();

	Logger::info() << "allocate session, ID = " << ss_ptr.get()->session_id() << std::endl;

	// session statement must be SS_CLOSE 
	assert(ss_ptr.get()->get_session_stat() == SS_CLOSE);
	ss_ptr.get()->set_waiting_mode();

	return ss_ptr;
} // end of alloc_session()  


// when connection is closed 
//////////////////////////////////////////////////////////////////
void server_ctrl::release_session( boost::shared_ptr<session> ss_ptr ) {

	Logger::info() << "release session id: " << ss_ptr.get()->session_id() << std::endl;

	ss_ptr.get()->shutdown();
	session_new_list_.push(ss_ptr);

} // end of release_session()  

/*
// when connection is closed 
//////////////////////////////////////////////////////////////////
void server_ctrl::release_session( unsigned short session_id ) {

	Logger::info() << "release session, ID = " << session_id << std::endl;

	session_list_[session_id].get()->shutdown();
	session_queue_.push_back(session_id);

} // end of release_session()
*/

// allocate a new packet for receive/process/send operation in session 
//////////////////////////////////////////////////////////////////
boost::shared_ptr<char[]> server_ctrl::alloc_packet() {

	Logger::info() << "allocate packet" << std::endl;

	boost::shared_ptr<char[]> packet_ptr = packet_pool_.pop();

	return packet_ptr;
} // end of alloc_packet()  


// released packet get back to packet pool  
//////////////////////////////////////////////////////////////////
void server_ctrl::release_packet( boost::shared_ptr<char[]> packet_ptr) {

	Logger::info() << "release packet" << std::endl;

	packet_ptr.get()[0] = 0x00; // kind of init 

	packet_pool_.push(packet_ptr);

	//packet_ptr = nullptr;

} // end of release_packet() 

// add listener
//////////////////////////////////////////////////////////////////
void server_ctrl::add_listener(unsigned short port_num){

	Logger::info() << "server_ctrl::add_listener (port:" << port_num <<") - BEGIN" << std::endl;

	listeners_map_t::const_iterator it = listeners_.find(port_num);
	if(it != listeners_.end()) {
		throw std::logic_error(
			"Such listener for port '"
			+ boost::lexical_cast<std::string>(port_num)
			+ "' already created"
		);	
	}

	listeners_[port_num] = boost::make_shared<tcp_listener>(boost::ref(ios_), port_num);
	listeners_[port_num].get()->start_listening();
}


// end of file 