#define BLYNK_TEMPLATE_ID "TMPL3jaPXoxLy"
#define BLYNK_TEMPLATE_NAME "Smart Car Controller"
#define BLYNK_AUTH_TOKEN "Ns5z6zhJkvEhP_EpXdPAAWxUJybi4KGz"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>

const char* ssid = "David's Galaxy S20 FE 5G";
const char* password = "ownp4619";

#define IR_LEFT D6
#define IR_RIGHT D5
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define TRIG_PIN D7
#define ECHO_PIN 3
#define EN_A D0
#define EN_B D8

String trafficStatus = "RED";
long distanceThreshold = 20;
int speedPWM = 150;
bool startProgram = false;

BLYNK_WRITE(V0) {
  startProgram = param.asInt();
  if (startProgram) {
    Serial.println("Program Started!");
  } else {
    stopMotors();
    Serial.println("Program Stopped!");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);
  stopMotors();
  setSpeed(speedPWM);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();
  if (startProgram) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, "http://192.168.186.174/");
      int httpCode = http.GET();
      if (httpCode == 200) {
        trafficStatus = http.getString();
        Serial.println("Traffic Status: " + trafficStatus);
      } else {
        Serial.println("Failed to get traffic status.");
      }
      http.end();
    }
    long distance = getDistance();
    Serial.println("Distance: " + String(distance) + " cm");
    if (trafficStatus == "GREEN" && distance > distanceThreshold) {
      followLine();
    } else if (distance <= distanceThreshold) {
      stopMotors();
      Serial.println("Obstacle detected!");
    } else {
      stopMotors();
    }
    delay(1000);
  }
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void followLine() {
  int leftSensor = digitalRead(IR_LEFT);
  int rightSensor = digitalRead(IR_RIGHT);
  if (leftSensor == LOW && rightSensor == LOW) {
    stopMotors();
  } else if (leftSensor == LOW && rightSensor == HIGH) {
    turnRight();
  } else if (leftSensor == HIGH && rightSensor == LOW) {
    turnLeft();
  } else {
    moveForward();
  }
}

void moveForward() {
  setSpeed(150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  setSpeed(250);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  setSpeed(250);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  setSpeed(150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setSpeed(int pwmValue) {
  analogWrite(EN_A, pwmValue);
  analogWrite(EN_B, pwmValue);
}
