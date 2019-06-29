#include <stdio.h>
#include <stdlib.h>
#include <Keypad.h>
#include "TimerOne.h"
#include <SoftwareSerial.h>

#define elev1Led 6
#define elev2Led 7
#define elev3Led 8

//bluetooth declarations
SoftwareSerial mySerial(10, 11); // RX, TX
int count = 0 ;

char var;
char incomingdata[2];

/* Each elev has elevnMode 0 off, 1 On and UP, 3 On and Down
                 elevnDest representing it's destination floor
                 elevnCurFloor representing it's current floor
                 destnFloor array represents all leevels where elevator will be opened 0 when no values stored and last value represent next place to be keptandthe number of 
                 already stored dests
                 elevnOpen = 0 when elevator closed and 1 when open
                 */

 int elev1Mode = 0; int elev1CurFloor = 1; int dest1Floor [6]; int elev1Open = 0;
 int elev2Mode = 0; int elev2CurFloor = 1; int dest2Floor [6]; int elev2Open = 0;
 int elev3Mode = 0; int elev3CurFloor = 1; int dest3Floor [6]; int elev3Open = 0;

 int nLevel, enteredMode; //values for user from certain level andwhether he wants to go up (entered Mode = 1) or down (entered Mode= 2)
 int shortestPath1 = 0;
 int shortestPath2 = 0;
 int shortestPath3 = 0;

 void addDest(int , int []);
 void getThreeDistElev();
 void addDestForShortestPath();
 void elev1GoToDest();
 void elev2GoToDest();
 void elev3GoToDest();
//KeyPad declarations
/*
 * 0 reprents floor1 up
 * 1 floor 2 up, 2 floor 2 down
 * 3 floor 3 up, 4 floor 3 down
 * 5 floor 4 up, 6 floor 4 down
 * 7 floor 5 down
 */
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
/*
 * from rigt to left
 * A3 , A2, A1, A0, 2, 3, 4, 5
 */

void setup() {  
  Serial.begin(9600);
  mySerial.begin(9600); // Default communication rate of the Bluetooth module
  Timer1.initialize(2000000);         // initialize timer1, and set 2000ms
  //Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

  pinMode(elev1Led, OUTPUT);
  digitalWrite(elev1Led, LOW);
  pinMode(elev2Led, OUTPUT);
  digitalWrite(elev2Led, LOW);
  pinMode(elev3Led, OUTPUT);
  digitalWrite(elev3Led, LOW);
}

void loop() 
{
  //recieving from external keypad
    char nLevelChar = keypad.getKey();
    if (nLevelChar)
      {
        if(nLevelChar == '0'){nLevel = 1; enteredMode = 1;} 
        else if(nLevelChar == '8'){nLevel = 2; enteredMode = 1;} 
        else if(nLevelChar == '7'){nLevel = 2; enteredMode = 2;} 
        else if(nLevelChar == '5'){nLevel = 3; enteredMode = 1;} 
        else if(nLevelChar == '4'){nLevel = 3; enteredMode = 2;} 
        else if(nLevelChar == '2'){nLevel = 4; enteredMode = 1;} 
        else if(nLevelChar == '1'){nLevel = 4; enteredMode = 1;} 
        else if(nLevelChar == '3'){nLevel = 5; enteredMode = 2;} 

        getThreeDistElev();//get paths difference for all elvators
        addDestForShortestPath(); //assign destination to the elevator of shortest path 
        /*Serial.println("dest1 array");
        for(int i = 0;i<6;i++)
        Serial.println(dest1Floor[i]);
        Serial.println("dest2 array");
        for(int i = 0;i<6;i++)
        Serial.println(dest2Floor[i]);
        Serial.println("dest3 array");
        for(int i = 0;i<6;i++)
        Serial.println(dest3Floor[i]);*/ 
      } 
      //Recieving from program
    if (mySerial.available() > 0)
      { 
        // Checks whether data is comming from the serial port
        var = mySerial.read();
        incomingdata[count] = var;
       // Serial.println(incomingdata[count]);
        count++;
        if(count == 2) 
              {
                /*Serial.println("recieved from bluetooth");
                Serial.println(incomingdata[0]);
                Serial.println(incomingdata[1]);*/
                int data = incomingdata[1] - '0';
                if(incomingdata[0] == '1'){addDest(data,dest1Floor);}//elevator 1
                else if(incomingdata[0] == '2'){addDest(data,dest2Floor);}//elevator 2
                else if(incomingdata[0] == '3'){addDest(data,dest3Floor); Serial.println("dest3 array");}//elevator 3
                
                count=0;
              }
        }
}



