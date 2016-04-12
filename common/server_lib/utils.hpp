#pragma once 

// include 
//////////////////////////////////////////////////////////////////
#include "protocol.hpp"
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <iostream>

// const
//////////////////////////////////////////////////////////////////
const unsigned short SESSION_NUM_LIMIT = 65535;					// maximum of unsigned short
const unsigned short MAX_SESSION_COUNT = 500;					// shold be smaller than SESSION_NUM_LIMIT						// default port number
const unsigned short MAX_BUFF_NUM_ON_EACH_SESSION = 6;			// (recv + send) * 3 = 6
const unsigned short PACKET_POOL_COUNT = MAX_SESSION_COUNT*2;	// maximum packet number in PACKET POOL

const unsigned short DEFAULT_PORT_NUMBER = 31400;				// default port number

// session statement 
const unsigned short SS_CLOSE 	= 0;							// session is off 
const unsigned short SS_WAITING = 0x1 << 1;						// when waiting for accepting 
const unsigned short SS_OPEN 	= 0x1 << 2;						// session is on
const unsigned short SS_UDP 	= 0x1 << 3;						// session is on
const unsigned short SS_MAX 	= 0x1 << 15;					// maximum count 


// packet unit struct for managing packet manager 
//////////////////////////////////////////////////////////////////
struct PKT_UNIT
{
	char ptr_[MAX_PACKET_SIZE];
	unsigned short id_;

	PKT_UNIT(unsigned short id){
		id_ = id;
	}
};


// logger class
//////////////////////////////////////////////////////////////////
class Logger
{
//---------------------- Member Functions ----------------------//	
private:
	Logger(std::ostream &o = std::cout):m_file(o){};

	static inline Logger& get(){
		Logger *p = instance_.get();
	
		if(!p) {
			instance_.reset(new Logger);
			p = instance_.get();
		}
		return *p;
	}

public:    

	static Logger& info(){
		Logger *p =  &get();
		//boost::unique_lock<boost::mutex> lock(p->mtx_);
		
#ifdef _DEBUG_		
		*p << "[INFO] ";
#endif /*_DEBUG_*/

		return *p;
	}

	static Logger& warning(){
		Logger *p =  &get();
		//boost::unique_lock<boost::mutex> lock(p->mtx_);
	
#ifdef _DEBUG_				
		*p << "[WARNING] ";
#endif /*_DEBUG_*/

		return *p;
	}

	static Logger& error(){
		Logger *p =  &get();
		//boost::unique_lock<boost::mutex> lock(p->mtx_);
		
#ifdef _DEBUG_		
		*p << "[ERROR] ";
#endif /*_DEBUG_*/		

		return *p;
	}

    template <typename T>
    Logger &operator << (const T &a) {

#ifdef _DEBUG_		    	
        m_file<<a;
#endif /*_DEBUG_*/

        return *this;
    }

    Logger &operator<<(std::ostream& (*pf) (std::ostream&)){

#ifdef _DEBUG_		
        m_file<<pf;
#endif /*_DEBUG_*/

        return *this;
    }

//---------------------- Member Functions ----------------------//	
public:
	//boost::mutex mtx_;
private:
    std::ostream &m_file;
    static std::unique_ptr<Logger> instance_;					// logger itself
}; // end of logger class 


// end of code 