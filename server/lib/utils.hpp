#pragma once 

#include <boost/thread/thread.hpp>
#include <iostream>

// Server Controller Class
//////////////////////////////////////////////////////////////////
template<typename T>
class singleton : private boost::noncopyable {

protected: 
	singleton() {}

public:
	~singleton() {}
	static T& get(){

		T *p = singleton_.get();
		
		if(!p) {
			singleton_.reset(new T);
			p = singleton_.get();
		}

		return *p;
	}

private:
	static std::auto_ptr<T> singleton_;
};

template<typename T> 
std::auto_ptr<T> singleton<T>::singleton_;


// end of code 