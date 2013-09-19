CC = g++
CFLAGS =

all: MotorExample BasicPWMExample
	@echo Executables located in bin

MotorExample: MotorExample.o PWM.o
	$(CC) MotorExample.o PWM.o -o ../bin/MotorExample

BasicPWMExample: BasicPWMExample.o PWM.o
	$(CC) $(CFLAGS) BasicPWMExample.o PWM.o -o ../bin/BasicPWMExample

MotorExample.o: ../examples/MotorExample.cpp
	$(CC) $(CFLAGS) -c ../examples/MotorExample.cpp

BasicPWMExample.o: ../examples/BasicPWMExample.cpp
	$(CC) $(CFLAGS) -c ../examples/BasicPWMExample.cpp

PWM.o: ../lib/PWM.cpp
	$(CC) -c ../lib/PWM.cpp

clean:
	$(RM) *o *~ ../bin/*

