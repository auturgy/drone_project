#pragma once 

// include 
//////////////////////////////////////////////////////////////////
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>

// const
//////////////////////////////////////////////////////////////////
const unsigned short SESSION_NUM_LIMIT = 65535;
const unsigned short MAX_SESSION_COUNT = 100;					// shold be smaller than SESSION_NUM_LIMIT

const unsigned short MAX_RECEIVE_BUFFER_LEN = 512;				// how about MAX MTU size 1500? 

// session statement 
const unsigned short SS_CLOSE = 0;								// session is off 
const unsigned short SS_WAITING = 1;							// when waiting for accepting 
const unsigned short SS_OPEN = 2;								// session is on
const unsigned short SS_MAX = 3;								// maximum count 


// namespace logger
//////////////////////////////////////////////////////////////////
namespace logger {

	inline void info(const std::string& log) {
#ifdef _DEBUG_
		std::cout << "[INFO] " << log << std::endl;
#endif //end of _DEBUG_
	}

	inline void warning(const std::string& log) {
#ifdef _DEBUG_
		std::cout << "[WARNING] " << log << std::endl;
#endif //end of _DEBUG_		
	}

	inline void error(const std::string& log) {
#ifdef _DEBUG_
		std::cout << "[ERROR] " << log << std::endl;
#endif //end of _DEBUG_		
	}
} // end of logger 

// end of code 