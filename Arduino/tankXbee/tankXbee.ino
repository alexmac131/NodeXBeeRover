#include <Servo.h>
Servo myservo;

const int pwm_a = 3;   //PWM control for motor outputs 1 and 2 is on digital pin 3
const int pwm_b = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
const int pingPin = 7;  // pinger
const int dir_a = 12;  //direction control for motor outputs 1 and 2 is on digital pin 12
const int dir_b = 13;  //direction control for motor outputs 3 and 4 is on digital pin 13
const int minRange = 40;


struct robotState {
 
 unsigned int turnPowerA;
 unsigned int  turnPowerB;
 unsigned int  turnDelay;
 unsigned int range;
 int RangeArray[25];
 String rangeData;
 String lastCommand;
};
typedef struct robotState RoverData;
RoverData robot;

void setup() {
  myservo.attach(10);
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("-----------");
  Serial.println("setup test ");
  Serial.println("-----------");
}


void loop () {
     Serial.flush();
     if (checkForCommands()) {
          Serial.println ("Data,Good Command," + String(robot.lastCommand));    
     }
   delay(100);
}

void directionSet (char direct) { 
  
  int pos = 0;
  //myservo.write(90);
  //delay(19000);
  Serial.println(direct);
  int count = 0;
  for(pos = 25; pos <= 155; pos += 5)  // goes from 0 degrees to 180 degrees 
  {                  
    // in steps of 1 degree 
    //Serial.print(pos);
    // Serial.print(" ");
    myservo.write(pos);     // tell servo to go to position in variable 'pos' 
    robot.RangeArray[count]  = pingArea ();
    delay(500);
    //Serial.println(robot.RangeArray[count]);
    count++; 
  } 
   myservo.write(90);

  robot.range = robot.RangeArray[12];    
 
  if (robot.range <= minRange && direct == 'F') {
    Serial.println ("ALERT,Sonar, "+ String(robot.range));
    return;
  }

  //Serial.println(robot.range);   
  boolean runCommand = false;
  if (direct == 'L' || direct == 'F') {
      digitalWrite(dir_a, HIGH);  
      digitalWrite(dir_b, HIGH);  
      if (direct == 'L') {
      robot.turnPowerA = 150;
      robot.turnPowerB = 100;
      robot.turnDelay = 290;
      } else {
        robot.turnPowerA = 150;
        robot.turnPowerB = 150;
        robot.turnDelay = 1000;
      }
      runCommand = true;
  }
  else if (direct == 'R' || direct == 'B') { 
      digitalWrite(dir_a, LOW); 
      digitalWrite(dir_b, LOW); 
      if (direct == 'R') {    
        robot.turnPowerA = 150;
        robot.turnPowerB = 100;
        robot.turnDelay = 290;
      } else {
          robot.turnPowerA = 150;
          robot.turnPowerB = 150;
          robot.turnDelay = 1000;
      }
      runCommand = true;      
  }
  else {
      return;
   }
  
  if (!runCommand) {
     return;
  }
   Serial.println("command building now and data ");
  analogWrite(pwm_a, (robot.turnPowerA));   
  analogWrite(pwm_b, (robot.turnPowerB));
  delay(robot.turnDelay); // this controls how long the command is excuted before we return control 
  analogWrite(pwm_a, 0);   
  analogWrite(pwm_b, 0);
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
  if (!stringCommandComplete) {
      return false;
  }
  else {
    //Serial.println("command to run  is " + commandString);
  }
  
  robot.lastCommand = commandString;
  //commandString.toLowerCase();
 Serial.println(commandString);
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
  else if (commandString == "tankdata") {
        sendBackData ();
        sendRadarData();
        return true;
  }
  else {
        return false;
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
