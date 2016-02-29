#pragma once 

#include "utils.hpp"

// Server Controller Class
//////////////////////////////////////////////////////////////////
class server_ctrl : public singleton<server_ctrl> {

//---------------------- Member Functions ----------------------//
private:
	

public:
	server_ctrl() {}
	virtual ~server_ctrl() {}

	std::size_t get_number_of_process();						// check the maximum number of CPUs

//---------------------- Member Variables ----------------------//
protected:

};

// end of code 