/*

  https://www.instructables.com/id/Arduino-Esp8266-Post-Data-to-Website/

  GET
  https://raistlinmolina.000webhostapp.com/espget.php?parked=YES&presence=YES&sound=NO&tilt=NO
  POST
  https://raistlinmolina.000webhostapp.com/esppost.php?parked=YES&presence=YES&sound=NO&tilt=NO
  Results on
  https://raistlinmolina.000webhostapp.com/sensor.html
*/


/*Interesting AT commands
   AT
   AT+CWMODE?
   AT+CWMODE=1  Sets client mode
   AT+CWLAP   Makes a wifi scan and shows APs
   AT+CWJAP="ssid","passwd"  connect to ssid AP with password passwd
   AT+CIFSR  show IP
   AT+CIPMUX=1    Accept multiple connections
   AT+CIPSERVER=1,80 Start a web server in port 80
*/

#include <SoftwareSerial.h>
//Only used inside the post method, move them there when definitive values are set
const String server = "raistlinmolina.000webhostapp.com";
//const String server = "192.168.1.1";
const String uri = "esppost.php";

SoftwareSerial WIFI1(3, 2); // RX | TX (Tx | Rx in the ESP respectively)

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
    "AT+CWJAP=\"Valhalla\",\"xxxxxxxx\"",
    "AT+CIFSR" ,
    //Not opening a server thsi time "AT+CIPMUX=1",
    //Not opening a server thsi time "AT+CIPSERVER=1,80",
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
  String parked = "YES";
  String presence = "NO";
  String sound = "NO";
  String tilt = "NO";
  
  data = "parked=" + parked + "&presence=" + presence + "&sound=" + sound + "&tilt=" + tilt;

  return data;
}

void httppost (String data)
{

  WIFI1.println("AT+CIPSTART=0,\"TCP\",\"" + server + "\",80");//start a TCP connection.
  Serial.println("AT+CIPSTART=0,\"TCP\",\"" + server + "\",80");
  delay(1000);
  if ( WIFI1.find("OK"))
  {
    Serial.println("TCP connection ready");
  }

  String postRequest =
    "POST " + uri + " HTTP/1.0\r\n" +
    "Host: " + server + "\r\n" +
    "Accept: *" + "/" + "*\r\n" +
    "Content-Length: " + (data.length()-1) + "\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "\r\n" + data;
  //Serial.println(postRequest);
  String sendCmd = "AT+CIPSEND=0,";//determine the number of caracters to be sent.
  delay(500);
  WIFI1.print(sendCmd);
  WIFI1.println(postRequest.length() );
  delay(1000);
  if (WIFI1.find(">"))
  {
    Serial.println("Sending.."); WIFI1.print(postRequest);
    if ( WIFI1.find("SEND OK"))
    {
      Serial.println("Packet sent");
      while (WIFI1.available())
      {
        String tmpResp = WIFI1.readString();
        Serial.println(tmpResp);
      }
      // close the connection
      WIFI1.println("AT+CIPCLOSE");
    }
  }else{
    Serial.println("No response");
  }
}

/*
 * Not used here, commented out to save memory
voit atTerminal(){
  String B = "." ;
  if (WIFI1.available())
  { char c = WIFI1.read() ;
    Serial.print(c);
  }
  if (Serial.available()) {
    char c = Serial.read();
    WIFI1.print(c);
  }
}
*/

void setup()
{
  Serial.begin(9600);
  WIFI1.begin(9600);
  setupWifi();
}

void loop()
{
  String data = gatherData();
  httppost(data);
  delay(60000);
  
}




