
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D8 (ASS RX) - BT TX no need voltage divider 
//  Arduino D9 (ASS TX) - BT RX through a voltage divider
//  Arduino D2 - Resistor + LED
 
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
AltSoftSerial ASSserial; 
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



byte elevator1 = 11;
byte elevator2 = 12;
byte elevator3 = 13;

char state =' ';
char tri = ' ';
//String sent = "9";
 
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    ASSserial.begin(9600);  
    Serial.println("ASSserial started at 9600");
    Serial.println(" ");
 
    pinMode(elevator1, OUTPUT); 
    digitalWrite(elevator1, LOW);
}
 
void loop()
{
     // Read from the Bluetooth module and turn the LED on and off///////

    if (ASSserial.available())//////
    {
        state = ASSserial.read();
        Serial.println(state);
 
        // The ascii code for 0 is dec 48
        // The ascii code for 1 is dec 49
        switch(state)
        {
          //elev3 elev2 elev1
          case 48: //000
          digitalWrite(elevator1,LOW);
          digitalWrite(elevator2,LOW);
          digitalWrite(elevator3,LOW);
          break; 
          case 49: //001
          digitalWrite(elevator1,HIGH);
          digitalWrite(elevator2,LOW);
          digitalWrite(elevator3,LOW);
          break; 
          case 50: //010
          digitalWrite(elevator1,LOW);
          digitalWrite(elevator2,HIGH);
          digitalWrite(elevator3,LOW);
          break;
          case 51: //011
          digitalWrite(elevator1,HIGH);
          digitalWrite(elevator2,HIGH);
          digitalWrite(elevator3,LOW);
          break;
          case 52: //100
          digitalWrite(elevator1,LOW);
          digitalWrite(elevator2,LOW);
          digitalWrite(elevator3,HIGH);
          break;
          case 53: //101
          digitalWrite(elevator1,HIGH);
          digitalWrite(elevator2,LOW);
          digitalWrite(elevator3,HIGH);
          break;
          case 54: //110
          digitalWrite(elevator1,LOW);
          digitalWrite(elevator2,HIGH);
          digitalWrite(elevator3,HIGH);
          break;
          case 55: //111
          digitalWrite(elevator1,HIGH);
          digitalWrite(elevator2,HIGH);
          digitalWrite(elevator3,HIGH);
          break;
        }
        
    }
  ////////////KeyPAd///////////////////////////
    char key = keypad.getKey();
  
   if (key){
   if(key == '1') {   ASSserial.print( Serial.read() );/* ASSserial.println("1 , UP");*/}
   if(key == '2') { Serial.println("2 , UP"); /*ASSserial.println("2 , UP");*/}
   if(key == '3') { Serial.println("2 , DOWN"); }
   if(key == '4') { Serial.println("3 , UP"); }
   if(key == '5') { Serial.println("3 , DOWN"); }
   if(key == '6') { Serial.println("4 , UP"); }
   if(key == '7') { Serial.println("4 , DOWN"); }
   if(key == '8') { Serial.println("5 , DOWN"); }
    
  }
   if (ASSserial.available()) {
    //ASSserial.println(sent);
    tri = ASSserial.read();
    Serial.println(tri);
     }
}