void addDestForShortestPath()
{
    if(shortestPath1 < shortestPath2)
    {
      if(shortestPath1 < shortestPath3)
        {addDest(nLevel,dest1Floor);mySerial.write('1');}
      else 
         {addDest(nLevel,dest3Floor);mySerial.write('3');}
    }
    else
    {
      if( shortestPath2 < shortestPath3)
       { addDest(nLevel,dest2Floor); mySerial.write('2');}
      else
        {addDest(nLevel,dest3Floor);mySerial.write('3');}
    }
}

void getThreeDistElev()
{
  
        //for elevator 1
        if(elev1Mode == 0) {shortestPath1 = abs(nLevel - elev1CurFloor);}
        else if (elev1Mode == 1)//Up mode
        {
          if(enteredMode == 1 && nLevel > elev1CurFloor){shortestPath1 = abs(nLevel-elev1CurFloor);}
          else {shortestPath1 = 5;}//won't be seen except if other paths take very long delay of 5 floors
          /*if(nLevel > elev1CurFloor){shortestPath1 = nLevel-elev1CurFloor;}
          else {shortestPath1 = 5;}//won't be seen except if other paths take very long delay of 5 floors*/
        }
         else //Down mode
        {
          if(enteredMode == 2 && nLevel < elev2CurFloor){shortestPath1 = abs(nLevel-elev1CurFloor);}
          else {shortestPath1 = 5;}//won't be seen except if other paths take very long delay of 5 floors
          /*if(nLevel < elev1CurFloor){shortestPath1 = elev1CurFloor-nLevel;}
          else {shortestPath1 = 5;}*/
        }

        //for elevator 2
        if(elev2Mode == 0) {shortestPath2 = abs(nLevel - elev2CurFloor);}
        else if (elev2Mode == 1)//Up mode
        {
          if(enteredMode == 1 && nLevel > elev2CurFloor){shortestPath2 = abs(nLevel-elev2CurFloor);}
          else {shortestPath2 = 5;}
        }
         else //Down mode
        {
          if(enteredMode == 2 && nLevel < elev2CurFloor){shortestPath2 = abs(elev2CurFloor-nLevel);}
          else {shortestPath2 = 5;}
        }

        //for elevator 3
        if(elev3Mode == 0) {shortestPath3 = abs(nLevel - elev3CurFloor);}
        else if (elev3Mode == 1)//Up mode
        {
          if(enteredMode == 1&& nLevel > elev3CurFloor ){shortestPath3 = abs(nLevel-elev3CurFloor);}
          else {shortestPath3 = 5;}
        }
         else //Down mode
        {
          if(enteredMode == 2 && nLevel < elev3CurFloor){shortestPath3 = abs(elev3CurFloor-nLevel);}
          else {shortestPath3 = 5;}
        } 
        /*
        Serial.println("Shortest Path1");
        Serial.println (shortestPath1);
        Serial.println("Shortest Path2");
        Serial.println (shortestPath2);
        Serial.println("Shortest Path3");
        Serial.println (shortestPath3);*/
}

void addDest  (int  newDest, int prevArr [])
{
  for(int i=0;i < prevArr[5];i++)
  {
    if(newDest<= prevArr[i])
    {
      int temp = prevArr[i];
      prevArr[i] = newDest;
      newDest = temp;
      if(newDest == prevArr[i])
        return;
        
    }
  }
  prevArr[prevArr[5]] = newDest;
  prevArr[5]++;
}

