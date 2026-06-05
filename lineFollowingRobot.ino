#include <Servo.h>

int counter = 0;  // Add this to declare the counter variable

int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int sensor5 = A4;

int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 7;
int enA = 6;
int enB = 5; 

int ultraSonicTrig = 8;
int ultraSonicEcho = 11;

Servo myservo;  // create Servo object to control a servo

int pos = 0;    // variable to store the servo position


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  myservo.write(0);
  myservo.attach(12);
  

  // road sensor

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);

  // motors

  pinMode(enA, OUTPUT); 
  pinMode(enB, OUTPUT);

  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

  digitalWrite(enA, HIGH); 
  digitalWrite(enB, HIGH); 

  // ultrasonic

  pinMode(ultraSonicTrig, OUTPUT);
  pinMode(ultraSonicEcho, INPUT);
  
  delay(5000);

}

void loop() {
  // put your main code here, to run repeatedly


  int s1 = digitalRead(sensor1); // сенсоры на дорогу
  int s2 = digitalRead(sensor2);
  int s3 = digitalRead(sensor3);
  int s4 = digitalRead(sensor4);
  int s5 = digitalRead(sensor5);

  long t, dist;

  digitalWrite(ultraSonicTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSonicTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSonicTrig, LOW);

  t = pulseIn(ultraSonicEcho, HIGH);
  dist = microsecondsToCentimeters(t);

  Serial.print("dist ");
  Serial.print(dist);

  Serial.print("counter ");
  Serial.print(counter);

  //Serial.print("pos ");
  //Serial.print(pos);

  Serial.print("\n");


  if ((dist < 15) && (dist != 0) && (counter < 1 )){
    forward();
    counter += 1;
  }

  //straight
  
  if ((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 1) && (s5 == 1) && (counter >= 1)) {
    forward();
    delay(10);
   }

//all zero
  if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0) && (counter != 2)){
    forward();
    counter += 1;
    delay(50);
  }

  if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0) && (counter == 7)){
    stop();
  }

  if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0) && (counter == 2)){
    payload();
    counter += 1;
  }

  // turn right
   
  if (((s2 == 0)||(s1 == 0)) && (s5 == 1) && (counter >= 1)) {
    turnRight();    
  }
  
  if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s5 == 1) && (counter >= 1)) {
    turnRight();
   
  }
  
  // turn left
  
  if (((s4 == 0)||(s5 == 0)) && (s1 == 1) && (counter >= 1)) {
    turnLeft();
    
  }

  if ((s1 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0) && (counter >= 1)) {
    turnLeft();
   
  }


  if (counter > 7){
    stop();
  }

  // Uncomment for debugging
  
  /*Serial.print("Sensor 1: ");
  Serial.println(s1);
  Serial.print("Sensor 2: ");
  Serial.println(s2);
  Serial.print("Sensor 3: ");
  Serial.println(s3);
  Serial.print("Sensor 4: ");
  Serial.println(s4);
  Serial.print("Sensor 5: ");
  Serial.println(s5);*/
}


void forward() {
  analogWrite(enA, 100);
  analogWrite(enB, 100);

  digitalWrite(in1, LOW); // Right Motor forward
  digitalWrite(in2, HIGH);  // Right Motor backward
  digitalWrite(in3, HIGH);  // Left Motor backward 
  digitalWrite(in4, LOW);   // Left Motor forward
}

void turnRight() {
  analogWrite(enA, 180);
  analogWrite(enB, 180);

  digitalWrite(in1, LOW);  // Right Motor forward
  digitalWrite(in2, HIGH); // Right Motor backward
  digitalWrite(in3, LOW);  // Left Motor backward
  digitalWrite(in4, HIGH); // Left Motor forward
}

void turnLeft() {
  analogWrite(enA, 180);
  analogWrite(enB, 180);

  digitalWrite(in1, HIGH); // Right Motor forward
  digitalWrite(in2, LOW);  // Right Motor backward
  digitalWrite(in3, HIGH); // Left Motor backward
  digitalWrite(in4, LOW);  // Left Motor forward
}

void stop() {
  digitalWrite(in1, LOW); // Right Motor forward
  digitalWrite(in2, LOW); // Right Motor backward
  digitalWrite(in3, LOW); // Left Motor backward
  digitalWrite(in4, LOW); // Left Motor forward
}

void payload() {

  digitalWrite(in1, LOW); // Right Motor forward
  digitalWrite(in2, LOW); // Right Motor backward
  digitalWrite(in3, LOW); // Left Motor backward
  digitalWrite(in4, LOW); // Left Motor forward

  for (pos = 0; pos <= 180; pos += 3) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  delay(50);
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // took half of the distance travelled.
  return microseconds / 29 / 2;
}


