long distance;
long time;
const int ledPin = 6 ;
const int usonicSensorPinOut = 9; // Pin to trigger the usound pulse
const int usonicSensorPinIn = 8; // Pin to read the usound rebound time
const int usonicSensorDelay = 5; // Delay to let the sensor stabilize
const int usonicSensorReadDelay = 10; // Delay to allow the sent pulse to complete
const float formula = 0.017; // Multiplier to pass sensor reading to cm
/* Sound speed in cm/microsecond divide by two as the time it took the bounce 
is double the time to travel the distance*/
const long carParkedThreshold = 100; //If we read less than this cm we assume there is a car parked under.
const int timesToRead = 3; // Times to read the distance to determine it.


void uSoundSensorSetup(){
  Serial.begin(9600);
  pinMode(usonicSensorPinOut, OUTPUT); 
  pinMode(usonicSensorPinIn, INPUT);
  pinMode( ledPin , OUTPUT) ; 
}

long readDistance (int times){
  long distance[3];
  for (int i = 0; i<times; i++){
    digitalWrite(usonicSensorPinOut,LOW); //We first stabilize the sensor
    delayMicroseconds(5);
    digitalWrite(usonicSensorPinOut, HIGH); // We send the usonic pulse
    delayMicroseconds(10);
    digitalWrite(usonicSensorPinOut, LOW);
    
    /*Function to read the time that has passed since we sent the pulse until we receive the echo*/
    time=pulseIn(usonicSensorPinIn, HIGH); 
    
    distance[i]= int(0.017*time);
    delay(100); 
  }
  
  long total = 0;
  for (int i = 0; i<times; i++){
    total = total + distance[i];
  }
  total = total / times;
  
  /*Monitorización en centímetros por el monitor serial*/
  Serial.println("Distance ");
  Serial.println(total);
  Serial.println(" cm.");
  delay(1000);
  return total;
}

boolean isCarParked(){
  if (readDistance(timesToRead) < carParkedThreshold) return true; else return false;
}

int announceCarParked(){
  Serial.println("There is car under the sensor");
}

int checkCarParked(){
  boolean carParked = isCarParked();
  if (carParked){
    announceCarParked();
    digitalWrite( ledPin , HIGH);
  }else{
    digitalWrite( ledPin , LOW);
  }
}

void setup(){
  uSoundSensorSetup();
}

void loop(){
  checkCarParked();
  }


