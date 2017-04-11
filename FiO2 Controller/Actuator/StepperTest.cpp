#include <iostream>
#include <unistd.h>
#include "StepperMotor.h"
using namespace std;
using namespace O2Controller;

int main(){
   cout << "Starting RPi Stepper Motor Example:" << endl;
   //Using 5 GPIOs, RPM=60 and 200 steps per revolution
   //MS1=17, MS2=24, STEP=27, SLP=4, DIR=22
   StepperMotor m(17,24,27,4,22,60,200);
   m.setDirection(StepperMotor::ANTICLOCKWISE);
   m.setStepMode(StepperMotor::STEP_FULL);
   m.setSpeed(100);  //rpm
   cout << "Rotating 10 times at 100 rpm anti-clockwise, full step..." << endl;
   m.rotate(3600.0f);   //in degrees

   m.sleep();   // cut power to the stepper motor
   cout << "End of Stepper Motor Example" << endl;
}
