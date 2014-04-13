#include <Servo.h>
Servo myservo;

const int servoPin = 9;
const int pwm_a = 3;   //PWM control for motor outputs 1 and 2 is on digital pin 3
const int pwm_b = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
const int pingPin = 7;  // pinger
const int dir_a = 12;  //direction control for motor outputs 1 and 2 is on digital pin 12
const int dir_b = 13;  //direction control for motor outputs 3 and 4 is on digital pin 13
const int minRange = 30;


struct robotState {
 
 unsigned int turnPowerA;
 unsigned int  turnPowerB;
 unsigned int  turnDelay;
 unsigned int turnPowerASaved;
 unsigned int  turnPowerBSaved;
 unsigned int  turnDelaySaved;
 boolean overRide ;
 unsigned int range;
 int RangeArray[25];
 String rangeData;
 String lastCommand;
};
typedef struct robotState RoverData;
RoverData robot;

void setup() {
  robot.overRide = false;
  myservo.attach(servoPin);
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  
  Serial.begin(9600);;
  radarSweep();
  sendBackData ();
  Serial.println("-----------");
  Serial.println("setup ready ");
  Serial.println("-----------");
  
}


void loop () {
     Serial.flush();
     if (checkForCommands()) {
          //Serial.println ("Data,Good Command," + String(robot.lastCommand));    
     }
     myservo.detach();
    delay(300);
}

void directionSet (char direct) { 
  
  //radarSweep();
  
  if (robot.overRide) {
    //Serial.println("we have an override");
    if (robot.range <= minRange ) {
      
      //Serial.println("we have an override ABORT");
      Serial.println ("ALERT,Sonar, "+ String(robot.range));
      robot.overRide = false;
      return;
    }
    // Serial.println("command building now and data ");
    
    analogWrite(pwm_a, (robot.turnPowerA));   
    analogWrite(pwm_b, (robot.turnPowerB));
    delay(robot.turnDelay); // this controls how long the command is excuted before we return control 
    analogWrite(pwm_a, 0);   
    analogWrite(pwm_b, 0);
    radarSweep();
    robot.overRide = false;
    //Serial.print("turn A ");
    //Serial.println(robot.turnPowerA);
    //Serial.print("turn b ");
    //Serial.println(robot.turnPowerB);
    //Serial.print(" delay ");
    //Serial.println(robot.turnDelay);
    //Serial.println("we have completed an  override");
    return;
  }
  
  
  if (robot.range <= minRange && direct == 'F') {
    Serial.println ("ALERT,Sonar, "+ String(robot.range));
    return;
  }

  //Serial.println(robot.range);   
  boolean runCommand = false;
  if (direct == 'F') {
      digitalWrite(dir_a, HIGH);  
      digitalWrite(dir_b, HIGH);  
      robot.turnPowerA = 100;
      robot.turnPowerB = 100;
      robot.turnDelay = 2400;
      runCommand = true;
  }
  else if (direct == 'L') { 
      digitalWrite(dir_a, HIGH); 
      digitalWrite(dir_b, HIGH); 
      robot.turnPowerA = 100;
      robot.turnPowerB = 10;
      robot.turnDelay = 800;
      runCommand = true;      
  }
  else if (direct == 'R') { 
      digitalWrite(dir_a, HIGH); 
      digitalWrite(dir_b, HIGH); 
      robot.turnPowerA = 10;
      robot.turnPowerB = 100;
      robot.turnDelay = 800;
      runCommand = true;      
  }
  else if (direct == 'B') { 
      digitalWrite(dir_a, LOW); 
      digitalWrite(dir_b, LOW); 
      robot.turnPowerA = 100;
      robot.turnPowerB = 100;
      robot.turnDelay = 2400;
      runCommand = true;      
  }
  else {
      return;
  }
  
  if (!runCommand) {
     return;
  }
  // Serial.println("command building now and data ");
  analogWrite(pwm_a, (robot.turnPowerA));   
  analogWrite(pwm_b, (robot.turnPowerB));
  delay(robot.turnDelay); // this controls how long the command is excuted before we return control 
  analogWrite(pwm_a, 0);   
  analogWrite(pwm_b, 0);
  radarSweep();
  return;
}

