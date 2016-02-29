#include "server_ctrl.hpp"
#include <boost/thread/thread.hpp>

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



// end of file 