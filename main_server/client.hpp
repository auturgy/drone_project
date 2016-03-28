#pragma once 

#include "../common/server_lib/session.hpp"

// const
//////////////////////////////////////////////////////////////////
const unsigned short CLIENT_TYPE_DRONE = 0;						// agent on drone
const unsigned short CLIENT_TYPE_TRANSMITTER = 1;				// RC transmitter
const unsigned short CLIENT_TYPE_MANAGER = 2;					// manager (option)
const unsigned short CLIENT_TYPE_MAX = 3;						// type maximum

// Server Controller Class
//////////////////////////////////////////////////////////////////
class client : session {
//---------------------- Member Functions ----------------------//
protected:
	void process_packet(const char* data, const unsigned short size);

//---------------------- Member Variables ----------------------//	
protected:

	boost::atomic<unsigned short> client_type_;
};
