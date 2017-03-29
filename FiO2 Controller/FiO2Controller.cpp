/* 
* Application to control buzzer, encoder, and motor.
* Adapted from Derek Molloy's book "Exploring Raspberry Pi" 
* and Samy Kamkar's combobreaker.

* The pins are wired as follows:
Motor:
* SLP - RPi GPIO4 = 7
* MS1 - RPi GPIO17 = 11
* STEP- RPi GPIO27 = 13
* DIR - RPi GPIO22 = 15
* MS2 - RPi GPIO24 = 18

Buzzer:

Encoder:



Import python code for pulse ox using Boostpython
or rewrite 
*/

#include <iostream>
#include "StepperMotor.h"
#include "GPIO.h"
#include "Wiring.h"

using namespace std;
using namespace exploringBB;

void soundBuzzer(GPIO buzzer){ // Not correct - pass object into function?
	buzz_GPIO.setValue(HIGH); // if alarm set high for 5 seconds 
	// usleep(5000);
	buzz_GPIO.setValue(LOW);
  
}

int main(){
  
   //Setup Motor GPIOs, RPM=60 and 200 steps per revolution
   StepperMotor m(17,24,27,4,22,60,200);
   m.setDirection(StepperMotor::CLOCKWISE); // set default to increase after calibration
   m.setStepMode(StepperMotor::STEP_FULL); // test half
   m.setSpeed(100);  //rpm
   
   //Encoder
   
   //PulseOx
   
   //Buzzer
   
   GPIO buzz_GPIO(17);    // pin 11 (Buzzer output)
   buzz_GPIO.setDirection(OUTPUT);    // setup buzz_GPIO as output

	
	m.increaseFiO2();
	// if alarm 
	m.decreaseFiO2();
	

   m.sleep();   // cut power to the stepper motor
}
