#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
bool tooslow = 1;
byte servoPin = 9;

Servo servo;
int duty_new = 1000; // Set signal value, which should be between 1100 and 1900
int duty_old = 0;

void setup() {
  servo.attach(servoPin);
  servo.writeMicroseconds(0); // send "stop" signal to ESC.
  Serial.begin(9600);
  delay(5000);
  Serial.println("start");
  Serial.println("Fill in the max dulty cycle");
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12); //Prescaler 256
  TIMSK1 |= (1 << TOIE1); //enable timer overflow
  pinMode(2, INPUT);
  attachInterrupt(0, RPM, FALLING);
}

ISR(TIMER1_OVF_vect) {
  tooslow = 1;
}

void loop() {
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
  if (tooslow == 1) 
  {
    Serial.println("Too Slow");
  }
  else 
  {
    rpmfloat = 120 / (rpmtime/ 31250.00);
    rpm = round(rpmfloat);
    Serial.print("RPM = ");
    Serial.println(rpm);
  }
}

void RPM () {
  rpmtime = TCNT1;
  TCNT1 = 0;
  tooslow = 0;
}

int setDuty(int duty_new, int duty_old){
  int a; 
  int b = 0;
  duty_new = duty_new - (duty_new % 50); // from 1055 to 1050 it change the value to a correct varuable.
  
  if(duty_new == duty_old){
    servo.writeMicroseconds(duty_new); // Send value to ESC.
    Serial.println("ESC is ready to use, have fun");
    Serial.println(duty_new);
  }
  if(duty_old > duty_new){
    for(a = duty_old; a > (duty_new - 50); a = a - 50){
      servo.writeMicroseconds(a); // Send value to ESC.
      Serial.println(a);
      delay(1000);
      b = Serial.parseInt();
      if(b != 0){
       stopESC(a);
       duty_new = 1000;
       break;
       }
      }
    }
    else if(duty_old < duty_new){
      for(a = duty_old; a < (duty_new + 50); a = a + 50){
        servo.writeMicroseconds(a); // Send value to ESC.
        Serial.println(a);
        delay(1000);
        b = Serial.parseInt();
        if(b != 0){
          stopESC(a);
          duty_new = 1000;
          break;
        }
      }
    }
    return duty_new;
}

void stopESC(int duty_old){
  int a;
  Serial.println("Emergency stop");
  for(a = duty_old; a > (1000 - 50); a = a - 50){
    servo.writeMicroseconds(a); // Send value to ESC.
    Serial.println(a);
    delay(1000);
  }
}
