#include <clock.h>

bool century = false;
bool h12Flag = false;
bool pmFlag = false;
bool alarmDayIsDay = false;
byte openAlarmEveryDay = 0b00001000;
byte closeAlarmEveryDay = 0b01000000;

float getTemp(DS3231 rtc){
  return  rtc.getTemperature();
}

int getYear(DS3231 rtc){
    return rtc.getYear();
}

int getMonth(DS3231 rtc){
    return rtc.getMonth(century);
}

int getDay(DS3231 rtc){
  return rtc.getDate();
}

int getHour(DS3231 rtc){
    return rtc.getHour(h12Flag, pmFlag);
}

int getMinute(DS3231 rtc){
    return rtc.getMinute();
}

void setYear(DS3231 rtc, int year){
    rtc.setYear(year);
}

void setMonth(DS3231 rtc, int month){
    rtc.setMonth(month);
}

void setDay(DS3231 rtc, int day){
  rtc.setDate(day);
}

void setHour(DS3231 rtc, int hour){
    rtc.setHour(hour);
}

void setMinute(DS3231 rtc, int minute){
    rtc.setMinute(minute);
    rtc.setSecond(0);
}

int getOpenAlarmHour(){
  int eepromEntry = EEPROM.read(open_hour_eeprom_address);
  if(eepromEntry == 255){
    return open_hour;
  }
  return eepromEntry;
}

int getOpenAlarmMinute(){
  int eepromEntry = EEPROM.read(open_minute_eeprom_address);
  if(eepromEntry == 255){
    return open_minute;
  }
  return eepromEntry;
}

void setOpenAlarm(DS3231 rtc, int hour, int minute){

  rtc.turnOffAlarm(1);

  rtc.setA1Time((byte) 0, hour , minute, (byte) 0, openAlarmEveryDay,  alarmDayIsDay, h12Flag, pmFlag);
  rtc.turnOnAlarm(1);
  rtc.checkIfAlarm(1);
  EEPROM.write(open_hour_eeprom_address, hour);
  EEPROM.write(open_minute_eeprom_address, minute);
  Serial.print("Open alarm set for : ");
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);
}


int getCloseAlarmHour(){
  int eepromEntry = EEPROM.read(close_hour_eeprom_address);
  if(eepromEntry == 255){
    return close_hour;
  }
  return eepromEntry;
}

int getCloseAlarmMinute(){
  int eepromEntry = EEPROM.read(close_minute_eeprom_address);
  if(eepromEntry == 255){
    return close_minute;
  }
  return eepromEntry;
}


void setCloseAlarm(DS3231 rtc, int hour, int minute){

  rtc.turnOffAlarm(2);

  rtc.setA2Time((byte) 0, hour , minute , closeAlarmEveryDay,  alarmDayIsDay, h12Flag, pmFlag);
  rtc.turnOnAlarm(2);
  rtc.checkIfAlarm(2);
  EEPROM.write(close_hour_eeprom_address, hour);
  EEPROM.write(close_minute_eeprom_address, minute);
  Serial.print("Close alarm set for : ");
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);
}