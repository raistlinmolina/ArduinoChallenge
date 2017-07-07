
// Presence sensor constants.
const int ledPin = 6 ;
const int pirSensorPin = 7 ;

/* The PIR sensor can be read in a digital way, it gives a HIGH read when it detects presence
 *  It can be adjusted in sensitivity and delay with the two potentiometers included in the board
 */

void presenceSensorSetup(){
  pinMode( ledPin , OUTPUT) ;
  pinMode (pirSensorPin , INPUT);
}

boolean presenceDetected(int pin){
  if (digitalRead( pin))
          return true;
      else
          return false;
}

void setup()
   {  
      presenceSensorSetup();
   }

void loop()
   {  
      if (presenceDetected( pirSensorPin))
          digitalWrite( ledPin , HIGH);
      else
          digitalWrite( ledPin , LOW);
   }