void elev1GoToDest()
{
  if(elev1Mode ==0 && dest1Floor[0] !=0)//was off and first destination given
  {
    if(dest1Floor[0] < elev1CurFloor){elev1Mode = 2;}
    else{elev1Mode = 1;}
  }
  elev1Open = 0;
  int i = 0;
  if(elev1Mode == 1)
  {
    //Up Mode
   for(i=0;i<dest1Floor[5];i++)
   {
     if(dest1Floor[i] >= elev1CurFloor)
       break;
   }
   int n;
   n = i;
   if(dest1Floor[n]> elev1CurFloor)
   {
    elev1CurFloor = elev1CurFloor +1;
    if(dest1Floor[n]== elev1CurFloor)
    {//for case level reached from below level
      elev1Open = 1;
      dest1Floor[5] = dest1Floor[5]-1;
      for(int m = n;m< 4;m++)
      {
        dest1Floor[m] = dest1Floor[m+1];
      }
      dest1Floor[4] = 0;
      if(dest1Floor[n] == 0 && dest1Floor[5] == 0) { elev1Mode = 0; }
      else if (dest1Floor[n] == 0 && dest1Floor[5] != 0)
      { elev1Mode = 2;}
    }
   }
   else if (dest1Floor[n] == elev1CurFloor)
   {//for case elevator is in it's place but called from outside
     dest1Floor[5] = dest1Floor[5]-1;
     elev1Open = 1;
     for(int m = n;m< 4;m++)
     {
        dest1Floor[m] = dest1Floor[m+1];
     }
     dest1Floor[4] = 0;
     if(dest1Floor[n] == 0 && dest1Floor[5] == 0){ elev1Mode = 0; }
     else if (dest1Floor[n] == 0 && dest1Floor[5] != 0)
     {
       elev1Mode = 2;
       elev1CurFloor = elev1CurFloor - 1;
     }
     else {elev1CurFloor = elev1CurFloor + 1;}
   }
  }
       
  else  if(elev1Mode == 2)
  {
    //Down mode
    for(i=(dest1Floor[5]- 1);i>0;i--)
    {
      if(dest1Floor[i] <= elev1CurFloor)
        break;
    }
    int n;
    n = i;
    if(dest1Floor[n] < elev1CurFloor)
    {
      elev1CurFloor = elev1CurFloor - 1;
      if(dest1Floor[n]== elev1CurFloor)
      {
        elev1Open = 1;
        dest1Floor[5] = dest1Floor[5]-1;
        for(int m = n;m< 4;m++)
        {
          dest1Floor[m] = dest1Floor[m+1];
        }
        dest1Floor[4] = 0;
        if(n == 0 && dest1Floor[5] == 0){ elev1Mode = 0; }
        else if (n == 0 && dest1Floor[5] != 0) { elev1Mode = 1; }
      }
    }
    else if (dest1Floor[n] == elev1CurFloor)
    {
      elev1Open = 1;
      dest1Floor[5] = dest1Floor[5]-1;
      for(int m = n;m< 4;m++)
      {
        dest1Floor[m] = dest1Floor[m+1];
      }
      dest1Floor[4] = 0;
      if(n == 0 && dest1Floor[5] == 0){ elev1Mode = 0; }
      else if (n == 0 && dest1Floor[5] != 0)
      {
        elev1Mode = 1;
        elev1CurFloor = elev1CurFloor + 1;
      }
      else {elev1CurFloor = elev1CurFloor - 1;}
    }
   }
}

