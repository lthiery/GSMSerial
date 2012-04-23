/*
Author: Louis Thiery (me@louisthiery.com)

v1.0 (April 11, 2012)

COPYRIGHT (C) 2012 Louis Thiery. All rights reserved.
This work is released under Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
For details, visit: http://creativecommons.org/licenses/by-sa/3.0/

LIABILITY
This program is distributed for educational purposes only and is no way suitable for any particular application, especially commercial. There is no implied suitability so use at your own risk!

DESCRIPTION
This library was written for a Motorola C168i but should carry over to other GSM devices. See example sketch to see how to use the functions. 
To learn how to connect your Arduino to a C168i please see http://louisthiery.com/gsmserial-a-cell-phone-for-your-arduino/
*/

#ifndef GSMSerial_h
#define GSMSerial_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#define BUFFSIZE 100
#define LENGTH_OF_NUM 11
#define LENGTH_OF_CONTENT 40

class GSMSerial{
 public:
  GSMSerial(int RX,int TX);
  char buffer[BUFFSIZE];
  char origin[LENGTH_OF_NUM+1];
  char content[LENGTH_OF_CONTENT+1];
  void start();
  bool isCharging();
  void sendTxt(const char num[], const char msg[]);
  void openTxt(const char num[]);
  void inTxt(const char msg[]);
  void inTxt(double d);
  void inTxt(int i);
  void closeTxt();
  void deleteTxts();
  bool hasTxt();
  uint8_t battery();
  void reset();
  uint8_t signal();
 private:
  SoftwareSerial _myPhone; //this should be pvt but I like having access
  int _rx;
  int _tx;
  void _txtMode();
  void _readPhone();
  uint8_t _retInt(uint8_t pointer);
};
#endif
