#include <iostream>
#include <unistd.h>
#include "motor/StepperMotor.h"

#include <wiringPi.h>
#define ENC_A 4
#define ENC_B 3

#define ENC_STEPS 1200
#define DEBOUNCE_TIME 200
#define TOLERANCE .05

using namespace std;
using namespace exploringRPi;

int maxPosition;
int minPosition = 0;
int encPosition = 0;

double set_spO2 = .93;
double set_FiO2 = .8;

double FiO2_Check = 0.6;
double spO2 = .95;
int count = 0;
bool fingerOut = true;

int main(){
	
   //Using 5 GPIOs, RPM=60 and 200 steps per revolution
   //MS1=17, MS2=24, STEP=27, SLP=4, DIR=22
   StepperMotor m(17,24,27,4,22,60,200);
   m.setDirection(StepperMotor::COUNTERCLOCKWISE);
   m.setStepMode(StepperMotor::STEP_FULL);
   m.setSpeed(100);  //rpm
 //Encoder
   	wiringPiSetupPhys(); 									// Pin Numbering
	pinMode(ENC_A, INPUT);
	pinMode(ENC_B, INPUT);
	pullUpDnControl(ENC_A, PUD_UP);							// turn on pullups
	pullUpDnControl(ENC_B, PUD_UP);
    wiringPiISR(ENC_A, INT_EDGE_RISING, &updatePosition); // Encoder Interrupt
	
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
			//m.increaseFiO2();
			wean = NULL;
			i=0;
			} 
		else if (wean ==true){
			//m.decreaseFiO2();
			wean = NULL
			i=0; // restart counter
	
	} else {
		// Check Patient before continuing 
		soundBuzzer();
		// Prompt Dr. Input
		FiO2_Check = 0.25;
		count++;
		if(count == 2){
			// Patient Weaned
			return 0;	
		}
	// Restart Loop
	}  
   }
}
