#include "server_ctrl.hpp"

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


	}

	return true;
} // end of init()



// end of file 