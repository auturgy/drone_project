#pragma once

#include "utils.hpp"

class tcp_listener;

// Session Class
//////////////////////////////////////////////////////////////////
class session /*: public boost::enalbed_shread_from_this<session>*/ {

//---------------------- Member Functions ----------------------//
public:
	explicit session( boost::shared_ptr<boost::asio::ip::tcp::socket> socket)
		: socket_(socket)
		, session_id_(MAX_SESSION_NUM)
	{}

	/*
	explicit session (
		boost::asio::io_service& ios,
		const boost::asio::ip::tcp::endpoint& endpoint
		)
			:socket_(boost::shared_ptr<boost::asio::ip::tcp::socket>(
				boost::ref(ios)))
	{
		socket_->connect(endpoint);
	}
	*/

	virtual ~session() {}

	// return the socket which this connection gets
	boost::asio::ip::tcp::socket& socket() {
		return *socket_.get();
	}

	// return session ID
	unsigned short sesson_id() {

		if(session_id_ == MAX_SESSION_NUM) {
			logger::error("session is not open!!!!");
		}
		
		return session_id_;
	}

	template<class Functor>
	void async_read(
		const boost::asio::mutable_buffers_1& buf,
		const Functor& f,
		std::size_t at_least_bytes
		) const 
	{
		//boost::asio::async_read(*socket_, buf, boost::asio::trasnfer_at_least(at_least_bytes), f);
	}

	template<class Functor>
	void async_write (
		const boost::asio::const_buffers_1& buf,
		const Functor& f
		) const
	{
		//boost::asio::async_write(*socket_, buf, f);
	}

	void shutdown() const {
		socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_->close();
	}

//---------------------- Member Variables ----------------------//	
protected:
	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
	unsigned short session_id_;									// max_num: 65,535
	//tcp_listener tcp_listener_;								//

};