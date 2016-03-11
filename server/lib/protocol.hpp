#pragma once 

// packet header structure
//////////////////////////////////////////////////////////////////
struct PACKET_HEADER
{
	unsigned short id_;
	unsigned short size_;										// packet size including packet header size 
};

// consts
//////////////////////////////////////////////////////////////////
const unsigned short MAX_RECEIVE_BUFFER_LEN = 512;				// how about MAX MTU size 1500? 
const unsigned short PACKET_HEADER_SIZE = sizeof(PACKET_HEADER);
const unsigned short MAX_PACKET_SIZE = PACKET_HEADER_SIZE + MAX_RECEIVE_BUFFER_LEN;

// protocols 
//
// for test, id = 0, it runs as echo server 
//
// from 1 to 65535, you can define your own protocol. 
//////////////////////////////////////////////////////////////////
const unsigned short REQ_TEST	= 0;							// test only 

	////////////////////////////////////
	//                                // 
	// define your own protocols here //
	//                                //
	////////////////////////////////////

const unsigned short REQ_MAX = 65535;
//////////////////////////////////////////////////////////////////


// REQ_TEST - pakcet  
///////////`///////////////////////////////////////////////////////
struct PKT_REQ_TEST : public PACKET_HEADER
{
	void Init()
	{
		id_ = REQ_TEST;
		size_ = sizeof(PKT_REQ_TEST);							// header + size of data
		//memset( buffer_, 0, MAX_RECEIVE_BUFFER_LEN );
	}

	char buffer_ [MAX_RECEIVE_BUFFER_LEN];
};




// end of file 