void elev2GoToDest()
{
  if(elev2Mode ==0 && dest2Floor[0] !=0)//was off and first destination given
  {
    if(dest2Floor[0] < elev2CurFloor){elev2Mode = 2;}
    else{elev2Mode = 1;}
  }
  elev2Open = 0;
  int i = 0;
  if(elev2Mode == 1)
  {
    //Up Mode
   for(i=0;i<dest2Floor[5];i++)
   {
     if(dest2Floor[i] >= elev2CurFloor)
       break;
   }
   int n;
   n = i;
   if(dest2Floor[n]> elev2CurFloor)
   {
    elev2CurFloor = elev2CurFloor +1;
    if(dest2Floor[n]== elev2CurFloor)
    {//for case level reached from below level
      elev2Open = 1;
      dest2Floor[5] = dest2Floor[5]-1;
      for(int m = n;m< 4;m++)
      {
        dest2Floor[m] = dest2Floor[m+1];
      }
      dest2Floor[4] = 0;
      if(dest2Floor[n] == 0 && dest2Floor[5] == 0) { elev2Mode = 0; }
      else if (dest2Floor[n] == 0 && dest2Floor[5] != 0) { elev2Mode = 2; }
    }
   }
   else if (dest2Floor[n] == elev2CurFloor)
   {//for case elevator is in it's place but called from outside
     dest2Floor[5] = dest2Floor[5]-1;
     elev2Open = 1;
     for(int m = n;m< 4;m++)
     {
        dest2Floor[m] = dest2Floor[m+1];
     }
     dest2Floor[4] = 0;
     if(dest2Floor[n] == 0 && dest2Floor[5] == 0){ elev2Mode = 0; }
     else if (dest2Floor[n] == 0 && dest2Floor[5] != 0)
     {
       elev2Mode = 2;
       elev2CurFloor = elev2CurFloor - 1;
     }
     else {elev2CurFloor = elev2CurFloor + 1;}
   }
  }
       
  else  if(elev2Mode == 2)
  {
    //Down mode
    for(i=(dest2Floor[5]- 1);i>0;i--)
    {
      if(dest2Floor[i] <= elev2CurFloor)
        break;
    }
    int n;
    n = i;
    if(dest2Floor[n] < elev2CurFloor)
    {
      elev2CurFloor = elev2CurFloor - 1;
      if(dest2Floor[n]== elev2CurFloor)
      {
        elev2Open = 1;
        dest2Floor[5] = dest2Floor[5]-1;
        for(int m = n;m< 4;m++)
        {
          dest2Floor[m] = dest2Floor[m+1];
        }
        dest2Floor[4] = 0;
        if(n == 0 && dest2Floor[5] == 0){ elev2Mode = 0; }
        else if (n == 0 && dest2Floor[5] != 0) { elev2Mode = 1; }
      }
    }
    else if (dest2Floor[n] == elev2CurFloor)
    {
      elev2Open = 1;
      dest2Floor[5] = dest2Floor[5]-1;
      for(int m = n;m< 4;m++)
      {
        dest2Floor[m] = dest2Floor[m+1];
      }
      dest2Floor[4] = 0;
      if(n == 0 && dest2Floor[5] == 0){ elev2Mode = 0; }
      else if (n == 0 && dest2Floor[5] != 0)
      {
        elev2Mode = 1;
        elev2CurFloor = elev2CurFloor + 1;
      }
      else {elev2CurFloor = elev2CurFloor - 1;}
    }
   }
}

