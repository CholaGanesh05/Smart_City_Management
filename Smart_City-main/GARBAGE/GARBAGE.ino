#include <Servo.h> 
Servo myServo; 
const int trigPin = 8;
const int echoPin = 7;
const int ir = 13;

void setup() {
  myServo.attach(9);        
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);   
  Serial.begin(9600);        
  pinMode(ir,INPUT);
}

void loop() {
  int irread = digitalRead(ir);

  long duration, distance;

  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 

  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println(irread);
if(irread==1){
  if (distance > 0 && distance <= 10) { 
    Serial.println("Object detected! Moving servo.");
    myServo.write(180); 
    delay(1000);
  } else {
    Serial.println("No object detected.");
    myServo.write(0);
  }}
  else{
    myServo.write(0);
  }

delay(500);
}