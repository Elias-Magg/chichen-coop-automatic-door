#ifndef CLOCK_FUNCTIONC_H
#define CLOCK_FUNCTIONC_H
#include <DS3231.h>
#include <Arduino.h>
#include <configuration.h>
#include <EEPROM.h>

float getTemp(DS3231 rtc);

int getYear(DS3231 rtc);

int getMonth(DS3231 rtc);

int getDay(DS3231 rtc);

int getHour(DS3231 rtc);

int getMinute(DS3231 rtc);

void setYear(DS3231 rtc, int year);

void setMonth(DS3231 rtc, int month);

void setDay(DS3231 rtc, int day);

void setHour(DS3231 rtc, int hour);

void setMinute(DS3231 rtc, int minute);

int getOpenAlarmHour();

int getOpenAlarmMinute();

void setOpenAlarm(DS3231 rtc, int hour, int minute);

int getCloseAlarmHour();

int getCloseAlarmMinute();

void setCloseAlarm(DS3231 rtc, int hour, int minute);

#endif