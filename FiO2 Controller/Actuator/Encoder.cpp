/*Encoder.cpp

*/

#include "Encoder.h"
include<iostream>
#include<unistd.h>
#include<cmath>
using namespace std;

namespace exploringRPi {
	
// Steppermotor.cpp has two constructors - not using GPIO class?
Encoder::Encoder(GPIO *chA, GPIO *chB, int position){
	
	this->gpio_chA = chA;
	this-<gpio_chB = chB;
	this->init(position);
}

// Calibration routine
void Encoder::init(int position){
	this->position = position
	
}

void Encoder::read(int A, int B){
	
	
}

// Get direction from Quadrature information
void Encoder::direction(int position){
	// read new position
	
}


Encoder::~Encoder(){}



}