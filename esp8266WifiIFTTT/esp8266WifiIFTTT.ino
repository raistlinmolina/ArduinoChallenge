/*Está sin terminar, mirar, lo de wifi101.h"

#include <SoftwareSerial.h>
#include <SPI.h>
#include <WiFi101.h>
 
// Definimos los parámetros para el IFTTT
#define HOSTIFTTT "maker.ifttt.com"
#define EVENTO "nombre_tu_evento"
#define IFTTTKEY "tu_clave_ifttt"

SoftwareSerial WIFI1(3, 2); // RX | TX

String commands[]=
  {  "AT+CWMODE=1",
     "AT+CWQAP",
     "AT+CWJAP=\"Valhalla\",\"xxxxxxxxx\"",
     "AT+CIFSR" ,
     "AT+CIPMUX=1",
     "AT+CIPSERVER=1,80",
     "END"                 
  };


String GetLineWIFI()
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
    return( S ) ;
  }
}

void setupWifi(){
  int index=0;
 while(commands[index] != "END")
  {  WIFI1.println(commands[index++]);
     while ( true)
        {   String s = GetLineWIFI();
            if ( s!= "") Serial.println(s);
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
void setup()
  {  Serial.begin(9600);
     WIFI1.begin(9600);
     setupWifi();
  }

void loop()
  {  String B= "." ;
     if (WIFI1.available())
         { char c = WIFI1.read() ;
           Serial.print(c);
         }
     if (Serial.available())
         {  char c = Serial.read();
            WIFI1.print(c);
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
