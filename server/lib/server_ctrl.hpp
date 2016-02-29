#pragma once 

#include "utils.hpp"

// Server Controller Class
//////////////////////////////////////////////////////////////////
class server_ctrl : private boost::noncopyable{

//---------------------- Member Functions ----------------------//
private:
	server_ctrl() {}											// constructor (do nothing)

public:
	virtual ~server_ctrl() {}									// destructor (do nothing)

	
	static server_ctrl& get();									// singleton method

	std::size_t get_number_of_process();						// check the maximum number of CPUs

//---------------------- Member Variables ----------------------//
protected:

private:
	static std::unique_ptr<server_ctrl> instance_;

};

// end of code 