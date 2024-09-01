#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// LCD setup
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// Bluetooth Serial
SoftwareSerial BTSerial(6, 7);

// RFID Reader Serial
SoftwareSerial rfid_reader(7, 1);

// IR Sensors
int IR_SENSOR1 = 2;  // front sensor for entrance
int IR_SENSOR2 = 3;  // return sensor for exit

// Motor Pins
int MOTOR1 = 12;  // Motor for Slot 1
int MOTOR2 = 11;  // Motor for Slot 2
int MOTOR3 = 10;  // Motor for Slot 3
int MOTOR4 = 9;   // Motor for Slot 4
int MOTOR5 = 8;   // Motor for Slot 5
int MOTOR6 = 7;   // Motor for Slot 6

// RFID Tag IDs for each slot
char tag_1[] = "270020F5E012";  // Slot 1
char tag_2[] = "270020F68E7F";  // Slot 2
char tag_3[] = "270020F4D023";  // Slot 3
char tag_4[] = "270020F6A7B4";  // Slot 4
char tag_5[] = "270020F9C9D8";  // Slot 5
char tag_6[] = "270020F2B4A6";  // Slot 6

char input[12];  // Variable to store the ID of the Tag being presented
int count = 0;   // Counter variable to navigate through the input[] character array

void setup() {
  lcd.begin(16, 2);
  rfid_reader.begin(9600); // Initialise Serial Communication with the RFID reader
  Serial.begin(9600); // Initialise Serial Communication with the Serial Monitor
  BTSerial.begin(9600); // Initialise Bluetooth Serial Communication
  
  // Motor and Sensor Pins Setup
  pinMode(IR_SENSOR1, INPUT);
  pinMode(IR_SENSOR2, INPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(MOTOR3, OUTPUT);
  pinMode(MOTOR4, OUTPUT);
  pinMode(MOTOR5, OUTPUT);
  pinMode(MOTOR6, OUTPUT);

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
      count = 0; // Reset the counter to zero
      while (rfid_reader.available() && count < 12) {
        input[count] = rfid_reader.read(); // Read 1 Byte of data and store it in the input[] variable
        count++; // increment counter
        delay(5);
      }

      if (count == 12) {
        count = 0; // reset counter variable to 0

        if (matchTag(input, tag_1)) {
          activateSlot(1);
        } else if (matchTag(input, tag_2)) {
          activateSlot(2);
        } else if (matchTag(input, tag_3)) {
          activateSlot(3);
        } else if (matchTag(input, tag_4)) {
          activateSlot(4);
        } else if (matchTag(input, tag_5)) {
          activateSlot(5);
        } else if (matchTag(input, tag_6)) {
          activateSlot(6);
        } else {
          lcd.setCursor(0, 0);
          lcd.print("Wrong Tag");
          Serial.println("Wrong Tag");
        }

        // Clear input buffer
        for (count = 0; count < 12; count++) {
          input[count] = 'F';
        }
        count = 0; // Reset counter variable
      }
    }
  }

  if (IR2 == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("VIST AGAIN");
    lcd.setCursor(0, 1);
    lcd.print("THANKS");

    // You can also add motor control logic for exiting if needed
    delay(5000); // Hold for 5 seconds before clearing the message
    lcd.clear();
  }
}

// Function to match RFID tags
boolean matchTag(char* inputTag, char* validTag) {
  for (int i = 0; i < 12; i++) {
    if (inputTag[i] != validTag[i]) {
      return false;
    }
  }
  return true;
}

// Function to activate specific parking slot
void activateSlot(int slot) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Matched Slot ");
  lcd.print(slot);
  Serial.print("Matched Slot ");
  Serial.println(slot);

  switch (slot) {
    case 1:
      runMotor(MOTOR1, 1000); // Run Motor 1 for Slot 1
      break;
    case 2:
      runMotor(MOTOR2, 1200); // Run Motor 2 for Slot 2
      break;
    case 3:
      runMotor(MOTOR3, 1400); // Run Motor 3 for Slot 3
      break;
    case 4:
      runMotor(MOTOR4, 1600); // Run Motor 4 for Slot 4
      break;
    case 5:
      runMotor(MOTOR5, 1800); // Run Motor 5 for Slot 5
      break;
    case 6:
      runMotor(MOTOR6, 2000); // Run Motor 6 for Slot 6
      break;
    default:
      lcd.clear();
      lcd.print("Unknown Slot");
      break;
  }
}

// Function to run motor for a specific duration
void runMotor(int motorPin, int duration) {
  digitalWrite(motorPin, HIGH);
  delay(duration);  // Run motor for the specified time
  digitalWrite(motorPin, LOW);
}
