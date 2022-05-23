#include <Servo.h>

byte servoPin = 9;

Servo servo;
int value = 1000; // Set signal value, which should be between 1100 and 1900

void setup() 
{
servo.attach(servoPin);

servo.writeMicroseconds(0); // send "stop" signal to ESC.
Serial.begin(9600);

delay(5000); // delay to allow the ESC to recognize the stopped signal
// servo.writeMicroseconds(1000);
// delay(2000);
// servo.writeMicroseconds(1500);
// delay(2000);
// servo.writeMicroseconds(1000);
Serial.println("start");
}


void loop() 
{
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



/*else if (value = 3000) {
Serial.println(value);
servo.writeMicroseconds(1450);
delay(200);
servo.writeMicroseconds(1400);
delay(200);
servo.writeMicroseconds(1350);
delay(200);
servo.writeMicroseconds(1300);
delay(200);
servo.writeMicroseconds(1250);
delay(200);
servo.writeMicroseconds(1200);
delay(200);
servo.writeMicroseconds(1150);
delay(200);
servo.writeMicroseconds(1100);
delay(200);
servo.writeMicroseconds(1050);
delay(200);
servo.writeMicroseconds(1000);
}*/
}
}
