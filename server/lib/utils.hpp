#pragma once 

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

#include <iostream>

//#define DEBUG

// const
//////////////////////////////////////////////////////////////////
unsigned short MAX_SESSION_NUM = 65535;

// logger
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