#ifndef Encoder_H_
#define Encoder_H_
#include "GPIO.h"

namespace exploringRPi {

/**
 * @class Encoder
 * @brief A class to read shaft position and direction form HKT22 rotary encoder. 
 * The class uses four GPIOs to track motor position.
 */
class Encoder {
	
public:
	enum DIRECTION(CLOCKWISE, COUNTERCLOCKWISE)


private:
	// GPIO pins CHA ChB 
		GPIO *gpio_chA, GPIO *gpio_chB;
		Direction direction;
		
public:
	Encoder(GPIO *);
	
	
	
	
	
	
}

}
