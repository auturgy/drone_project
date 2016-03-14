#pragma once 

// include 
//////////////////////////////////////////////////////////////////
#include "protocol.hpp"
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <iostream>

// const
//////////////////////////////////////////////////////////////////
const unsigned short SESSION_NUM_LIMIT = 65535;					// maximum of unsigned short
const unsigned short MAX_SESSION_COUNT = 100;					// shold be smaller than SESSION_NUM_LIMIT						// default port number
const unsigned short MAX_BUFF_NUM_ON_EACH_SESSION = 6;			// (recv + send) * 3 = 6
const unsigned short PACKET_POOL_COUNT = MAX_SESSION_COUNT*2;	// maximum packet number in PACKET POOL

const unsigned short DEFAULT_PORT_NUMBER = 31400;				// default port number

// session statement 
const unsigned short SS_CLOSE = 0;								// session is off 
const unsigned short SS_WAITING = 1;							// when waiting for accepting 
const unsigned short SS_OPEN = 2;								// session is on
const unsigned short SS_MAX = 3;								// maximum count 

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
		
#ifdef _DEBUG_		
		*p << "[INFO] ";
#endif /*_DEBUG_*/

		return *p;
	}

	static Logger& warning(){
		Logger *p =  &get();

#ifdef _DEBUG_				
		*p << "[WARNING] ";
#endif /*_DEBUG_*/

		return *p;
	}

	static Logger& error(){
		Logger *p =  &get();

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
private:
    std::ostream &m_file;
    static std::unique_ptr<Logger> instance_;					// logger itself

}; // end of logger class 


// ringbuffer - referred to boost example
// http://www.boost.org/doc/libs/1_60_0/doc/html/atomic/usage_examples.html
//////////////////////////////////////////////////////////////////
template<typename T, size_t Size>
class ringbuffer {
//---------------------- Member Functions ----------------------//		
public:
	ringbuffer() 
		: head_(0)
		, tail_(0) 
	{}
	
	bool push(const T & value)
	{
		const size_t head = head_.load(boost::memory_order_relaxed);
		
#ifdef _BOOST_EXAMPLE_		
		size_t next_head = next(head);
		if (next_head == tail_.load(boost::memory_order_acquire)){
		  	return false;
		}
#else /* _BOOST_EXAMPLE_ */		
		size_t next_head;
		do {
			next_head = next(head);
			Logger::info() <<"push: head(" << head << ") |next_head(" << next_head << ") |tail_(" <<tail_<<")"<<std::endl;
		} while(next_head == tail_.load(boost::memory_order_acquire));
#endif /* _BOOST_EXAMPLE_ */

		ring_[head] = value;
		head_.store(next_head, boost::memory_order_release);
		return true;
	}

	bool pop(T & value)
	{
		//size_t tail = tail_.load(boost::memory_order_relaxed);
		//if (tail == head_.load(boost::memory_order_acquire))
		//	return false;
		size_t tail;
		do {
			tail = tail_.load(boost::memory_order_relaxed);
			Logger::info() <<"pop: tail(" << tail << ") |head_(" << head_ << ")" << std::endl;
		} while(tail == head_.load(boost::memory_order_acquire));

		value = ring_[tail];
		tail_.store(next(tail), boost::memory_order_release);
		return true;
	}

	T& pop()
	{
		//size_t tail = tail_.load(boost::memory_order_relaxed);
		//if (tail == head_.load(boost::memory_order_acquire))
		//	assert(0);
		size_t tail;
		do {
			tail = tail_.load(boost::memory_order_relaxed);
			Logger::info() <<"pop: tail(" << tail << ") |head_(" << head_ << ")" << std::endl;
			
		} while(tail == head_.load(boost::memory_order_acquire));
		
		size_t curr = tail;
		tail_.store(next(curr), boost::memory_order_release);
		return ring_[curr];
	}

private:
	const size_t next(size_t const current)
	{
		return (current + 1) % Size;
	}

//---------------------- Member Variables ----------------------//	
	T ring_[Size];											// ADD 1 not to make it deadlock in any case. 
	boost::atomic<size_t> head_, tail_;
};







// end of code 