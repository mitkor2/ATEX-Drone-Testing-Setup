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
int setDuty(int duty_new, int duty_old);
void stopESC(int duty_old);

byte servoPin = 9;
Servo servo;
int duty_new = 1000; // Set signal value, which should be between 1100 and 1900
int duty_old = 0;

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
    duty_new = Serial.parseInt();
    if ((duty_new >= 1000) && (duty_new < 2000)) 
    {
      Serial.println(duty_new);
      duty_old = setDuty(duty_new, duty_old); // Send value to ESC 
    }
    else if (duty_new == 2000)
    {
      Serial.println("Great, turn power suply on and after music fill in the min duty cycle and you can start");
      duty_old = 1000;
      servo.writeMicroseconds(duty_new); // Send value to ESC.
    }
  }
  delay(1000); 
  Serial.print(minute(t));
  Serial.print(":");
  Serial.print(second(t));
  Serial.print(",");
  Serial.print(duty_old);
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

int setDuty(int duty_new, int duty_old)
{
  int a; 
  int b = 0;
  duty_new = duty_new - (duty_new % 50); // from 1055 to 1050 it change the value to a correct varuable.
  
  if(duty_new == duty_old)
  {
    servo.writeMicroseconds(duty_new); // Send value to ESC.
    Serial.println("ESC is ready to use, have fun");
    Serial.println(duty_new);
  }
  if(duty_old > duty_new)
  {
    for(a = duty_old; a > (duty_new - 50); a = a - 50)
    {
      servo.writeMicroseconds(a); // Send value to ESC.
      Serial.println(a);
      delay(1000);
      b = Serial.parseInt();
      if(b != 0)
      {
       stopESC(a);
       duty_new = 1000;
       break;
       }
      }
    }
    else if(duty_old < duty_new)
    {
      for(a = duty_old; a < (duty_new + 50); a = a + 50){
        servo.writeMicroseconds(a); // Send value to ESC.
        Serial.println(a);
        delay(1000);
        b = Serial.parseInt();
        if(b != 0)
        {
          stopESC(a);
          duty_new = 1000;
          break;
        }
      }
    }
    return duty_new;
}

void stopESC(int duty_old)
{
  int a;
  Serial.println("Emergency stop");
  for(a = duty_old; a > (1000 - 50); a = a - 50)
  {
    servo.writeMicroseconds(a); // Send value to ESC.
    Serial.println(a);
    delay(1000);
  }
}
