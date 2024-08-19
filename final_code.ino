#include <LiquidCrystal.h>
LiquidCrystal lcd( A0,A1,A2,A3,A4,A5);

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(6,7);

#include <SoftwareSerial.h> // include the Software Serial Library
SoftwareSerial rfid_reader(7,1); // Creat a Software Serial Object Mapping it to 7th pin as Rx and 1st Pin as Tx

//IR SENSOR
int IR_SENSOR1 = 2;  // front 
int IR_SENSOR2 = 3;  // return

// motor 
int MOTOR1 = 12;     // front m1
int MOTOR2 = 11;     // front m1
int MOTOR3 = 10;     // return m2
int MOTOR4 = 9;      // return m2


char tag[] ="270020F5E012"; // The Tag ID which we got from Program 16.0
char tag_1[] ="270020F68E7F";
char tag_2[] ="270020F4D023";
char input[12]; // A Variable to store the ID of the Tag being presented
int count = 0; // A counter variable to navigate through the input[] character array
boolean flag = 0; // A variable to store the Tag match status
boolean flag1 = 0; 
boolean flag2 = 0; 
boolean state = 0; // A variable to store the state of the Led to be toggled
void setup()
{
  lcd.begin(16,2);
  rfid_reader.begin(9600); // Initialise Serial Communication with the RFID reader
  Serial.begin(9600); // Initialise Serial Communication with the Serial Monitor
  BTSerial.begin(9600);
  pinMode(13,OUTPUT); // LED output
  pinMode(IR_SENSOR1,INPUT);
  pinMode(IR_SENSOR2,INPUT);
  pinMode(MOTOR1,OUTPUT);
  pinMode(MOTOR2,OUTPUT);
  pinMode(MOTOR3,OUTPUT);
  pinMode(MOTOR4,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("CAR PARKING");
  lcd.setCursor(0,1);
  lcd.print("SYSTEM");
  delay(3000);
  lcd.clear();
}

void loop()
{
   int IR1 = digitalRead(IR_SENSOR1);
   int IR2 = digitalRead(IR_SENSOR2);

if(IR1 == LOW)
{ 
  lcd.setCursor(0,0);
  lcd.print("plz insert card");
  
  if(rfid_reader.available())// Check if there is Incoming Data in the RFID Reader Serial Buffer. 
  {
    count = 0; // Reset the counter to zero
    while(rfid_reader.available() && count < 12) // Keep reading Byte by Byte from the Buffer till the RFID Reader Buffer is empty or till 12 Bytes (the ID size of our Tag) is read
    {
      input[count] = rfid_reader.read(); // Read 1 Byte of data and store it in the input[] variable
      count++; // increment counter
      delay(5);
    }
    if(count == 12) // When the counter reaches 12 (the size of the ID) we stop and compare each value of the input[] to the corresponding stored value
    {
      count =0; // reset counter varibale to 0
      flag = 1;
       flag1 = 1;
        flag2 = 1;
      while(count<12 && flag !=0) // Iterate through each value and compare till either the 12 values are all matching or till the first mistmatch occurs
      {
        if(input[count]==tag[count])
        flag = 1; // everytime the values match, we set the flag variable to 1
        else
        flag=0; // if the ID values don't match, set flag variable to 0 and stop comparing by exiting the while loop
        count++; // increment i

         
      }
       while(count<12 && flag1 !=0) // Iterate through each value and compare till either the 12 values are all matching or till the first mistmatch occurs
      {
        if(input[count]==tag_1[count])
        flag1 = 1; // everytime the values match, we set the flag variable to 1
        else
        flag1=0; // if the ID values don't match, set flag variable to 0 and stop comparing by exiting the while loop
        count++; // increment i
        }

         while(count<12 && flag2 !=0) // Iterate through each value and compare till either the 12 values are all matching or till the first mistmatch occurs
      {
        if(input[count]==tag_2[count])
        flag2 = 1; // everytime the values match, we set the flag variable to 1
        else
        flag2=0; // if the ID values don't match, set flag variable to 0 and stop comparing by exiting the while loop
        count++; // increment i

         
      }
    }
    if(flag == 1) // If flag variable is 1, then it means the tags match
    {
       lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Matched1");
      Serial.println("Matched1!");
      BTSerial.println("270020F5E012");
      state = !state; // Toggle state
      digitalWrite(13,state); // Apply the new state to the LED
   ON();
      
    }
    else if(flag1 == 1) // If flag variable is 1, then it means the tags match
    {
       lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("Matched2");
      Serial.println("Matched2!");
       BTSerial.println("270020F68E7F");
      state = !state; // Toggle state
      digitalWrite(13,state); // Apply the new state to the LED
       ON();
    }
     else if(flag2 == 1) // If flag variable is 1, then it means the tags match
    {
       lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Matched3");
      Serial.println("Matched3!");
       BTSerial.println("270020F4D023");
      state = !state; // Toggle state
      digitalWrite(13,state); // Apply the new state to the LED
       ON();
    }
    else
    {
       lcd.setCursor(0,0);
        lcd.print("Wrong Tag");
      Serial.println("Wrong Tag"); // Incorrect Tag Message
    }
    
    for(count=0; count<12; count++) // Fill the input variable array with a fixed value 'F' to overwrite all values getting it empty for the next read cycle
      {
        input[count]= 'F';
      }
     count = 0; // Reset counter variable  
  }
}


if(IR2 == LOW)
{
    lcd.clear();
   lcd.setCursor(0,0);
  lcd.print("   VIST AGAIN");
  lcd.setCursor(0,1);
  lcd.print("     THANKS");
   
  digitalWrite(MOTOR3,HIGH);
  digitalWrite(MOTOR4,LOW);
  delay(200);
  digitalWrite(MOTOR3,LOW);
  digitalWrite(MOTOR4,LOW);
  delay(5000);
  digitalWrite(MOTOR3,LOW);
  digitalWrite(MOTOR4,HIGH);
  delay(200);
  digitalWrite(MOTOR3,LOW);
  digitalWrite(MOTOR4,LOW);

    lcd.clear();
}
}

void ON()
{
  digitalWrite(MOTOR1,HIGH);
  digitalWrite(MOTOR2,LOW);
  delay(200);
  digitalWrite(MOTOR1,LOW);
  digitalWrite(MOTOR2,LOW);
  delay(5000);
  digitalWrite(MOTOR1,LOW);
  digitalWrite(MOTOR2,HIGH);
  delay(200);
  digitalWrite(MOTOR1,LOW);
  digitalWrite(MOTOR2,LOW);

  lcd.clear();
}

