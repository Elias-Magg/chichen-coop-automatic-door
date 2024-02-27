#ifndef SCREEN_FUNCTIONS_H
#define SCREEN_FUNCTIONS_H
#include <Arduino.h>
#include <configuration.h>
#include <Servo.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <clock.h>
#include <motor.h>
#include <battery.h>

void lcdPowerOn(LiquidCrystal_I2C lcd);

void lcdPowerOff(LiquidCrystal_I2C lcd);

void printLowBatteryWarning(LiquidCrystal_I2C lcd);

void printHomeScreen(LiquidCrystal_I2C lcd, DS3231 rtc);

void displayVariableToBeChanged(LiquidCrystal_I2C lcd, const char *prompt, int variableToBeChanged);

int promtVariableChange(LiquidCrystal_I2C lcd, const char *prompt, int variableToBeChanged, int variableMin, int variableMax);

void lcdPrintArray(LiquidCrystal_I2C lcd, char entries[][20], int positionSelected);

void printOptionsScreen(LiquidCrystal_I2C lcd, DS3231 rtc, Servo mainServo, Servo lockServo);

void flashScreen(LiquidCrystal_I2C lcd);

#endif