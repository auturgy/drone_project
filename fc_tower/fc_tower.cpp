#include "../common/server_lib/server_ctrl.hpp"
#include "client.hpp"
typedef singleton<server_ctrl> server_singleton; 

int main(int argc, char* argv[]){


	server_singleton::get().init();
	
	// pause here until server is shutdown
	server_singleton::get().start(); 
	

	return 0;
}