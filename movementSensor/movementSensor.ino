
// Tilt sensor constants.
const int ledPin = 6 ;
const int tiltSensorPin = 10 ;
int tiltPreviousStatus = LOW;
int movementDelay = 2000;

/* The PIR sensor can be read in a digital way, it gives a HIGH read when it detects presence
 *  It can be adjusted in sensitivity and delay with the two potentiometers included in the board
 */


void tiltSensorSetup(){
  pinMode( ledPin , OUTPUT) ;
  pinMode (tiltSensorPin , INPUT);
}

boolean hasBeenMoved(int pin){
  int currentStatus = digitalRead(pin);
  boolean moved = (currentStatus != tiltPreviousStatus);
  tiltPreviousStatus = currentStatus;
  return moved;
}

int announceMovement(){
  int errorCode = 0;
  Serial.println("Moved");
  digitalWrite( ledPin , HIGH);
  delay(movementDelay);
  digitalWrite( ledPin , LOW);
  return errorCode;
}

void checkMovement(int pin){
  boolean moved = hasBeenMoved(pin);
  if (moved) {
    announceMovement();
  }
}

void setup()
   {
    Serial.begin(9600);
    tiltSensorSetup();
   }

void loop()
   {  
      checkMovement(tiltSensorPin);
   }

