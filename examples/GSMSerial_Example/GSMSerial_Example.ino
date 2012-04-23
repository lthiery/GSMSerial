
/*
Author: Louis Thiery (me@louisthiery.com)

v1.0 (April 11, 2012)

COPYRIGHT (C) 2012 Louis Thiery. All rights reserved.
This work is released under Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
For details, visit: http://creativecommons.org/licenses/by-sa/3.0/

LIABILITY
This program is distributed for educational purposes only and 
is no way suitable for any particular application, especially commercial. 
There is no implied suitability so use at your own risk!

If you need help wiring a C168i to an Arduino, please visit my site:
http://louisthiery.com/gsmserial-a-cell-phone-for-your-arduino/
*/

#include <SoftwareSerial.h>
#include <GSMSerial.h>
//for numbers that are longer than 11 digits,
//change compiler variable in library!
#define CONTACT "16173334444"

GSMSerial phone(5,6); //(RX) 5 red,(TX) 6 yellow

void setup(){
  Serial.begin(9600);
  phone.start();
  //easy method to send message
  phone.sendTxt(CONTACT, "Hello, world");
  //in case you want to do something more complicated
  phone.openTxt(CONTACT);
  phone.inTxt("You like pi? ");
  double pi= 3.14;
  phone.inTxt(pi);
  phone.closeTxt();
}

void loop(){
  //make sure you run hasTxt before looking at
  //the class variables origin & content
  if(phone.hasTxt()){
    Serial.println("Text message received");
    Serial.print("   origin: ");
    Serial.println(phone.origin);
    Serial.print("   content: ");
    Serial.println(phone.content);
  }
  Serial.println();
  if(phone.isCharging()){
    Serial.println("The phone is charging");
  }
  else{
    Serial.print("Battery level is: ");
    Serial.println(phone.battery(),DEC);
  }
  Serial.println();
  Serial.print("Signal strength is: ");
  Serial.println(phone.signal());
  Serial.println();
  delay(1000);
}