unsigned int pingArea(){
  long duration, cm;
  
  pinMode(pingPin, OUTPUT);   // clear the eyes
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pingPin, HIGH); // look 
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  
  pinMode(pingPin, INPUT);  // listen
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  return cm;
}

long microsecondsToCentimeters(long microseconds){
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

boolean checkForCommands () {
  String  commandString = ""; 
  boolean stringCommandComplete = false;  
    
  

  while (Serial.available()) {
      char inChar = Serial.read();
      
      if((int)inChar == 13 || (int)inChar == 10) {
         stringCommandComplete = true;
         break;    
      }
      else {
          commandString +=  inChar;  
      }
      delay(40);
   }
  Serial.flush();
  if (!stringCommandComplete || commandString.length() < 2) {
      return false;
  }
  else {
    //Serial.println("command to run  is " + commandString);
  }
  
  robot.lastCommand = commandString;
  //commandString.toLowerCase();
 //Serial.println(commandString);
     int testForEngine = commandString.indexOf("engineleft:");
     if (testForEngine != -1) {  
       String dataT = commandString.substring(commandString.indexOf(":") + 1);
         //Serial.println("we have a left command for the engine  " + dataT );
         robot.overRide = true;
         robot.turnPowerASaved =  robot.turnPowerA;
         robot.turnPowerA =dataT.toInt();
         if (robot.turnPowerA > 100) {
           robot.turnPowerA = 100;
         }

         return true;
       
     }
  
     testForEngine = commandString.indexOf("engineright:");
     if (testForEngine != -1) {  
         String dataT = commandString.substring(commandString.indexOf(":") +1 );
         //Serial.println("we have a right command for the engine " + dataT );
         robot.overRide = true;
         robot.turnPowerBSaved =  robot.turnPowerB;
         robot.turnPowerB =dataT.toInt();
         if (robot.turnPowerB > 100) {
           robot.turnPowerB = 100;
         }
         return true;
     }
  
     testForEngine = commandString.indexOf("enginepower:");
     if (testForEngine != -1) {
       String dataT = commandString.substring(commandString.indexOf(":") + 1);
         //Serial.println("we have a power command for the engine   " + dataT );
         robot.overRide = true;
         robot.turnDelaySaved =  robot.turnDelay;
         robot.turnDelay =dataT.toInt();
         if (robot.turnDelay < 400) {
            robot.turnDelay = 400;  
         } 
         return true;
     }
   
  if (commandString == "left") {
        directionSet('L');
        return true;      
  }
  else if (commandString == "right") {
        directionSet  ('R');      
        return true;      
  }
  else if (commandString == "back") {
        directionSet ('B');      
        return true;      
  } 
  else if (commandString == "forward") {
        directionSet ('F');
        return true;
  }
  else {
      sendBackData ();
      return false;;
  }
  return false;
}

void sendRadarData () {
  Serial.print("radar,");
  for (int c = 0; c < 25; c++) {
    // r obot.rangeData .=  212,22,2,222,22252,2424521,11
    Serial.print(robot.RangeArray[c]);
    if (c < 24) {
     Serial.print(",");
    }  
  }
  Serial.println(); 
}

void sendBackData () {
  String Message  =  "rover," + robot.lastCommand + "," + String(robot.turnPowerA)  + "," + String(robot.turnPowerB) + "," + String(robot.turnDelay) + "," + String(robot.range);
  Serial.println(Message);  
  
}

void radarSweep () {  
  int pos = 0;
  //myservo.write(90);
  //delay(19000);
  //Serial.println(direct);
  int count = 0;
  for(pos = 25; pos <= 155; pos += 5)  // goes from 0 degrees to 180 degrees 
  {                  
    // in steps of 1 degree 
    //Serial.print(pos);
    // Serial.print(" ");
    myservo.attach(servoPin);
    myservo.write(pos);     // tell servo to go to position in variable 'pos'
    delay(200);
    myservo.detach();
    delay(200); 
    robot.RangeArray[count]  = pingArea ();
    //Serial.print(robot.RangeArray[count]);
    //Serial.print(" pos ");
    //Serial.println(pos);
    count++; 
  } 
  myservo.attach(servoPin);
  myservo.write(90);
  delay(200);
  myservo.detach();

  robot.range = robot.RangeArray[12];    
  sendRadarData();
}
