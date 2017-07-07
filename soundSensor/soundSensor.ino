

const int ledPin = 6 ;
const int soundSensorAnalogPin = A0 ;
const int soundSensorDigitalPin = 12 ;
const int soundThreshold = 200;

void soundSensorSetup(){
  Serial.begin(9600);
  pinMode( ledPin , OUTPUT) ;
  pinMode (soundSensorAnalogPin , INPUT);
  pinMode (soundSensorDigitalPin , INPUT);
}

boolean soundDetected(int pin){
  if (digitalRead( pin))
          return true;
      else
          return false;
}

int anounceSoundDetected(){
  digitalWrite( ledPin , HIGH);
  delay(1000);
  digitalWrite( ledPin , LOW);
}

int checkSound(int pin){
//My sensor seems to be very crappy, digital read was always on despite the adjustment.
//Only the Analog read can be used, it's quite bad but at least usable.
/*  if (soundDetected(soundSensorDigitalPin)){
    anounceSoundDetected();
  }*/
  
  if (readSound(soundSensorDigitalPin)>soundThreshold){
    anounceSoundDetected();
  }
}

int readSound(int pin){
  int soundVol = analogRead(pin);
  Serial.println(soundVol);
  return soundVol;
}

void setup()
{  
  soundSensorSetup();
}

void loop()
   {  
    checkSound(soundSensorDigitalPin);
    readSound(soundSensorAnalogPin);    
   }
