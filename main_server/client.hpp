#pragma once 

#include "../common/server_lib/session.hpp"

// const
//////////////////////////////////////////////////////////////////
const unsigned short CLIENT_TYPE_NONE 			= 0;			// no type

const unsigned short CLIENT_TYPE_DRONE 			= 1;			// agent on drone
const unsigned short CLIENT_TYPE_TRANSMITTER 	= 2;			// RC transmitter
const unsigned short CLIENT_TYPE_MANAGER 		= 3;			// manager (option)

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
