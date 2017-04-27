#include <AFMotor.h>
#include <SoftwareSerial.h>
 
AF_DCMotor motor(3, MOTOR12_1KHZ); // create motor #1, 64KHz pwm
AF_DCMotor motor2(4, MOTOR12_1KHZ); // create motor #2, 64KHz pwm

//pins go 14 - 19
//a5 -> 19
const int trigPin = 19;
//a4 -> 18
const int echoPin = 18;
int initialSpeed = 200;
#define ledPin 15
int state = 0;
SoftwareSerial mySerial(16,17);//rx,tx

void setup() {
  // initialize serial communication, useful for printing/logging
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  mySerial.begin(9600);

  motor.setSpeed(initialSpeed);     // set the speed to 200/255
  motor2.setSpeed(initialSpeed);
}

void loop()
{

  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
  }
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
//  mySerial.print(cm);
//  mySerial.println("cm");
//  mySerial.println();
  
  if(cm < 15){
    digitalWrite(ledPin, HIGH);
    motor.run(RELEASE);      // stopped
    motor2.run(RELEASE);
  } else{
    digitalWrite(ledPin, LOW);
      motor.run(FORWARD);      // turn it on going forward
      motor2.run(FORWARD);
//  delay(250);
  }
    
  
  delay(100);
}

/*
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
*/

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}