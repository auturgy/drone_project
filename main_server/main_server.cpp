#include "../common/server_lib/server_ctrl.hpp"
#include "client.hpp"

int main(int argc, char* argv[]){




	server_ctrl::get().init();
	
	// pause here until server is shutdown
	server_ctrl::get().start(); 
	

	return 0;
}