/*
Author: Louis Thiery (me@louisthiery.com)

v1.1 (April 23, 2012)

COPYRIGHT (C) 2012 Louis Thiery. All rights reserved.
This work is released under Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
For details, visit: http://creativecommons.org/licenses/by-sa/3.0/

LIABILITY
This program is distributed for educational purposes only and is no way suitable for any particular application, especially commercial. There is no implied suitability so use at your own risk!

DESCRIPTION
This library was written for a Motorola C168i but should carry over to other GSM devices. See example sketch to see how to use the functions. 
To learn how to connect your Arduino to a C168i please see http://louisthiery.com/gsmserial-a-cell-phone-for-your-arduino/
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "GSMSerial.h"

#define C168i_BAUD 4800
#define DELAY 100 //these compiler variable and the next are very fickle
#define DELAY_PER_CHAR 1 //I have yet to find a way to optimize them
#define TXT_SEND_DELAY 9000

uint8_t attempts = 0;

GSMSerial::GSMSerial(int RX,int TX): _myPhone(RX,TX){
  char buffer[BUFFSIZE];
  int _RX = RX;
  int _TX = TX;
  char origin[LENGTH_OF_NUM+1]; //+1 for terminal bit
  char content[LENGTH_OF_CONTENT+1];
}

void GSMSerial::start(){
  pinMode(_rx,INPUT);
  pinMode(_tx,OUTPUT);
  _myPhone.begin(C168i_BAUD);
  _myPhone.println("AT+CMGF=1");
  delay(DELAY);
  _myPhone.println("AT+CPMS=\"ME\",\"ME\",\"ME\"");
  delay(DELAY);
  _myPhone.flush();
}

//returns true if phone is on charger
bool GSMSerial::isCharging(){
  _myPhone.println("AT");
  delay(DELAY);
  _myPhone.flush();
  _myPhone.println("AT+CBC");
  _readPhone();
  if(buffer[15]=='0') return false;
  else return true; 
}

uint8_t GSMSerial::battery(){
  _myPhone.println("AT+CBC");
  _readPhone();
  const uint8_t i=19;
  //know that if form of 1x0 then must be 100
  if(buffer[i]=='1' && buffer[i+2]=='0') return 100;
  //otherwise its a two digit number, so parse it from char
  else {
    //if (_retInt(i)>100) return battery(); //integrity check
    return _retInt(i);
  }
}

void GSMSerial::reset(){
  _myPhone.println("AT*SWRESET");
}

uint8_t GSMSerial::signal(){
  _myPhone.println("AT+CSQ");
  _readPhone();
  uint8_t i=17;
  //if(_retInt(i)>100) return signal(); //integrity check
  return _retInt(i);
}

void GSMSerial::sendTxt(const char num[], const char msg[])
{
  openTxt(num);
  inTxt(msg);
  closeTxt();
}

void GSMSerial::openTxt(const char num[])
{
  _txtMode();
  _myPhone.print("AT+CMGS=\"+");
  _myPhone.print(num);
  _myPhone.println("\"");
  delay(DELAY);
  _myPhone.flush();
}
void GSMSerial::inTxt(const char msg[]){
  _myPhone.print(msg);
}

void GSMSerial::inTxt(double d){
  _myPhone.print(d);
}

void GSMSerial::inTxt(int i){
  _myPhone.print(char(i+48));
}

void GSMSerial::closeTxt(){
  _myPhone.write(26);
  delay(DELAY);
  _myPhone.flush();
  delay(TXT_SEND_DELAY);//keep you from talking to cell again too soon
}

//deletes ALL text from phone memory, sent & received
void GSMSerial::deleteTxts(){
  while(hasTxt()){
    _myPhone.println("AT+CMGD=1");
    delay(DELAY);
    _myPhone.flush();
  }
}

//checks to see if there are ANY texts on the phone, sent or received
bool GSMSerial::hasTxt(){
  _txtMode();
  _myPhone.println("AT+CMGL=\"ALL\"");
  _readPhone();
  if(buffer[16]=='O'){ 
    buffer[0]='\0';
    return false;
  }
  //if you do, bring origin and content over to buffer
  _myPhone.flush();
  _txtMode();
  _myPhone.println("AT+CMGL=\"ALL\"");
  _readPhone();
  int k=0;
  for(int i=38;i<38+LENGTH_OF_NUM;i++) origin[k++]=buffer[i];
  origin[k]='\0';
  int j=0; k=0;
  for(int i=0;i<BUFFSIZE; i++){
    if(buffer[i]=='\n') j++; //keep count of lines
    if(j==2 && k<LENGTH_OF_CONTENT && buffer[i]!='\n') content[k++]=buffer[i];
  }
  content[k]='\0';
  return true;
}

//takes all phone's TX and buffers it for processing
void GSMSerial::_readPhone(){
  delay(DELAY);
  uint8_t pointer=0;
  while(_myPhone.available() && pointer<BUFFSIZE){
    delay(DELAY_PER_CHAR);
    char current = _myPhone.read();
    if(current!= '-1') buffer[pointer++]=current;
    if(current='K' && buffer[pointer-2]=='O'){
      buffer[pointer]='\0';
      return;
    }
  }
}

void GSMSerial::_txtMode(){
  _myPhone.println("AT");
  delay(DELAY);
  _myPhone.println("AT+CMGF=1");
  delay(DELAY);
  _myPhone.flush();
}

uint8_t GSMSerial::_retInt(uint8_t pointer){
   return (buffer[pointer]-48)*10+buffer[pointer+1]-48;//for some this is cheaper than using atoi
}

