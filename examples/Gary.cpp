/*
 * Gary.cpp
 *
 *	Created by on September 21, 2013 by:
 * 		Mike Lewis (Alphalem) (http://www.alphalem.com)
 *
 *  Based on ServoExample.cpp by:
 *      Saad Ahmad (http://www.saadahmad.ca)
 */

#include <string>
#include "../lib/PWM.h"
#include "../lib/Servo.h"
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
	ServoControl(PIN_SERVO_LEFT, MIN_SPEED, MIN_SPEED, MAX_SPEED,
			MIN_SERVO_PULSE_TIME, MAX_SERVO_PULSE_TIME),
	ServoControl(PIN_SERVO_RIGHT, MIN_SPEED, MIN_SPEED, MAX_SPEED,
			MIN_SERVO_PULSE_TIME, MAX_SERVO_PULSE_TIME)
};

// Stop all the servos when we interrupt the program
void sig_handler(int signum)
{
	for (int i = 1; i <= 2; i++) {
		ServoControl & servo = servoControls[i];
		servo.SetOutputValue(0);
		servo.UpdatePWMSignal();
	}

	exit(signum);
}

int main()
{
	signal(SIGINT, sig_handler);
	signal(SIGSEGV, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGHUP, sig_handler);
	signal(SIGABRT, sig_handler);

	for (int i = 1; i <= 2; i++) {
		servoControls[i].Enable();
	}

	while (true) {
		// Increase by 10% each time and view the output PPM signal
		for (int i = 0; i < 100; i += 10) {
			std::clock_t startTime = std::clock();
			while (((clock() - startTime) * 1000.0 / CLOCKS_PER_SEC) < 500) {
				for (int j = 1; j <= 2; j++) {
					ServoControl & servo = servoControls[j];
					// Offset the servos a bit
					servo.Angle((i + j * 20) % 100);
					servo.UpdatePWMSignal();
				}
			}
		}
	}
}

