#include <Servo.h>
#include <Keypad.h>
#include <Stepper.h>

//KeyPad declarations
const byte ROWS = 2; //four rows
const byte COLS = 2; //four columns
char keys[ROWS][COLS] = {
  {'1','2'},
  {'4','5'},
  };
//from right to left - - A1 A0 - - A3 A2
byte rowPins[ROWS] = { A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0 , A1}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//Stepper declarations
const int stepsPerRevolution = 20;
Stepper finger1(stepsPerRevolution, 2,3,4,5);
int fing1Pos=0,fing2Pos=0;                 // Actual steppers positions(0-4096)->(0-360°)

Servo finger2;

void setup() {  
  Serial.begin(9600);
  finger2.attach(7);
  finger1.setSpeed(200);
  finger2.write(0);
}

void loop() {
    char key = keypad.getKey();
  
  if (key){
   if(key == '1') { goToPos(180); Serial.println(fing1Pos);}
   if(key == '4') { goToPos(270);Serial.println(fing1Pos);}
   if(key == '2') { finger2.write(0); }
   if(key == '5') { finger2.write(300); }
  }
  
}
void goToPos(int newPos)
{
  //to goto certain position by finger1 stepper motor
  int diff = newPos - fing1Pos; int stepNo;
  if(diff == 0)
    return;
  else if (diff > 0)
  {
    stepNo = 1;
    while(fing1Pos < newPos)
    {
      finger1.step(stepNo);
      fing1Pos++;
    }
  }
  else
  {
    stepNo = -1;
    while(fing1Pos > newPos)
    {
      finger1.step(stepNo);
      fing1Pos--;
    } 
  }
}
