CC = g++
CFLAGS =

all: Gary MotorExample BasicPWMExample
	@echo Executables located in bin

Gary: Gary.o PWM.o
	$(CC) Gary.o PWM.o -o ../bin/Gary

MotorExample: MotorExample.o PWM.o
	$(CC) MotorExample.o PWM.o -o ../bin/MotorExample

BasicPWMExample: BasicPWMExample.o PWM.o
	$(CC) $(CFLAGS) BasicPWMExample.o PWM.o -o ../bin/BasicPWMExample

Gary.o: ../examples/Gary.cpp
	$(CC) $(CFLAGS) -c ../examples/Gary.cpp

MotorExample.o: ../examples/MotorExample.cpp
	$(CC) $(CFLAGS) -c ../examples/MotorExample.cpp

BasicPWMExample.o: ../examples/BasicPWMExample.cpp
	$(CC) $(CFLAGS) -c ../examples/BasicPWMExample.cpp

PWM.o: ../lib/PWM.cpp
	$(CC) -c ../lib/PWM.cpp

clean:
	$(RM) *o *~ ../bin/*

