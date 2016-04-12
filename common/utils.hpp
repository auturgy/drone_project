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
const unsigned short SS_WAITING = 1;							// when waiting for accepting 
const unsigned short SS_OPEN 	= 2;							// session is on
const unsigned short SS_MAX 	= 3;							// maximum count 


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
class logger
{
//---------------------- Member Functions ----------------------//	
public:    
	logger(std::ostream &o = std::cout):m_file(o){};

	logger& info(){
		
#ifdef _DEBUG_		
		m_file << "[INFO] ";
#endif /*_DEBUG_*/

		return *this;
	}

	logger& warning(){
	
#ifdef _DEBUG_				
		m_file << "[WARNING] ";
#endif /*_DEBUG_*/

		return *this;
	}

	logger& error(){
		
#ifdef _DEBUG_		
		m_file << "[ERROR] ";
#endif /*_DEBUG_*/		

		return *this;
	}

    template <typename T>
    logger &operator << (const T &a) {

#ifdef _DEBUG_		    	
        m_file << a;
#endif /*_DEBUG_*/

        return *this;
    }

    logger &operator<<(std::ostream& (*pf) (std::ostream&)){

#ifdef _DEBUG_		
        m_file << pf;
#endif /*_DEBUG_*/

        return *this;
    }

//---------------------- Member Functions ----------------------//	
protected:
    std::ostream &m_file;
}; // end of logger class 


// Singleton class 
/////////////////////////////////////////////////////////////////
template <typename T>
class singleton {
protected:
	singleton() {}
	~singleton() {}

public: 
	static T& get() {
		T* p = instance_.get();

		if(!p) {
			instance_.reset(new T);
			p = instance_.get();
		}

		return *p; 
	}

private:
	static std::unique_ptr<T> instance_;				// class instance
};

template <typename T> 
std::unique_ptr<T> singleton<T>::instance_ = nullptr;

// end of code 