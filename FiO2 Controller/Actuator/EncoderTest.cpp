/* Encoder Test using wiringPi

Encoder Quadrature 
ISR

Improve calibration routine

*/

#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include "StepperMotor.h"
using namespace std;

#define ENC_A 4
#define ENC_B 3
#define ENC_STEPS 1200
#define DEBOUNCE_TIME 200

int encPos = 0;

// ISR to indicate shaft movement - debounced
void updatePosition(){
	 static unsigned long lastISRTime = 0, x = 1;
	 static unsigned long currentISRTIme = millis();
	 if (currentISRTime - lastISRTime > DEBOUNCE_TIME){

	if (digitalRead(ENC_A) == digitalRead(ENC_B)){
		 encPos++;
		}else{encPos--;}
	 }
	cout << "Encoder Position: " << encPos <<endl;
	lastISRTime = currentISRTime;
 }

 void calibrate(StepperMotor *motor){
//	  spin CCW for 2 seconds - shaft catches on knob
//	  spin 146 steps ~ 270deg
	 motor.setDirection(ANTICLOCKWISE);
	 motor.threadedStepforDuration(146, 500); // check rpm
	 unsigned long minposition = 0;
	 motor.setDirection(CLOCKWISE);
	 motor.threadedStepforDuration(146, 500); // check rpm
	 int steps = updatePosition(minposition);
	 
 }

 int main() {
	wiringPiSetupGpio(); 									// GPIO numbering
	pinMode(ENC_A, INPUT);
	pinMode(ENC_B, INPUT);
	pullUpDnControl(ENC_A, PUD_UP);							// turn on pullups
	pullUpDnControl(ENC_B, PUD_UP);
//	calibrate(motor);
//	while(1){
//		wiringPiISR(ENC_A, INT_EDGE_RISING, &updatePosition);
//	}


	increaseFiO2(&motor);
	return 0;
 }
