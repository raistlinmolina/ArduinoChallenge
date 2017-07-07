#include <SoftwareSerial.h>
SoftwareSerial BT1(3, 2); // RX | TX

void setup()
  {  Serial.begin(9600);
     BT1.begin(9600);
  }

void loop()
  {  String B= "." ;
     if (BT1.available())
         { char c = BT1.read() ;
           Serial.print(c);
         }
     if (Serial.available())
         {  char c = Serial.read();
            BT1.print(c);
         }
   }

/*Interesting AT commands
 * AT
 * AT+CWMODE?
 * AT+CWMODE=1  Sets client mode
 * AT+CWLAP   Makes a wifi scan and shows APs
 * AT+CWJAP="ssid","passwd"  connect to ssid AP with password passwd
 * AT+CIFSR  show IP
 * AT+CIPMUX=1    Accept multiple connections
 * AT+CIPSERVER=1,80 Start a web server in port 80
 */
