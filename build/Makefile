CC = g++
CFLAGS =

all: Gary ServoExample MotorExample BasicPWMExample
	@echo Executables located in bin

Gary: Gary.o PWM.o
	$(CC) Gary.o PWM.o -o ../bin/Gary
	
ServoExample: ServoExample.o PWM.o
	$(CC) ServoExample.o PWM.o -o ../bin/ServoExample

MotorExample: MotorExample.o PWM.o
	$(CC) MotorExample.o PWM.o -o ../bin/MotorExample

BasicPWMExample: BasicPWMExample.o PWM.o
	$(CC) $(CFLAGS) BasicPWMExample.o PWM.o -o ../bin/BasicPWMExample

Gary.o: ../examples/Gary.cpp
	$(CC) $(CFLAGS) -c ../examples/Gary.cpp
	
ServoExample.o: ../examples/ServoExample.cpp
	$(CC) $(CFLAGS) -c ../examples/ServoExample.cpp

MotorExample.o: ../examples/MotorExample.cpp
	$(CC) $(CFLAGS) -c ../examples/MotorExample.cpp

BasicPWMExample.o: ../examples/BasicPWMExample.cpp
	$(CC) $(CFLAGS) -c ../examples/BasicPWMExample.cpp

PWM.o: ../libraries/PWM.cpp
	$(CC) $(CFLAGS) -c ../libraries/PWM.cpp

clean:
	$(RM) *o *~ ../bin/*

