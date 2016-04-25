#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "gpio.hpp"
#include <boost/lexical_cast.hpp>

using namespace std;

// construction
//////////////////////////////////////////////////////////////////
gpio::gpio(string gnum)
        :gpio_num_(gnum)
        ,pre_val(0)
{}

// destruction 
//////////////////////////////////////////////////////////////////
gpio::~gpio()
{

}

// Before starting this daemon, you need to install servoblaster
//
// funtion setval_gpio for micro second value
//////////////////////////////////////////////////////////////////
bool gpio::setval_gpio(unsigned short val)
{
    //if( pre_val != val ) {

        string setval_str = "/dev/servoblaster";
        //string gpio_value = gpio_num_;

        // below code can be removed if calibration is done with the value less than 100 
        if( val < 100 ) val = 100; // minimum is 100

        string gpio_value = gpio_num_ + "=" + boost::lexical_cast<std::string>(val);
        ofstream setvalgpio(setval_str.c_str()); // open value file for gpio

        if(!setvalgpio.is_open()){
            cout << " OPERATION FAILED: Unable to set the value of GPIO"<<  gpio_num_ <<" ."<< endl;
            return false;
        }

        setvalgpio << gpio_value << std::endl ;      //write value to value file
        setvalgpio.close();                            // close value file
    //}

    pre_val = val;

    return true;
}

//
//////////////////////////////////////////////////////////////////
string gpio::get_gpio_num(){

        return gpio_num_;

}

/* When using pi-blaster~ not tested well

// construction 
//////////////////////////////////////////////////////////////////
gpio::gpio(string gnum)
	:gpio_num_(gnum)
    ,pre_val(0)
{
    setval_gpio("1");
}

// destruction 
//////////////////////////////////////////////////////////////////
gpio::~gpio()
{
    setval_gpio("0");
}

// function setval_gpio(string val)
//////////////////////////////////////////////////////////////////
bool gpio::setval_gpio(string val) {

    string gpio_value = "\"" + gpio_num_ + "=" + val + "\"";
    
    // open value file for gpio
    ofstream pi_blaster("/dev/pi-blaster"); 

    if(!pi_blaster.is_open()){
        cout << " OPERATION FAILED: Unable to set the value of GPIO"<<  gpio_num_ <<" ."<< endl;
        return false;
    }

    pi_blaster << gpio_value << std::endl ;         //make sure that adds "end line"
    pi_blaster.close(); 

    return true;
}


// Before starting this daemon, you need to install pi-blaster
// in https://github.com/sarfata/pi-blaster
// (GPIO number - BCM xx in the diagram in the site)
// below gpio setting is default, so you can change if you wnat 

// funtion setval_gpio for micro second value  
//////////////////////////////////////////////////////////////////
bool gpio::setval_gpio(unsigned short val)
{
    string gpio_value = "=0.";

    if (val != 0) {

        if(val < 100) gpio_value += "0";

        // for example, "17=0.11" > /dev/pi-blaster
        // means set 1100us on GPIO pin 17. 
        gpio_value += boost::lexical_cast<std::string>(val);
    } else {
        return false;
    }

    return setval_gpio(gpio_value);
}

// 
//////////////////////////////////////////////////////////////////
string gpio::get_gpio_num(){

	return gpio_num_;

}
*/

// end of file 