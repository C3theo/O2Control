/* 
* Application to control buzzer and motor from PulseOx and Encoder readings.
* Adapted from Derek Molloy's book "Exploring Raspberry Pi" 
* and Samy Kamkar's combobreaker.

Using WiringPi
*
* The pins are wired as follows:
* Motor:
* SLP - RPi GPIO4 = 7
* MS1 - RPi GPIO17 = 11
* STEP- RPi GPIO27 = 13
* DIR - RPi GPIO22 = 15
* MS2 - RPi GPIO24 = 18
*
* Buzzer:
* driver - RPi GPIO =  
*
* Encoder:
* chA - RPi GPIO4 = 5
* chB - RPi GPIO3 = 3
*
* PulseOx: ttyUSB0
*
*/

#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include "StepperMotor.h"
#include "GPIO.h"

#define BUZZ 11 
#define ENC_A 4
#define ENC_B 3

#define ENC_STEPS 1200
#define DEBOUNCE_TIME 200
#define TOLERANCE .05

using namespace std;
using namespace O2Controller;


int maxPosition;
int minPosition = 0;
int encPosition = 0;

double set_spO2 = .93;
double set_FiO2 = .8;

double FiO2_Check = 0.6;
double spO2 = .95;
int count = 0;
bool fingerOut = true;

void soundBuzzer(){ 
	for(int i = 0; i<6; i++){
		digitalWrite(BUZZ, HIGH); 
		usleep(100);
		digitalWrite(BUZZ, LOW);
	}
}

void updatePosition(){
	 static unsigned long lastISRTime = 0, x = 1;
	 static unsigned long currentISRTIme = millis();
	 if (currentISRTime - lastISRTime > DEBOUNCE_TIME){

	if (digitalRead(ENC_A) == digitalRead(ENC_B)){
		 encPosition++;
		}else{encPosition--;}
	 }
	//cout << "Encoder Position: " << encPosition <<endl;
	lastISRTime = currentISRTime;
 }

 void calibrate(StepperMotor *motor, int *maxPosition){
	 motor.setDirection(ANTICLOCKWISE);
	 motor.threadedStepforDuration(146, 500); // check rpm
	 motor.sleep();
	 sleep(1);
	 encPosition = 0; // this will have to update in another thread??
	 motor.wake();
	 
	 // encPosition has to be updating during calibration
	 motor.setDirection(CLOCKWISE);
	 motor.threadedStepforDuration(146, 500); // check rpm
	 motor.sleep();
	 sleep(1);
	 maxPosition = encPosition;
	 motor.wake();
 }

void increaseFiO2(StepperMotor *motor){
	motor.wake();
	motor.setDirection(StepperMotor::CLOCKWISE);
	motor.rotate(270.0F);
	FiO2 += .1;
	motor.sleep();
}

void decreaseFiO2(StepperMotor *motor){
	motor.wake();
	motor.setDirection(StepperMotor::ANTICLOCKWISE);
	motor.rotate(270.0F);
	FiO2 -= .1;
	motor.sleep();
}

int main(){
   //Motor 
   StepperMotor m(17,24,27,4,22,60,200); //rpm = 60 steps = 200
   m.setStepMode(StepperMotor::STEP_FULL); // 
   m.setSpeed(60);  //rpm
 
   //Encoder
   	wiringPiSetupPhys(); 									// Pin Numbering
	pinMode(ENC_A, INPUT);
	pinMode(ENC_B, INPUT);
	pullUpDnControl(ENC_A, PUD_UP);							// turn on pullups
	pullUpDnControl(ENC_B, PUD_UP);
    wiringPiISR(ENC_A, INT_EDGE_RISING, &updatePosition); // Encoder Interrupt
	
   //Buzzer
   pinMode(BUZZ, OUTPUT);
   
	//PulseOx
	

   calibrate(&motor, &maxPosition);
   
   bool wean = false;
   bool FingerOut = false; // this comes from PulseOx
   
   enum SAFETY{UNDETECTABLE, ABOVE_SETPT, BELOW_SETPT, BELOW_SAFE};
   
   	switch(SAFETY){
		case ABOVE_SETPT:
		wean = true;
		break;
		case UNDETECTABLE :
		wean = false;
		break;
		case BELOW_SETPT:
		wean = false;
		break;
		case BELOW_SAFE:
		wean = false;
		break;
	}
   

   while(1){
	   
	if (FiO2 > FiO2_Check) {   
	// Change to multithreaded timer
		for(int i=0;i<=180; i++){
			// SpO2 above target for 30 minutes (1800 seconds)
			if((i==180)&&(spO2 > spo2_set)&&(spO2 - spo2_set > TOLERANCE)){ // 3rd Test
				wean = true;
				}
			// Safety System 
			// Automatically increase FiO2 if SpO2 undetectable, < 88%, or below setpoint
			else if(fingerOut == true;){ // 1st Test
				wean = false;
				fingerOut = false;
				spO2 = .85;
				break;}
			else if(spO2 < .88){ // 2nd Test
				wean = false;
				break;}
			else if((spO2 > spo2_set) && (spO2 - spo2_set > TOLERANCE)) {
				wean = false;
				break;}
			sleep(10); // Check every 10 seconds
		}
	
		if(wean == false){
			m.increaseFiO2();
			wean = NULL;
			i=0;
			} 
		else if (wean ==true){
			m.decreaseFiO2();
			wean = NULL
			i=0; // restart counter
	
	} else {
		// Check Patient before continuing 
		soundBuzzer();
		// Prompt Dr. Input
		FiO2_Check = 0.25;
		count++;
		if(count == 2){
			sprintf('Patient Weaned');
			return 0;	
		}
	// Restart Loop
	}  
   }
}
