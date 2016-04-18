#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "gpio.hpp"
#include <boost/lexical_cast.hpp>

using namespace std;

// construnction 
//////////////////////////////////////////////////////////////////
//gpio::gpio()
//	:gpio_num_(4)												//this->; //GPIO4 is default
//{}

// construction 
//////////////////////////////////////////////////////////////////
gpio::gpio(string gnum)
	:gpio_num_(gnum)
{}

// funtion setval_gpio for micro second value  
//////////////////////////////////////////////////////////////////
bool gpio::setval_gpio(unsigned short val)
{
    string setval_str = "/dev/servoblaster";
    string gpio_value = gpio_num_ + "=" + boost::lexical_cast<std::string>(val) + "us";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio

    if(!setvalgpio.is_open()){
        cout << " OPERATION FAILED: Unable to set the value of GPIO"<<  gpio_num_ <<" ."<< endl;
        return false;
    }

    setvalgpio << gpio_value ;      //write value to value file
    setvalgpio.close();             // close value file
    return true;
}

// 
//////////////////////////////////////////////////////////////////
string gpio::get_gpio_num(){

	return gpio_num_;

}

// end of file 