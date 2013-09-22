CC = g++
CFLAGS =

default: Gary
	@echo Executable located in bin

all: Gary ServoExample MotorExample BasicPWMExample
	@echo Executables located in bin
	
examples: ServoExample MotorExample BasicPWMExample
	@echo Executables located in bin

Gary: Gary.o PWM.o
	$(CC) Gary.o PWM.o -o ../bin/Gary
	
ServoExample: ServoExample.o PWM.o
	$(CC) ServoExample.o PWM.o -o ../bin/ServoExample

MotorExample: MotorExample.o PWM.o
	$(CC) MotorExample.o PWM.o -o ../bin/MotorExample

BasicPWMExample: BasicPWMExample.o PWM.o
	$(CC) $(CFLAGS) BasicPWMExample.o PWM.o -o ../bin/BasicPWMExample

Gary.o: ../src/Gary.cpp
	$(CC) $(CFLAGS) -c ../src/Gary.cpp
	
ServoExample.o: ../src/examples/ServoExample.cpp
	$(CC) $(CFLAGS) -c ../src/examples/ServoExample.cpp

MotorExample.o: ../src/examples/MotorExample.cpp
	$(CC) $(CFLAGS) -c ../src/examples/MotorExample.cpp

BasicPWMExample.o: ../src/examples/BasicPWMExample.cpp
	$(CC) $(CFLAGS) -c ../src/examples/BasicPWMExample.cpp

PWM.o: ../src/PWM.cpp
	$(CC) $(CFLAGS) -c ../src/PWM.cpp

clean:
	$(RM) *o *~ ../bin/*

