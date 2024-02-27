#include <Arduino.h>
#include <Servo.h>
#include <configuration.h>
#include <DS3231.h>
#include <Wire.h>
#include <time.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <stdio.h>
#include <stdlib.h>
#include <clock.h>
#include <screen.h>
#include <motor.h>


LiquidCrystal_I2C licuidCrystal(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte uiPointer[] = {0x00,0x00,0x04,0x02,0x1D,0x02,0x04,0x00};

Servo mainServo;

Servo lockServo;

DS3231 rtc;



void checkOpenAlarm(DS3231 rtc){
  if(rtc.checkIfAlarm(1, false)){
    Serial.println("Open alarm !!!");
    rtc.checkIfAlarm(1, true);
    open(mainServo, lockServo, getMainServoOpenSecondsOfRotation(), getMainServoSecondsOfLiftOff(), getLockServoSecondsOfActuation());
  }
}


void checkCloseAlarm(DS3231 rtc){
  if(rtc.checkIfAlarm(2, false)){
    Serial.println("Close alarm !!!");
    rtc.checkIfAlarm(2, true);
    close(mainServo, lockServo, getMainServoCloseSecondsOfRotation(), getMainServoSecondsOfLiftOff(), getLockServoSecondsOfActuation());
  }
}


void setup() {
  Serial.begin(57600);
  Wire.begin();

  pinMode(keypad_button_down_pin, INPUT_PULLUP);
  pinMode(keypad_button_select_pin, INPUT_PULLUP);
  pinMode(keypad_button_up_pin, INPUT_PULLUP);

  Serial.print("Open Alarm state :  ");
  Serial.println(rtc.checkAlarmEnabled(1));

  Serial.print("Close Alarm state :  ");
  Serial.println(rtc.checkAlarmEnabled(2));

  Serial.print("current time : ");
  Serial.print(getDay(rtc), DEC);
  Serial.print("/");
  Serial.print(getMonth(rtc), DEC);
  Serial.print("/");
  Serial.print(getYear(rtc), DEC);
  Serial.print(" ");
  Serial.print(getHour(rtc), DEC);
  Serial.print(":");
  Serial.println(getMinute(rtc), DEC);

  Serial.print("Is door open : ");
  Serial.println(isDoorOpen());

  licuidCrystal.begin(20, 4);
  licuidCrystal.createChar(0, uiPointer);

  lcdPowerOff(licuidCrystal);

}

void loop() {

  if(digitalRead(keypad_button_select_pin) == LOW){
    lcdPowerOn(licuidCrystal);
    printLowBatteryWarning(licuidCrystal);
    printHomeScreen(licuidCrystal, rtc);

    bool buttonContinuouslyPressed = true;
    int checkTime = (home_screen_on_seconds * 1000) / 10;

    for(int i = 0; i < 10; i++){
      delay(checkTime);
      printHomeScreen(licuidCrystal, rtc);
      if(digitalRead(keypad_button_select_pin) == HIGH){
        buttonContinuouslyPressed = false;
      }
    }

    if(digitalRead(keypad_button_select_pin) == LOW && buttonContinuouslyPressed){
      flashScreen(licuidCrystal);
      while(digitalRead(keypad_button_select_pin) == LOW){}
      Serial.println("Entering options screen");
      printOptionsScreen(licuidCrystal, rtc, mainServo, lockServo);
    }
    lcdPowerOff(licuidCrystal);
  }

  checkCloseAlarm(rtc);
  checkOpenAlarm(rtc);

}


