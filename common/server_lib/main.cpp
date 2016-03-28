#ifdef _SERVER_TEST_

#include "server_ctrl.hpp"


int main() {

	//std::cout << "CPU number: " << server_ctrl::get().get_number_of_process() << std::endl;
	
	server_ctrl::get().init();
	
	// pause here until server is shutdown
	server_ctrl::get().start(); 
	
	//server_ctrl::get().stop();
	
	getchar();
	return 0;
}

#endif /* _SERVER_TEST_ */