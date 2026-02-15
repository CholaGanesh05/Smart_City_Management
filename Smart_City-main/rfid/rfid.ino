#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 7
#define OUTPUT_PIN A0

MFRC522 rfid(SS_PIN, RST_PIN);

byte authorizedUID[] = {0x4A, 0xA0, 0xB5, 0x15};
const int UID_LENGTH = 4;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);
  Serial.println("Place your RFID card near the reader...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (isAuthorizedCard()) {
    Serial.println("Authorized Card Detected!");
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(10000);
    digitalWrite(OUTPUT_PIN, LOW);
  } else {
    Serial.println("Unauthorized Card!");
  }

  rfid.PICC_HaltA();
}

bool isAuthorizedCard() {
  if (rfid.uid.size != UID_LENGTH) {
    return false;
  }

  for (byte i = 0; i < UID_LENGTH; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      return false;
    }
  }
  return true;
}
