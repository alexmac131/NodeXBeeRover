  
const int pwm_a = 3;   //PWM control for motor outputs 1 and 2 is on digital pin 3
const int redPin = 4 ; // Red LED,   connected to digital pin 9
const int greenPin = 5;  // Green LED, connected to digital pin 10
const int bluePin = 6;  // Blue LED,  connected to digital pin 11
const int pingPin = 7;  // pinger
const int pwm_b = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
const int dir_a = 12;  //direction control for motor outputs 1 and 2 is on digital pin 12
const int dir_b = 13;  //direction control for motor outputs 3 and 4 is on digital pin 13
const int minRange = 40;


int val = 0;     //value for fade
int blank[3] = { 0, 0, 0};
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int rangeNow = 0;

void setup() {
  
  pinMode(redPin, OUTPUT);   // sets the pins as output
  pinMode(greenPin, OUTPUT);   
  pinMode(bluePin, OUTPUT); 
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  
  analogWrite(pwm_a, 0);  //set both motors to run at (100/255 = 39)% duty cycle (slow)
  analogWrite(pwm_b, 0);
  Serial.begin(9600);
}


void loop () {
  
   String receivedCommand = "";
   receivedCommand = checkForCommands();
   if (receivedCommand.length() > 1) {
     if (!runCommand(receivedCommand)) {
         Serial.println ("Alert,Bad Command," + receivedCommand);
     }  
   } 
   receivedCommand  = "";
   signalLED(blank);
   delay(500);
 }

void directionSet (char directon) { 
  float turnPowerA = 215;
  float turnPowerB = 215;
  float  balance = .9;
  int turnDelay = 900;
  
  rangeNow = pingArea ();    
  switch (directon) {
    case 'L':  // LEFT TURN
      digitalWrite(dir_a, HIGH);  
      digitalWrite(dir_b, HIGH);  
      turnPowerA = 190;
      turnPowerB = 40;
      turnDelay = 290;
      break;
    case 'R':  // RIGHT TURN
      digitalWrite(dir_a, LOW); 
      digitalWrite(dir_b, LOW); 
      turnPowerA = 190;
      turnPowerB = 40;
      turnDelay = 290;
      break;  
    case 'F':  // FORWARD
      if (rangeNow < minRange) {
        Serial.println ("ALERT,Sonar, "+ String(rangeNow));
        blinkSpeed(450);
        return;
      }
      digitalWrite(dir_a, HIGH);  
      digitalWrite(dir_b, HIGH);   
      // tracks do not turn equally  so we adjust the power to make it go straight.
      turnPowerA = turnPowerB * balance;
      break;
    case 'B': // BACK
        digitalWrite(dir_a, LOW);  
        digitalWrite(dir_b, LOW);
        turnPowerA = turnPowerB * balance;
        break;  
    case 'S':  // STOP
      digitalWrite(dir_a, LOW); 
      digitalWrite(dir_b, LOW); 
      turnPowerA = 0;
      turnPowerB = 0;
      turnDelay = 1;
      break;
    
    default:
      return;
      break;
  }  
  char tmp[20];
  
  String Message  = String (directon) + "," + dtostrf(turnPowerA, 10, 4, tmp) + ","  ;
    
  Message =  Message + dtostrf(turnPowerB, 10, 4, tmp) + "," + String(turnDelay);  
  Message =  "Rover," + Message + "," + String(rangeNow);
  Serial.println(Message);
  
  analogWrite(pwm_a, turnPowerA);   
  analogWrite(pwm_b, turnPowerB);
  delay(turnDelay); // this controls how long the command is excuted before we return control 
  analogWrite(pwm_a, 0);   
  analogWrite(pwm_b, 0);
  
 }
 
 
void blinkSpeed (int speeded) {
      
      signalLED(red);
      delay(speeded);
      signalLED(blank);
      delay(speeded);
      signalLED(red); 
      delay(speeded);
      signalLED(blank);
      delay(speeded);
      signalLED(red);
      delay(speeded);
      signalLED(blank);
      delay(speeded);
      signalLED(red);
      delay(speeded);
      signalLED(blank);
}

int pingArea(){
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

void signalLED  (int color[3]) {
  int  R = (color[0] * 255) / 100 ;
  int  G =  (color[1] * 255 ) /100 ;
  int  B = ( color[2] * 255) / 100 ;
  
  analogWrite(redPin, R);   // Write current values to LED pins
  analogWrite(greenPin, G);      
  analogWrite(bluePin, B);
}

String checkForCommands () {
  String commandString = ""; 
  boolean stringCommandComplete = false;  
  
  if (Serial.available() > 0) {
    //stringCommandComplete = false; 
    while (Serial.available()) {
      char inChar = (char)Serial.read(); 
      if((int)inChar == 13 || (int)inChar == 118) {
         stringCommandComplete = true;
         break;    
      }
      else {
          commandString += inChar;
      }
   }
  }
  if (commandString.length() > 0) {
    blinkSpeed(50);
  }
  return commandString;  
}

boolean runCommand (String CMD) {
  if (CMD.equalsIgnoreCase("left")) {
        directionSet('L');
        return true;      
  }
  else if (CMD.equalsIgnoreCase("right")) {
        directionSet  ('R');      
        return true;      
  }
  else if (CMD.equalsIgnoreCase("back")) {
        directionSet ('B');      
        return true;      
  }  
  else if (CMD.equalsIgnoreCase("stop")) {
        directionSet ('S');
        return true;
  }
  else if (CMD.equalsIgnoreCase("forward")) {
        directionSet ('F');
        return true;
  }
  else {
        return false;
  }
  return false;
}
