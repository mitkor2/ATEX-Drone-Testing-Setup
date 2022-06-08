#include <SPI.h>
#include <time.h>
#include <Servo.h>
#include <stdio.h>
#include <TimeLib.h>

#include "Adafruit_MAX31855.h"

#define MAXCLK  6
#define MAXDO   5
#define MAXCS_S1   4
#define MAXCS_S2   3
#define MAXCS_S3   2

Adafruit_MAX31855 thermocouple1(MAXCLK, MAXCS_S1, MAXDO);
Adafruit_MAX31855 thermocouple2(MAXCLK, MAXCS_S2, MAXDO);
Adafruit_MAX31855 thermocouple3(MAXCLK, MAXCS_S3, MAXDO);

void begin_couple(Adafruit_MAX31855 thermocouple, String sensorname);
int read_couple(Adafruit_MAX31855 thermocouple);

byte servoPin = 9;
Servo servo;
int value = 1000; // Set signal value, which should be between 1100 and 1900
int value2 = 0;

void setup() 
{
  servo.attach(servoPin);
  servo.writeMicroseconds(0); // send "stop" signal to ESC.
  
  Serial.begin(9600);
  delay(5000);
  Serial.println("start");
  Serial.println("Fill in the max dulty cycle");

  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc
  
  begin_couple(thermocouple1, "thermocouple 1");
  begin_couple(thermocouple2, "thermocouple 2");
  begin_couple(thermocouple3, "thermocouple 3");

}

void loop() 
{
  time_t t = now();
  if (Serial.available() > 0) 
  {
  // read the incoming byte:

  value = Serial.parseInt();
  if ((value >= 1000) && (value <= 2000)) 
  {
  Serial.println(value);
  if (value == 2000)
  {
    Serial.println("Great, turn power suply on and after music fill in the min dulty cycle and you can start");
  }
  servo.writeMicroseconds(value); // Send value to ESC.
  value2 = value;
  }
  } 
  delay(1000); 
  Serial.print(minute(t));
  Serial.print(":");
  Serial.print(second(t));
  Serial.print(",");
  Serial.print(value2);
  Serial.print(",");
  Serial.print(thermocouple1.readCelsius());
  Serial.print(",");
  Serial.print(thermocouple2.readCelsius());
  Serial.print(",");
  Serial.println(thermocouple3.readCelsius()); 
}

void begin_couple(Adafruit_MAX31855 thermocouple, String sensorname)
{
  if (!thermocouple.begin()) 
  {
    Serial.println("ERROR Please check"+sensorname);
    while (1) delay(10);
  }
}
