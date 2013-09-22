/*
 * Gary.cpp
 *
 *	Created by on September 21, 2013 by:
 * 		Mike Lewis (Alphalem) (http://www.alphalem.com)
 */

#include <string>
#include "../include/PWM.h"
#include "../include/Motor.h"
#include <ctime>
#include <cstdlib>
#include <signal.h>

const float MIN_SPEED = 0;
const float CTR_SPEED = 90;
const float MAX_SPEED = 180;
const int MIN_SERVO_PULSE_TIME = 750;
const int CTR_SERVO_PULSE_TIME = 1475;
const int MAX_SERVO_PULSE_TIME = 2250;
const std::string PIN_SERVO_LEFT("P9_14");
const std::string PIN_SERVO_RIGHT("P8_13");

ServoControl servoControls[] = {
	ServoControl(PIN_SERVO_LEFT, CTR_SPEED, MIN_SPEED, MAX_SPEED),
	ServoControl(PIN_SERVO_RIGHT, CTR_SPEED, MIN_SPEED, MAX_SPEED)
};

// Stop all the servos when we interrupt the program
void sig_handler(int signum)
{
	for (int i = 0; i < 2; i++) {
		ServoControl& servo = servoControls[i];
		servo.SetAngle(90);
		servo.UpdatePWMSignal();
	}

	exit(signum);
}

// State machine for robot's motion
void move(int state)
{
	ServoControl& servoLeft = servoControls[0];
	ServoControl& servoRight = servoControls[1];
	switch (state) {
		case 1: // forward
			servoLeft.SetAngle(0);
			servoRight.SetAngle(180);
			break;
		case 2: // reverse
			servoLeft.SetAngle(180);
			servoRight.SetAngle(0);
			break;
		case 3: // left
			servoLeft.SetAngle(180);
			servoRight.SetAngle(180);
			break;
		case 4: // right
			servoLeft.SetAngle(0);
			servoRight.SetAngle(0);
			break;
		default: // halt
			servoLeft.SetAngle(90);
			servoRight.SetAngle(90);
	}
	servoLeft.UpdatePWMSignal();
	servoRight.UpdatePWMSignal();
}
			

int main()
{
	signal(SIGINT, sig_handler);
	signal(SIGSEGV, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGHUP, sig_handler);
	signal(SIGABRT, sig_handler);

	for (int i = 0; i < 2; i++) {
		servoControls[i].Enable();
	}

	/*while (true) {
		// Increase by 10% each time and view the output PPM signal
		for (int i = 0; i < 100; i += 10) {
			std::clock_t startTime = std::clock();
			while (((clock() - startTime) * 1000.0 / CLOCKS_PER_SEC) < 500) {
				for (int j = 0; j < 2; j++) {
					ServoControl& servo = servoControls[j];
					// Offset the servos a bit
					servo.SetAngle(i);
					servo.UpdatePWMSignal();
				}
			}
		}
	}*/
}

