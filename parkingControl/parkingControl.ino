/*
*/



#include <SoftwareSerial.h>
//Only used inside the post method, move them there when definitive values are set
const String server = "raistlinmolina.000webhostapp.com";
const String uri = "/esppost.php";
String lastData = "";



long distance;
long time;

//Pin setup
const int ledPin = 6 ;
const int wifiRxPin = 3;
const int wifiTxPin = 2;
const int usonicSensorPinOut = 9; // Pin to trigger the usound pulse
const int usonicSensorPinIn = 8; // Pin to read the usound rebound time
const int pirSensorPin = 7 ;
const int tiltSensorPin = 10 ;


const int usonicSensorDelay = 5; // Delay to let the sensor stabilize
const int usonicSensorReadDelay = 10; // Delay to allow the sent pulse to complete
const float formula = 0.017; // Multiplier to pass sensor reading to cm
/* Sound speed in cm/microsecond divide by two as the time it took the bounce 
is double the time to travel the distance*/
const long carParkedThreshold = 100; //If we read less than this cm we assume there is a car parked under.
const int timesToRead = 3; // Times to read the distance to determine it.

int tiltPreviousStatus = LOW;
int movementDelay = 2000;

SoftwareSerial WIFI1(wifiRxPin, wifiTxPin); // RX | TX (Tx | Rx in the ESP respectively)


void uSoundSensorSetup(){
  Serial.begin(9600);
  pinMode(usonicSensorPinOut, OUTPUT); 
  pinMode(usonicSensorPinIn, INPUT);
   
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
  ///delay(1000);
  return total;
}

boolean isCarParked(){
  if (readDistance(timesToRead) < carParkedThreshold) return true; else return false;
}

void presenceSensorSetup(){
  pinMode (pirSensorPin , INPUT);
}

void tiltSensorSetup(){
  pinMode (tiltSensorPin , INPUT);
}

boolean hasBeenMoved(){
  int currentStatus = digitalRead(tiltSensorPin);
  boolean moved = (currentStatus != tiltPreviousStatus);
  tiltPreviousStatus = currentStatus;
  return moved;
}

boolean presenceDetected(){
  if (digitalRead(pirSensorPin))
          return true;
      else
          return false;
}

String getLineWIFI()
{
  String S = "";
  if (WIFI1.available())
  {
    char c = WIFI1.read();
    while ( c != '\n' )
    {
      S = S + c ;
      delay(25) ;
      c = WIFI1.read();
    }
    return ( S ) ;
  }
}

//reset the esp8266 module
void reset()
{
  WIFI1.println("AT+RST");
  delay(1000);
  if (WIFI1.find("OK") ) Serial.println("Module Reset");
}

// Connect to WIFI
void setupWifi() {
  String commands[] =
  {
    "AT+CWMODE=1",
    "AT+CWQAP",
    "AT+CWJAP=\"Valhalla\",\"xxxxx\"",
    "AT+CIFSR" ,
    //"AT+CIPMUX=1",
    "END"
  };

  int index = 0;
  while (commands[index] != "END")
  {
    WIFI1.println(commands[index++]);
    while ( true)
    { String s = getLineWIFI();
      if ( s != "") Serial.println(s);
      if ( s.startsWith("no change"))
        break;
      if ( s.startsWith("OK"))
        break;
      if ( s.startsWith("ready"))
        break;
      // if (millis()-T >10000) break;
    }
    Serial.println("....................");
  }
}

String gatherData(){
  String data;
  String parked;
  String presence;
  String tilt;
  
  
  if (isCarParked()) parked="YES"; else parked="NO";
  if (presenceDetected()) presence="YES"; else presence="NO";
  if (hasBeenMoved()) tilt="YES"; else tilt="NO";
  String sound = "NO";
  
  data = "parked=" + parked + "&presence=" + presence + "&sound=" + sound + "&tilt=" + tilt;

  return data;
}

void httppost (String data)
{

  WIFI1.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  Serial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");
  delay(500);
  if ( WIFI1.find("OK"))
  {
    Serial.println("TCP connection ready");
  }
    
    
  String postRequest =
    "POST " + uri + " HTTP/1.1\r\n" +
    "Host:" + server + "\r\n" +
    "Accept:" + "*/" + "*\r\n" +
    "Content-Length:" + data.length() + "\r\n" +
    "Content-Type:application/x-www-form-urlencoded\r\n" +
    "\r\n" +
    data + "\r\n";
  //Serial.println(postRequest);
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  Serial.print(sendCmd);
  Serial.println(postRequest.length() );
  WIFI1.print(sendCmd);
  WIFI1.println(postRequest.length() );
  delay(500);
  if (WIFI1.find(">"))
  {
    Serial.println("Sending.."); WIFI1.print(postRequest);
    if ( WIFI1.find("SEND OK"))
    {
      Serial.println("Packet sent");
      Serial.println(postRequest);
      delay(1000);
      while (WIFI1.available())
      {
        String tmpResp = WIFI1.readString();
        Serial.println(tmpResp);
      }
      // close the connection
      WIFI1.println("AT+CIPCLOSE");
      Serial.println("AT+CIPCLOSE");
    }
  }else{
    Serial.println("No response");
  }
}

void setup()
{
  pinMode( ledPin , OUTPUT) ;
  uSoundSensorSetup();
  presenceSensorSetup();
  tiltSensorSetup();
  Serial.begin(9600);
  WIFI1.begin(9600);
  setupWifi();

}

void loop()
{
  
  
  String data = gatherData();
  //We will only publish data if something changes
  Serial.println(data);
  if (!lastData.equals(data)) {
    lastData = data;
    httppost(data);
  }

  //delay(3000);
}




