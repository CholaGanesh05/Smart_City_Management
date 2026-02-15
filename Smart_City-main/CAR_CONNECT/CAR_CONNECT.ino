#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "David's Galaxy S20 FE 5G";       // Mobile hotspot SSID
const char* password = "ownp4619";   // Mobile hotspot password

// Pin configuration
#define IR_LEFT D5
#define IR_RIGHT D6
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

String trafficStatus = "RED"; // Default status

void setup() {
  Serial.begin(115200);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Query traffic light status
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, "http://192.168.232.174/"); // Replace <TRAFFIC_LIGHT_IP> with NodeMCU 1's IP
    int httpCode = http.GET();

    if (httpCode == 200) {
      trafficStatus = http.getString();
      Serial.println("Traffic Status: " + trafficStatus);
    } else {
      Serial.println("Failed to get traffic status.");
    }
    http.end();
  }

  // Control car based on traffic light status
  if (trafficStatus == "GREEN") {
    followLine();
  } else {
    stopMotors();
  }

  delay(1000); // Polling interval
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
