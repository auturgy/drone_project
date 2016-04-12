#ifdef _SERVER_TEST_

#include "server_ctrl.hpp"
typedef singleton<server_ctrl> server_singleton; 

int main() {

	//std::cout << "CPU number: " << server_ctrl::get().get_number_of_process() << std::endl;
	
	server_singleton::get().init();
	
	// wait here until server is shutdown
	server_singleton::get().start(); 
	
	//server_singleton::get().stop();
	
	std::getchar();
	return 0;
}

#endif /* _SERVER_TEST_ */