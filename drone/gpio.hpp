#pragma once 

#include <string>


// RC Singal Transmitter Class 
//////////////////////////////////////////////////////////////////
class gpio 
{
//---------------------- Member Functions ----------------------//
public:
    //gpio_handler();  											// create a GPIO object that controls GPIO4 (default
    gpio(std::string x); 										// create a GPIO object that controls GPIOx, where x is passed to this constructor
    ~gpio();

    //bool setdir_gpio(std::string dir); 							// Set GPIO Direction
    //bool setval_gpio(std::string val); 							// Set GPIO Value (putput pins)
    bool setval_gpio(unsigned short val);
    //bool getval_gpio(std::string& val); 						// Get GPIO Value (input/ output pins)
    
    std::string get_gpio_num(); 								// return the GPIO number associated with the instance of an object
protected:
    inline bool setval_gpio(std::string val);                            // Set GPIO Value (putput pins)

//---------------------- Member Variables ----------------------//
protected:
    std::string gpio_num_; 										// GPIO number associated with the instance of an object
    unsigned short pre_val;
};


// end of file 
