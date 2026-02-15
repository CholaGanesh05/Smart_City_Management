#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define GREEN_LIGHT_PIN D6
#define GREEN_LIGHT_PIN2 D7
#define RED_LIGHT_PIN D5
#define RED_LIGHT_PIN2 D3
#define echo1 D1
#define echo2 D2
#define signal D8

const char* ssid = "David's Galaxy S20 FE 5G";
const char* password = "ownp4619";

ESP8266WebServer server(80);
String trafficStatus = "RED";

unsigned long previousMillis = 0;
const unsigned long greenDuration = 10000;
const unsigned long redDuration = 10000;
bool isGreen = false;

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LIGHT_PIN, OUTPUT);
  pinMode(RED_LIGHT_PIN, OUTPUT);
  pinMode(GREEN_LIGHT_PIN2, OUTPUT);
  pinMode(RED_LIGHT_PIN2, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(signal, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", trafficStatus);
  });

  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  long duration = pulseIn(echo1, HIGH);
  long distance = duration * 0.034 / 2;
  Serial.println(distance);
  long duration1 = pulseIn(echo2, HIGH);
  long distance1 = duration1 * 0.034 / 2;
  Serial.println(distance1);
  unsigned long currentMillis = millis();
  if(signal==HIGH){
    digitalWrite(GREEN_LIGHT_PIN, HIGH);
    digitalWrite(GREEN_LIGHT_PIN2, HIGH);
    digitalWrite(RED_LIGHT_PIN, LOW);
    digitalWrite(RED_LIGHT_PIN2, LOW);
  }
  else{
  if (distance < 50 && distance1 > 50) {
    digitalWrite(GREEN_LIGHT_PIN, HIGH);
    digitalWrite(GREEN_LIGHT_PIN2, HIGH);
    digitalWrite(RED_LIGHT_PIN, LOW);
    digitalWrite(RED_LIGHT_PIN2, LOW);
    trafficStatus = "GREEN";
  } else if (distance > 50 && distance1 < 50) {
    digitalWrite(GREEN_LIGHT_PIN, LOW);
    digitalWrite(GREEN_LIGHT_PIN2, LOW);
    digitalWrite(RED_LIGHT_PIN, HIGH);
    digitalWrite(RED_LIGHT_PIN2, HIGH);
    trafficStatus = "GREEN";
  } else {
    if (isGreen && currentMillis - previousMillis >= greenDuration) {
      digitalWrite(GREEN_LIGHT_PIN, LOW);
      digitalWrite(GREEN_LIGHT_PIN2, LOW);
      digitalWrite(RED_LIGHT_PIN, HIGH);
      digitalWrite(RED_LIGHT_PIN2, HIGH);
      trafficStatus = "RED";
      isGreen = false;
      previousMillis = currentMillis;
    } else if (!isGreen && currentMillis - previousMillis >= redDuration) {
      digitalWrite(GREEN_LIGHT_PIN, HIGH);
      digitalWrite(GREEN_LIGHT_PIN2, HIGH);
      digitalWrite(RED_LIGHT_PIN, LOW);
      digitalWrite(RED_LIGHT_PIN2, LOW);
      trafficStatus = "GREEN";
      isGreen = true;
      previousMillis = currentMillis;
    }
  }}
  if (distance > 50) {
    trafficStatus = "GREEN";
  }

  server.handleClient();
}
