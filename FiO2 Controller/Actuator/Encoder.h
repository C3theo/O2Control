/*
Encoder.h


calibrate 

read

http://www.phidgets.com/documentation/Phidgets/3531_0_Datasheet.pdf

*/

#ifndef Encoder_H_
#define Encoder_H_
#include "GPIO.h"
#include "StepperMotor.h"

namespace O2Controller {

/**
 * @class Encoder
 * @brief A class to calculate absolute position, direction form HKT22 rotary encoder.
 Range limited by stops on KNOB
 TODO: Add range in steps and degrees
 * The class uses four GPIOs to track motor position.
 
 */
class Encoder{
	
private:
	
	GPIO *gpio_chA, GPIO *gpio_chB; // GPIO inclusion
	int minPosition = 0;
	int maxPosition;
	int calibrate(StepperMotor motor){this->maxPosition};
	
public:
	Encoder(*gpio_chA, GPIO *gpio_chB);
	
	static unsigned long lastISRTime  = 0, x = 1;
	static unsigned long currentISRTIme;
	
	// Declare virtual if objects might provide different functionality
	void init();
	void update();
	

	
private:
//Encoder thread while motor running
	bool threadRunning;
	pthread_t thread;
	CallbackType callbackFunction;
}

}

#endif 