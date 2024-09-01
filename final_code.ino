#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
SoftwareSerial BTSerial(6, 7);
SoftwareSerial rfid_reader(7, 1);

// IR Sensors
int IR_SENSOR1 = 2;  // for detecting car entry
int IR_SENSOR2 = 3;  // for detecting car exit

// Motor pins for X, Y, Z axis
int MOTOR_X = 12;
int MOTOR_Y = 11;
int MOTOR_Z = 10;

char tag[] = "270020F5E012";
char tag_1[] = "270020F68E7F";
char tag_2[] = "270020F4D023";
char input[12]; // Variable to store the ID of the Tag being presented
int count = 0;  // Counter variable
boolean flag = 0, flag1 = 0, flag2 = 0, state = 0;

void setup() {
  lcd.begin(16, 2);
  rfid_reader.begin(9600);
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  pinMode(13, OUTPUT);
  pinMode(IR_SENSOR1, INPUT);
  pinMode(IR_SENSOR2, INPUT);
  pinMode(MOTOR_X, OUTPUT);
  pinMode(MOTOR_Y, OUTPUT);
  pinMode(MOTOR_Z, OUTPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("CAR PARKING");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM");
  delay(3000);
  lcd.clear();
}

void loop() {
  int IR1 = digitalRead(IR_SENSOR1);
  int IR2 = digitalRead(IR_SENSOR2);

  if (IR1 == LOW) { 
    lcd.setCursor(0, 0);
    lcd.print("Plz insert card");
  
    if (rfid_reader.available()) {
      readRFID();
      
      if (flag == 1) {
        performParking(1); // Slot 1
      } else if (flag1 == 1) {
        performParking(2); // Slot 2
      } else if (flag2 == 1) {
        performParking(3); // Slot 3
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Wrong Tag");
        Serial.println("Wrong Tag");
      }
      clearInput();
    }
  }

  if (IR2 == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("VIST AGAIN");
    lcd.setCursor(0, 1);
    lcd.print("THANKS");
    runMotor(MOTOR_Z, 5000); // Return to start position
    lcd.clear();
  }
}

void readRFID() {
  count = 0;
  while (rfid_reader.available() && count < 12) {
    input[count] = rfid_reader.read();
    count++;
    delay(5);
  }
  if (count == 12) {
    checkTagMatch();
  }
}

void checkTagMatch() {
  flag = flag1 = flag2 = 1;
  for (int i = 0; i < 12; i++) {
    if (input[i] != tag[i]) flag = 0;
    if (input[i] != tag_1[i]) flag1 = 0;
    if (input[i] != tag_2[i]) flag2 = 0;
  }
}

void clearInput() {
  for (int i = 0; i < 12; i++) {
    input[i] = 'F';
  }
  count = 0;
}

void performParking(int slot) {
  switch (slot) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Matched1");
      Serial.println("Matched1!");
      BTSerial.println("270020F5E012");
      moveToSlot(1);
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Matched2");
      Serial.println("Matched2!");
      BTSerial.println("270020F68E7F");
      moveToSlot(2);
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Matched3");
      Serial.println("Matched3!");
      BTSerial.println("270020F4D023");
      moveToSlot(3);
      break;
  }
}

void moveToSlot(int slot) {
  switch (slot) {
    case 1:
      runMotor(MOTOR_X, 3000); // X-axis movement
      runMotor(MOTOR_Y, 2000); // Y-axis movement
      runMotor(MOTOR_Z, 1000); // Z-axis movement
      break;
    case 2:
      runMotor(MOTOR_X, 6000); // X-axis movement
      runMotor(MOTOR_Y, 4000); // Y-axis movement
      runMotor(MOTOR_Z, 2000); // Z-axis movement
      break;
    case 3:
      runMotor(MOTOR_X, 9000); // X-axis movement
      runMotor(MOTOR_Y, 6000); // Y-axis movement
      runMotor(MOTOR_Z, 3000); // Z-axis movement
      break;
  }
}

void runMotor(int motorPin, int duration) {
  digitalWrite(motorPin, HIGH);
  delay(duration);
  digitalWrite(motorPin, LOW);
}
