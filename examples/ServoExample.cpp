/*
 * MotorExample.cpp
 *
 *	Example of PWM used to control motors via an esc
 *
 *  Created on: May 29, 2013
 *      Author: Saad Ahmad ( http://www.saadahmad.ca )
 */

#include <string>
#include "../lib/PWM.h"
#include "../lib/Motor.h"
#include <ctime>
#include <cstdlib>

const float MIN_SPEED = 0;
const float MAX_SPEED = 100;
const int MIN_SERVO_PULSE_TIME = 750;
const int MAX_SERVO_PULSE_TIME = 2250;
const std::string PIN_SERVO_RIGHT("P8_13");
const std::string PIN_SERVO_LEFT("P9_14");

MotorControl motorControls[] = {
	MotorControl(PIN_SERVO_LEFT, MIN_SPEED, MIN_SPEED, MAX_SPEED),
	MotorControl(PIN_SERVO_RIGHT, MIN_SPEED, MIN_SPEED, MAX_SPEED)
};


#include <signal.h>

// Stop all the motors when we interrupt the program so they don't keep going
void sig_handler(int signum) {
	for (unsigned int i = 0; i < 2; i++) {
		MotorControl & motor = motorControls[i];
		motor.SetOutputValue(0);
		motor.UpdatePWMSignal();
	}

	exit(signum);
}

int main() {
	signal(SIGINT, sig_handler);
	signal(SIGSEGV, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGHUP, sig_handler);
	signal(SIGABRT, sig_handler);

	for (int i = 0; i < 2; i++) {
		motorControls[i].Enable();
	}

	while (true) {
		// Increase by 10% each time and view the output PPM signal
		for (int i = 0; i < 100; i += 5) {
			std::clock_t startTime = std::clock();
			while (((clock() - startTime) * 1000.0 / CLOCKS_PER_SEC) < 500) {
				for (int iServo = 0; iServo < 2; iServo++) {
					MotorControl & motor = motorControls[iServo];
					// Offset the motors a bit
					motor.SetOutputValue(i);
					motor.UpdatePWMSignal();
				}
			}
		}
	}
}

