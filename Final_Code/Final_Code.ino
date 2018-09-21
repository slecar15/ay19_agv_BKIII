#include <Servo.h>
const int pingPinFront = 2;
const int pingPinLeft = 3;
const int pingPinRight = 4;

int ir_inL = 5;
int ir_inR = 6;
int ir_inF = 7;
int ir_out = 8;
boolean readingL;
boolean readingR;
boolean readingF;
long durationF, durationR, cmF, cmR, cm, cmL, durationL;
int lightL; //analog input is int, not float
int lightM;
int lightR;


Servo myservoL, myservoR;

int i = 0;
const int dur = 1;


void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(ir_out, OUTPUT);
  pinMode(ir_inL, INPUT);
  pinMode(ir_inR, INPUT);
  pinMode(ir_inF, INPUT);

  myservoL.attach(10);
  myservoR.attach(11);

  for (i = 0; i < dur; i++ ) // Initial pause from turning the robot on
 { 
  myservoL.writeMicroseconds(1500);    // motor stop 
  myservoR.writeMicroseconds(1500);
  delay(1000); 
  }
  
}

void loop() {
  cantTurnRight(); // Should start on the wall based on initial function
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  if (voltage < 1.5) {
    canTurnRightDark();
  }
  else{
    canTurnRight();
}
}// loop()

void lightTurnR() {
  for (i = 0; i< dur; i++) {
     myservoR.writeMicroseconds(1600);     // Turns 90 degrees to the right
     myservoL.writeMicroseconds(1400);
     delay(1025);                           
  }
}

void lightTurnL() {
  for (i = 0; i< dur; i++) {
     myservoR.writeMicroseconds(1400);     // Turns 90 degrees to the left
     myservoL.writeMicroseconds(1600);     
     delay(1025);                          
  }
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

long checkRight() {
  pinMode(pingPinRight, OUTPUT);
  digitalWrite(pingPinRight, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPinRight, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPinRight, LOW);
  pinMode(pingPinRight, INPUT);
  durationR = pulseIn(pingPinRight, HIGH);
  cmR = microsecondsToCentimeters(durationR);
  return cmR;
}
  
long checkForward() {
  pinMode(pingPinFront, OUTPUT);
    digitalWrite(pingPinFront, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPinFront, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPinFront, LOW);
    pinMode(pingPinFront, INPUT);
    durationF = pulseIn(pingPinFront, HIGH);
    cmF = microsecondsToCentimeters(durationF);
    return cmF;
}

void cantTurnRight() {

  tone(ir_out, 38000);         // Flash the IR lights
  readingF = digitalRead(ir_inF);
  
  cmR = checkRight();
  
  
  while((cmR < 30)){
  
  readingR = digitalRead(ir_inR);
  readingF = digitalRead(ir_inF);
  
    cmF = checkForward();

    if((cmF < 15) || (readingF == 1)){           // Nothing in front
       cantGoForward();
  }// cmF < 15
    else{                                  // Something in front
      myservoL.writeMicroseconds(1625);    // Quarter speed in one direction
      myservoR.writeMicroseconds(1625); 
  }// else
  readingF = digitalRead(ir_inF);
  cmR = checkRight();

  delay(100);
}// cmR < 30
} // cantTurnRight

void cantGoForward() {
  myservoL.writeMicroseconds(1500);    // Motor stop
  myservoR.writeMicroseconds(1500);
  delay(500);
      
  lightTurnL();                        // Turns left
  delay(200);
  myservoL.writeMicroseconds(1500);    // Motor stop
  myservoR.writeMicroseconds(1500);
  delay(500);
} // cantGoForward

void canTurnRight() {
    
    myservoL.writeMicroseconds(1500);    // motor stop 
    myservoR.writeMicroseconds(1500);
    delay(1000);
    
    cmF = checkForward();
    delay(50);
    cmR = checkRight();
    delay(50);

    lightTurnR();
    myservoL.writeMicroseconds(1500);    // motor stop 
    myservoR.writeMicroseconds(1500);
    delay(300);
    
    int count = 0;
    
    while (count < 3){
      
      cmF = checkForward();
      if (cmF < 30) {
        myservoL.writeMicroseconds(1500);    // motor stop 
       myservoR.writeMicroseconds(1500);
        delay(300);
        lightTurnL();
       myservoL.writeMicroseconds(1500);    // motor stop 
       myservoR.writeMicroseconds(1500);
       delay(300);
       count = 3;
      } 
      
      else {
        myservoL.writeMicroseconds(1625);    // motor stop 
        myservoR.writeMicroseconds(1625);
        delay(400);
        myservoL.writeMicroseconds(1500);    // motor stop 
        myservoR.writeMicroseconds(1500);
        delay(300);
      }
      count += 1;
      cmF = checkForward();
    } // while (count < 3) 
  
    delay(50);
} // canTurnRight

void canTurnRightDark() {
    
    myservoL.writeMicroseconds(1500);    // motor stop 
    myservoR.writeMicroseconds(1500);
    delay(1000);
    
    cmF = checkForward();
    delay(50);
    cmR = checkRight();
    delay(50);
    
    lightTurnR();
    myservoL.writeMicroseconds(1500);    // motor stop 
    myservoR.writeMicroseconds(1500);
    delay(300);
    
    int count = 0;
    
    while (count < 3){
      
      cmF = checkForward();
      if (cmF < 30) {
        myservoL.writeMicroseconds(1500);    // motor stop 
        myservoR.writeMicroseconds(1500);
        delay(300);
        lightTurnL();
        myservoL.writeMicroseconds(1500);    // motor stop 
        myservoR.writeMicroseconds(1500);
        delay(300);
        count = 3;
      }
      
      else {
        myservoL.writeMicroseconds(1625);    // motor stop 
        myservoR.writeMicroseconds(1625);
        delay(400);
        myservoL.writeMicroseconds(1500);    // motor stop 
        myservoR.writeMicroseconds(1500);
        delay(300);
      }
      count += 1;
      cmF = checkForward();
    } // while (count < 3)
    
    /* PUT THE PHOTORESISTOR */
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
    
    if (voltage > 1.00){                      // Read if photoresistor sees above 1.00
      myservoL.writeMicroseconds(1500);       // motor stop 
      myservoR.writeMicroseconds(1500);
      delay(30000);                           // Overall stop because it found the light source
    } // if (voltage < 1.50);
    
    delay(50);
} // canTurnRightDark