void elev3GoToDest()
{
  if(elev3Mode ==0 && dest3Floor[0] !=0)//was off and first destination given
  {
    if(dest3Floor[0] < elev3CurFloor){elev3Mode = 2;}
    else{elev3Mode = 1;}
  }
  elev3Open = 0;
  int i = 0;
  if(elev3Mode == 1)
  {
    //Up Mode
   for(i=0;i<dest3Floor[5];i++)
   {
     if(dest3Floor[i] >= elev3CurFloor)
       break;
   }
   int n;
   n = i;
   if(dest3Floor[n]> elev3CurFloor)
   {
    elev3CurFloor = elev3CurFloor +1;
    if(dest3Floor[n]== elev3CurFloor)
    {//for case level reached from below level
      elev3Open = 1;
      dest3Floor[5] = dest3Floor[5]-1;
      for(int m = n;m< 4;m++)
      {
        dest3Floor[m] = dest3Floor[m+1];
      }
      dest3Floor[4] = 0;
      if(dest3Floor[n] == 0 && dest3Floor[5] == 0) { elev3Mode = 0; }
      else if (dest3Floor[n] == 0 && dest3Floor[5] != 0) { elev3Mode = 2; }
    }
   }
   else if (dest3Floor[n] == elev3CurFloor)
   {//for case elevator is in it's place but called from outside
     dest3Floor[5] = dest3Floor[5]-1;
     elev3Open = 1;
     for(int m = n;m< 4;m++)
     {
        dest3Floor[m] = dest3Floor[m+1];
     }
     dest3Floor[4] = 0;
     if(dest3Floor[n] == 0 && dest3Floor[5] == 0){ elev3Mode = 0; }
     else if (dest3Floor[n] == 0 && dest3Floor[5] != 0)
     {
       elev3Mode = 2;
       elev3CurFloor = elev3CurFloor - 1;
     }
     else {elev3CurFloor = elev3CurFloor + 1;}
   }
  }
       
  else  if(elev3Mode == 2)
  {
    //Down mode
    for(i=(dest3Floor[5]- 1);i>0;i--)
    {
      if(dest3Floor[i] <= elev3CurFloor)
        break;
    }
    int n;
    n = i;
    if(dest3Floor[n] < elev3CurFloor)
    {
      elev3CurFloor = elev3CurFloor - 1;
      if(dest3Floor[n]== elev3CurFloor)
      {
        elev3Open = 1;
        dest3Floor[5] = dest3Floor[5]-1;
        for(int m = n;m< 4;m++)
        {
          dest3Floor[m] = dest3Floor[m+1];
        }
        dest3Floor[4] = 0;
        if(n == 0 && dest3Floor[5] == 0){ elev3Mode = 0; }
        else if (n == 0 && dest3Floor[5] != 0){ elev3Mode = 1; }
      }
    }
    else if (dest3Floor[n] == elev3CurFloor)
    {
      elev3Open = 1;
      dest3Floor[5] = dest3Floor[5]-1;
      for(int m = n;m< 4;m++)
      {
        dest3Floor[m] = dest3Floor[m+1];
      }
      dest3Floor[4] = 0;
      if(n == 0 && dest3Floor[5] == 0){ elev3Mode = 0; }
      else if (n == 0 && dest3Floor[5] != 0)
      {
        elev3Mode = 1;
        elev3CurFloor = elev3CurFloor + 1;
      }
      else {elev3CurFloor = elev3CurFloor - 1;}
    }
   }
}

    // Interrupt is called once a millisecond, 
    void callback() 
    {
        
  //send each elev position and if opened
   
 elev1GoToDest();
 elev2GoToDest();
 elev3GoToDest();

 if(elev1Open == 1){digitalWrite(elev1Led, HIGH);}
 else {digitalWrite(elev1Led, LOW);}
 if(elev2Open == 1){digitalWrite(elev2Led, HIGH);}
 else {digitalWrite(elev2Led, LOW);}
 if(elev3Open == 1){digitalWrite(elev3Led, HIGH);}
 else {digitalWrite(elev3Led, LOW);}

  Serial.println("elevator 1 Current Floor : ");
  Serial.print(elev1CurFloor);
  //Serial.println("elevator 1 Open ");
  //Serial.println( elev1Open);
  Serial.println("elevator 2 Current Floor : ");
  Serial.print(elev2CurFloor);
  //Serial.println("elevator 2 Open ");
  //Serial.println( elev2Open);
  Serial.println("elevator 3 Current Floor : ");
  Serial.print(elev3CurFloor);
  //Serial.println("elevator 3 Open ");
  //Serial.println( elev3Open);
    }
