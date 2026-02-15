// Pin configuration
#define IR_LEFT D5  // Left IR sensor pin
#define IR_RIGHT D6 // Right IR sensor pin

#define IN1 D1  // Motor 1 input 1
#define IN2 D2  // Motor 1 input 2
#define IN3 D3  // Motor 2 input 1
#define IN4 D4  // Motor 2 input 2

void setup() {
  // Configure pins as input or output
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start with motors stopped
  stopMotors();

  Serial.begin(9600);
  Serial.println("Line-following robot initialized.");
}

void loop() {
  // Read IR sensor values
  int leftSensor = digitalRead(IR_LEFT);
  int rightSensor = digitalRead(IR_RIGHT);

  // Debugging: print sensor values
  Serial.print("Left: ");
  Serial.print(leftSensor);
  Serial.print(" | Right: ");
  Serial.println(rightSensor);

  // Decision-making logic
  if (leftSensor == LOW && rightSensor == LOW) {
    // Both sensors on black line -> move forward
    stopMotors();
  } else if (leftSensor == LOW && rightSensor == HIGH) {
    // Left sensor on black line, right sensor on white -> turn left
    turnRight();
  } else if (leftSensor == HIGH && rightSensor == LOW) {
    // Right sensor on black line, left sensor on white -> turn right
    turnLeft();
  } else {
    // Both sensors on white -> stop (no line detected)
    
    moveForwardd();
  }

  delay(100); // Small delay for stability
}


void moveForwardd() {
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
