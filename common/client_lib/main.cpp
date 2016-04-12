#ifdef _CLIENT_TEST_
//#include <cstdlib>
//#include <cstring>

#include "connection.hpp"
typedef singleton<connection> conn_singleton;

// posix_input class 
//////////////////////////////////////////////////////////////////
class posix_input {
public:
	posix_input(boost::asio::io_service& io_service)
		:input_(io_service, ::dup(STDIN_FILENO))
		,input_buffer_(MAX_RECEIVE_BUFFER_LEN)
	{}
	~posix_input() {}

	void init()
	{
		std::cout << "====================================================================================" << std::endl;
		std::cout << "   You can put your message through TCP, UDP, or both after successful connection" 	<< std::endl;
		std::cout << "====================================================================================" << std::endl;
		post_posix_input();
	}

	 void close() { 
	 	input_.close();
	 }

protected:
	void post_posix_input()
	{
		// Read a line of input entered by the user.
		boost::asio::async_read_until(
			input_,
			input_buffer_, 
			'\n',
			boost::bind(
				&posix_input::handle_read_input, 
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	void handle_read_input(const boost::system::error_code& error, std::size_t length)
	{
		if (!error)
		{
			std::string line;
            std::istream is(&input_buffer_);
            std::getline(is, line);

			if(conn_singleton::get().get_connection_stat() == SS_OPEN) {
				boost::shared_ptr<PKT_UNIT>& packet = conn_singleton::get().alloc_packet();
				PKT_REQ_TEST* packet_p = (PKT_REQ_TEST*)packet.get();				
				packet_p->init();
				std::memcpy(packet_p->buffer_, line.c_str(), length);
				packet_p->size_ += length;

				conn_singleton::get().post_send(packet, packet_p->size_);
			}

			conn_singleton::get().post_udp_send(line.c_str(), length);

			post_posix_input();
			
		} else {
			close();
		}
	}

protected:
	boost::asio::posix::stream_descriptor input_;
	boost::asio::streambuf input_buffer_;
};


// upd connection test 
//////////////////////////////////////////////////////////////////
int udp_test(std::string& addr) {

	conn_singleton::get().udp_on(addr, DEFAULT_PORT_NUMBER);

	return 0;
}

// tcp connection test 
//////////////////////////////////////////////////////////////////
int tcp_test(std::string& addr) {

	auto endpoint = boost::asio::ip::tcp::endpoint( 
						boost::asio::ip::address::from_string(addr), 
						DEFAULT_PORT_NUMBER);

	conn_singleton::get().connect(endpoint);

	return 0;
}

// main
//////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	std::string addr = "127.0.0.1";
	posix_input posix_input(conn_singleton::get().ios_);

	if( argc > 1)
		addr = argv[1]; 
	
	posix_input.init();
	conn_singleton::get().init();

	tcp_test(addr);
	if(udp_test(addr)){	
		return -1;		
	}

	conn_singleton::get().start();					// wait until ctrl-c
	//conn_singleton::get().shutdown();				// it doesn't need 
	
	return 0;
}

#endif /* _CLIENT_TEST_ */

