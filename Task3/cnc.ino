#include <SoftwareSerial.h>
#include <Servo.h>
#include <Stepper.h>
#include <Vector.h>


const int stepsPerRevolution = 20; 
Servo myservo;  // create servo object to control a servo
Stepper stepperX(stepsPerRevolution, 2,3,4,5);
Stepper stepperY(stepsPerRevolution, 8,9,10,11);


int Xpos=0,Ypos=0;                 // Actual steppers positions(0-4096)->(0-360°)
int newXpos, newYpos;          // New steppers positions
int xstep,ystep;

Vector<int> xReadings;
Vector<int> yReadings;

SoftwareSerial mySerial(12, 13); // RX, TX
char var;
String temp, incomingData;


void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  
  //Servo
  myservo.attach(6);  // attaches the servo on pin 6 to the servo object
  myservo.write(10); // tell servo to go to position in variable 'pos'
  
 // Stepper
 stepperX.setSpeed(200);
 stepperY.setSpeed(200);
}
void loop()
{
  if (mySerial.available())
  {
    var = mySerial.read();
    temp = String(var);
    if(var == 'x')
    {
      xReadings.PushBack(incomingData.toInt());
      Serial.print("x= "+String(incomingData.toInt()));
      incomingData="";
    }
     else if(var == 'y')
    {
      yReadings.PushBack(incomingData.toInt());
      Serial.println("\ty= "+String(incomingData.toInt()));
      incomingData="";
    }
    else if(var == 'e')
    {
      Serial.println("Start plotting....");
      plot();
      xReadings.Clear();
      yReadings.Clear();
      incomingData="";
    }
    else 
    {
      incomingData+=temp;
    }
  }
}
//-----------------------------------------------------------------------------
// Plot incoming points
//-----------------------------------------------------------------------------
void plot()
{
  Serial.println("Size= "+String(xReadings.Size()));
  for(int i = 0; i< xReadings.Size(); i++)
  {
    newXpos = xReadings[i];//map(xReadings[i],0,50,0,200);
    newYpos = yReadings[i];//map(yReadings[i],0,50,0,200);
    aproachX(newXpos);
    aproachY(newYpos);
    if(i ==0)
    {
      myservo.write(0);
      delay(15);
    }
    delay(10);
  }
  myservo.write(10);
  delay(15);
  Serial.println("Finished!!");
  
//  int i = 0;
//  while(i<=xReadings.Size()) //loop entered to draw full line between two points
//  {   
//    if(newXpos  -  Xpos != 0)   
//    {
//       if(i==2)
//       {
//         for (int pos = 50; pos>= 30; pos -= 1) 
//         {
//          myservo.write(pos);
//          delay(15);
//         }
//       }
//      aproachX(newXpos);
//      aproachY(newYpos);    
//    }
//    else 
//    {
//      if(i ==xReadings.Size()) 
//      {
//        stepperX.step(-Xpos);
//        stepperY.step(-Ypos);
//        newXpos=0;
//        newYpos=0;
//        for (int pos = 30; pos >= 50; pos += 1) 
//        { 
//           myservo.write(pos);              
//            delay(15);                      
//        }
//        break;
//      }
//      else
//      {
//  
//        newXpos = map(xReadings[i],0,50,0,150);
//        newYpos = map(yReadings[i],0,50,0,150);
//        Serial.print("new Xpos= "+String(newXpos));
//        Serial.println("\tnew Ypos= "+String(newYpos));
//  //      int dest = (sq(newXpos-Xpos) + sq(newYpos-Ypos));
//  //      Serial.println("dest= "+String(dest));
//  //      xstep = abs(Xpos-newXpos)/dest;
//  //      ystep = abs(Ypos-newYpos)/dest;
//  //      Serial.print("xStep= "+String(xstep));
//  //      Serial.println("\tyStep= "+String(ystep)); 
//        i++;
//      }
//    }
//    delay(10);
//  }
//   Serial.println("finished");
}
//-----------------------------------------------------------------------------
// Aproach stepperX to the newX position.
//-----------------------------------------------------------------------------
void aproachX(int newX)
{
  int Xdir =newX-Xpos;//step value
  Serial.print("xStep= "+String(Xdir));
  stepperX.step(Xdir);
  Xpos += Xdir;
}
//-----------------------------------------------------------------------------
// Aproach stepperY to the newY position.
//-----------------------------------------------------------------------------
void aproachY(int newY)
{
  int Ydir = newY-Ypos;
  Serial.println("\tyStep= "+String(Ydir));
  stepperY.step(Ydir);
  Ypos += Ydir; 
}
