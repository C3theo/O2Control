#!/bin/bash
g++ FiO2Controller.cpp Actuator/GPIO.cpp Actuator/StepperMotor.cpp -o O2Test -pthread