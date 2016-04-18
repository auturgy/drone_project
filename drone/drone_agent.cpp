#include "drone_agent.hpp"
#include "gpio.hpp"

typedef singleton<logger> logger_singleton; 

// overriding function : init()
//////////////////////////////////////////////////////////////////
bool drone_agent::init() {
	
	// uncomment the below code when building on RPI
	gpio_set();
	
	return connection::init();
}

// gpio set 
//////////////////////////////////////////////////////////////////
void drone_agent::gpio_set() {

	/*
	** Before starting this daemon, you need to install servoblast
	** https://github.com/richardghirst/PiBits/tree/master/ServoBlaster
	** below gpio setting is default, so you can change if you wnat 
	*/
	gpio_2_r_pins_.push_back(std::make_unique<gpio>("0"));
	gpio_2_r_pins_.push_back(std::make_unique<gpio>("1"));
	gpio_2_r_pins_.push_back(std::make_unique<gpio>("2"));
	gpio_2_r_pins_.push_back(std::make_unique<gpio>("3"));
	gpio_2_r_pins_.push_back(std::make_unique<gpio>("4"));
	gpio_2_r_pins_.push_back(std::make_unique<gpio>("5"));

}

// send data to gpio
//////////////////////////////////////////////////////////////////
void drone_agent::send_to_gpio(RC_SIGNAL *rcs){

	// codes need to be optimized ***
	// because PWM is not changed frequently in most cases.

	gpio_2_r_pins_[0]->setval_gpio(rcs->pin_1_);
	gpio_2_r_pins_[1]->setval_gpio(rcs->pin_2_);
	gpio_2_r_pins_[2]->setval_gpio(rcs->pin_3_);
	gpio_2_r_pins_[3]->setval_gpio(rcs->pin_4_);
	gpio_2_r_pins_[4]->setval_gpio(rcs->pin_5_);
	gpio_2_r_pins_[5]->setval_gpio(rcs->pin_6_);

}

// overriding function : handle_connect
//////////////////////////////////////////////////////////////////
void drone_agent::handle_connect(const boost::system::error_code& error) {

	if(!error) {
		// do something here just after connection is established. 
	}

	connection::handle_connect(error);
}

// overriding function : process_packet
//////////////////////////////////////////////////////////////////
void drone_agent::process_packet(const char* data, const unsigned short size) {

	// do something here!

}

void drone_agent::handle_udp_receive( const boost::system::error_code& error, std::size_t bytes_transferred ) {

	RC_SIGNAL *rcs_p = reinterpret_cast<RC_SIGNAL *>(rcv_udp_buff_->get()->ptr_);

	if( sizeof(RC_SIGNAL) == bytes_transferred ) {

		std::cout << rcs_p->pin_1_ << "\t" << rcs_p->pin_2_ << "\t" << rcs_p->pin_3_ << "\t" << rcs_p->pin_4_ << "\t" << rcs_p->pin_5_ << "\t"  << rcs_p->pin_6_ << std::endl;		

		// uncomment the below code when building on RPI
		send_to_gpio(rcs_p);
	}

	// do something here!!!

	post_udp_recv();
}

#ifdef _DRONE_AGENT_

typedef singleton<drone_agent> agent_singleton; 
// main
//////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	std::string addr;

	if( argc > 1) {
		addr = argv[1]; 
	} else {
		addr = "127.0.0.1";
	}

	agent_singleton::get().init();
	agent_singleton::get().udp_on(addr, DEFAULT_PORT_NUMBER);
	agent_singleton::get().start();

	//agent_singleton::get().shutdown();

	return 0;
}

#endif /* _DRONE_AGENT_ */


// end of file 