#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "gpio.hpp"

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

// 
//////////////////////////////////////////////////////////////////
bool gpio::export_gpio()
{
    string export_str = "/sys/class/gpio/export";

    // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    ofstream exportgpio(export_str.c_str()); 
 
    if(!exportgpio.is_open()){
        cout << " OPERATION FAILED: Unable to export GPIO"<< gpio_num_ <<" ."<< endl;
        return false;
    }

    exportgpio << gpio_num_ ; 									//write GPIO number to export
    exportgpio.close(); 										//close export file
    return true;
}

// 
//////////////////////////////////////////////////////////////////
bool gpio::unexport_gpio()
{
    string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    if(!unexportgpio.is_open()){
        cout << " OPERATION FAILED: Unable to unexport GPIO"<< gpio_num_ <<" ."<< endl;
        return false;
    }

    unexportgpio << gpio_num_ ; //write GPIO number to unexport
    unexportgpio.close(); //close unexport file
    return true;
}

// 
//////////////////////////////////////////////////////////////////
bool gpio::setdir_gpio(string dir)
{

    string setdir_str ="/sys/class/gpio/gpio" + gpio_num_ + "/direction";
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
        if(!setdirgpio.is_open()){
            cout << " OPERATION FAILED: Unable to set direction of GPIO" << gpio_num_ <<" ."<< endl;
            return false;
        }

        setdirgpio << dir; //write direction to direction file
        setdirgpio.close(); // close direction file
        return true;
}

// 
//////////////////////////////////////////////////////////////////
bool gpio::setval_gpio(string val)
{

    string setval_str = "/sys/class/gpio/gpio" + gpio_num_ + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
        if(!setvalgpio.is_open()){
            cout << " OPERATION FAILED: Unable to set the value of GPIO"<<  gpio_num_ <<" ."<< endl;
            return false;
        }

        setvalgpio << val ;//write value to value file
        setvalgpio.close();// close value file
        return true;
}

// 
//////////////////////////////////////////////////////////////////
bool gpio::getval_gpio(string& val){

    string getval_str = "/sys/class/gpio/gpio" + gpio_num_ + "/value";
    ifstream getvalgpio(getval_str.c_str());// open value file for gpio
    if (!getvalgpio.is_open()){
        cout << " OPERATION FAILED: Unable to get value of GPIO"<< gpio_num_ <<" ."<< endl;
        return true;
            }

    getvalgpio >> val ;  //read gpio value

    if(val != "0")
        val = "1";
    else
        val = "0";

    getvalgpio.close(); //close the value file
    return true;
}

// 
//////////////////////////////////////////////////////////////////
string gpio::get_gpio_num(){

	return gpio_num_;

}

// end of file 