#include "server_ctrl.hpp"
#include "session.hpp"
#include <boost/make_shared.hpp>

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


// 
//////////////////////////////////////////////////////////////////
bool server_ctrl::init( 
	const std::vector< boost::shared_ptr < session > >& sessions = std::vector< boost::shared_ptr < session > >(), 
	const unsigned short max_num = 0 
	){

	if( max_num > 0 ) {

		logger::info("To process instances inhereted from session class");

	} else {

		logger::info("Default session class is used to handle each connection");

		for( int i = 0; i < MAX_SESSION_COUNT; ++i )
		{
			boost::shared_ptr<session> ss_ptr = boost::make_shared<session>(ios_, i);
			session_list_.push_back(ss_ptr);
			session_queue_.push_back(i);
		}
	}

	return true;
} // end of init()

// allocate new session for incoming connection
//////////////////////////////////////////////////////////////////
boost::shared_ptr<session> server_ctrl::alloc_session() {

	// update session manager 
	unsigned short session_id = session_queue_.front();
	session_queue_.pop_front();									// redundant 

	// session statement must be SS_CLOSE 
	assert(session_list_[session_id].get()->get_session_stat() == SS_CLOSE);

	session_list_[session_id].get()->set_waiting_mode();
	return session_list_[session_id];
} // end of alloc_session()  


// when connection is closed 
//////////////////////////////////////////////////////////////////
void server_ctrl::release_session( unsigned short session_id ) {

	//std::cout << "Å¬¶óÀÌ¾ðÆ® Á¢¼Ó Á¾·á. ¼¼¼Ç ID: " << nSessionID << std::endl;
	logger::info("release session");

	session_list_[session_id]->shutdown();
	session_queue_.push_back(session_id);

	//PostAccept();
	
} // end of release_session()  

// end of file 