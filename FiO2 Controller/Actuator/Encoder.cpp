/* Encoder.cpp

Using wiringPi

*/

#include "Encoder.h"
#include "StepperMotor.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <iostream>
\

using namespace std;

namespace O2Controller {

// Using GPIO Class
Encoder::Encoder(GPIO *gpio_chA, GPIO *gpio_chB, int stepsPerRevolution;){
	this->gpio_chA = gpio_chA; // 3
	this->gpio_chB = gpio_chB; // 4
	
	// default min/max position of knob
	this->stepsPerRevolution = stepsPerRevolution;
	minPos = 0;
	this->maxPos = calibrate(StepperMotor *motor);	
}


void Encoder::init(){
	wiringPiSetupGpio(); 										// GPIO numbering
	pinMode(gpio_chA, INPUT);
	pinMode(gpio_chB, INPUT);
	pullUpDnControl(gpio_chA, PUD_UP);							// turn on pullups
	pullUpDnControl(gpio_chB, PUD_UP);
	wiringPiISR(this->gpio_chA, INT_EDGE_RISING, &update);  	// setinterrupt
	
	this->gpio_chA->setEdgeType(GPIO::EDGE);
	this->gpio_chB->setEdgeType(GPIO::EDGE);
	
	this->gpio_chA->waitForEdge(CallbackType callback); // ISR
	this->gpio_chA->waitForEdge(CallbackType callback);
	
	
	
}


void update(){
	static unsigned long lastISRTime = 0, x = 1;
	static unsigned long currentISRTIme = millis();
	if (currentISRTime - lastISRTime > DEBOUNCE_TIME){
	if (digitalRead(gpio_chA) == digitalRead(gpio_chB)){
		 this->pos++;
		 //Set Direction
		}else{this->pos--;
			setDirection
		}
	 }
	cout << "Encoder Position: " << this->pos <<endl;
	lastISRTime = currentISRTime;
 }
 
 int Encoder::calibrate(StepperMotor *motor){
	

	 
	this->motor->setDirection(ANTICLOCKWISE);
	this->motor->rotate(2700.0f);
	this->motor->sleep();
	
	usleep(1000);
	this->pos = 0; // min
	
	this->motor->setDirection(CLOCKWISE);
	this->motor->rotate(2700.0f);
	this->motor->sleep();
	
	// ???
	this->maxPos = this->pos;
	
	
 }
Encoder::~Encoder(){}

}