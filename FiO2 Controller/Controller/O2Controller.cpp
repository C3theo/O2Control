/* O2Controller.cpp
	Extends StepperMotor
	Inherits from PulseOx
	
Too big of an object better separate??
	


*/

#include 'StepperMotor.h'
#include 'GPIO.h'

class O2Controller::protected StepperMotor, public Encoder public CMS50D, public Buzzer
{

private:
	int spO2;
	int FiO2;

public:
	void increaseO2(){
		
	}
	void decreaseO2(){
		
	}
	
	
	
	
	
	
	virtual void ~O2Controller();
};