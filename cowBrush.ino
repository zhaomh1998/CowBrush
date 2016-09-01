#include <Wire.h>
#include "Adafruit_VCNL4010.h"

Adafruit_VCNL4010 vcnl;       //Setup proximity sensor
unsigned long initPosit = 0;  //The variable that stores the initial position of the cow
const int stage1Lim = 5000;   //Distance for stage 1
const int stage2Lim = 8000;   //Distance for stage 2

const int stage1Speed = 180;  //Speed for stage 1
const int stage2Speed = 255;  //Speed for stage 2 (Motor speed ranges from 0-255)

const int motorPin = 3;   //Output pin for motor
unsigned long read = 0;   //Variable that stores the read value of the proximity sensor (always positive so use unsigned long)
long distance = 0;        //Variable that stores the calculated distance between initial value (could be negative if pushed back)


void setup() {
	pinMode(motorPin, OUTPUT);                 //Set the pin of the motor to output
	Serial.begin(9600);
	//Proximity sensor testing
	Serial.println("VCNL4010 test");
	if (!vcnl.begin()) {
		Serial.println("Sensor not found :(");
		while (1);
	}
	Serial.println("Found VCNL4010");
	initPosit = vcnl.readProximity();         //Get the value of the proximity sensor as the initial value (position)
}


void loop() {
	read = vcnl.readProximity();              //Read current value of the sensor (current location)
	distance = initPosit - read;              //Calculate how far the current location is away from the initial position and stores the value in variable "distance"
	delay(100);                               //Delay after reading the sensor value
	if (distance < stage1Lim) {               //If distance is smaller than the limit distance for stage1, stop motor
		analogWrite(motorPin, 0);             //Write 0 (volts) to the motor
		Serial.println("Motor stage 0");      //Print in serial monitor
	}
	else if (distance >= stage1Lim && distance < stage2Lim) {   //If distance between stage 1 limit and stage 2 limit
		analogWrite(motorPin, stage1Speed);                     //Turn on the motor to speed set for stage 1
		Serial.println("Motor stage 1");
	}
	else if (distance >= stage2Lim) {             //If distance is larger than stage 2 limit
		analogWrite(motorPin, stage2Speed);       //Turn on the motor to speed set for stage 2
		Serial.println("Motor stage 2");
	}
}
