const int trigPin1 = 9;
const int echoPin1 = 10;
const int trigPin2 = 3;
const int echoPin2 = 6;
const int led1 = 8;
const int led2 = 4;
const int led3 = 2;

unsigned long sensor2StartTime = 0;
bool sensor2Triggered = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  Serial.println("Ultrasonic sensor system is ready...");
}

void loop() {
  handleUltrasonicSensors();
}

void handleUltrasonicSensors() {
  float distance1 = measureDistance(trigPin1, echoPin1);
  float distance2 = measureDistance(trigPin2, echoPin2);

  if (distance1 < 100) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }

  if (distance2 < 100) {
    digitalWrite(led2, HIGH);
  } else {
    digitalWrite(led2, LOW);
  }

  if (distance2 < 40) {
    if (!sensor2Triggered) {
      sensor2StartTime = millis();
      sensor2Triggered = true;
    } else if (millis() - sensor2StartTime >= 25000) {
      digitalWrite(led3, HIGH);
    }
  } else {
    sensor2Triggered = false;
    digitalWrite(led3, LOW);
  }

  Serial.print("Sensor 1 Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Sensor 2 Distance: ");
  Serial.print(distance2);
  Serial.println(" cm");
}

float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0343) / 2;
}
