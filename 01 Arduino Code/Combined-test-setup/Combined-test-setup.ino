#include <Servo.h>

byte servoPin = 9;

Servo servo;
int value = 1000; // Set signal value, which should be between 1100 and 1900
//-------------------------------------- 
#define RT0 100000   // Ω
#define B 4267      // K
//-------------------------------------- 
#define VCC 5    //Supply voltage
#define R 100000  //R=100KΩ
float cal_temp(int A);
//Variables
float RT, VR, ln, TX, T0, VRT;

 
void setup() 
{
  servo.attach(servoPin);

  servo.writeMicroseconds(0); // send "stop" signal to ESC.

  Serial.begin(9600);
  T0 = 25 + 273.15;                 //Temperature T0 from datasheet, conversion from Celsius to kelvin
}
 
void loop() 
{ 
  Serial.print(cal_temp(A0));
  Serial.print(",");
  Serial.print(cal_temp(A1));
  Serial.print(",");
  Serial.println(cal_temp(A2));
  delay(1000); 
  if (Serial.available() > 0) 
  {
  // read the incoming byte:
  //value = Serial.read();
  value = Serial.parseInt();
  if ((value >= 1000) && (value <= 2000)) 
  {
  Serial.println(value);
  servo.writeMicroseconds(value); // Send value to ESC.
  }
  }
}

float cal_temp(int A)
{
  VRT = analogRead(A);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT
 
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
 
  TX = TX - 273.15;                 //Conversion to Celsius
  return TX;
}
