#include <screen.h>

char options[][20] = {
    "Set clock",
    "Set open time",
    "Set close time",
    "Set open seconds",
    "Set open speed",
    "Set close seconds",
    "Set close speed",
    "Set lock min angle",
    "Set lock max angle",
    "Set door state",
    "Test drive",
    "Cancel"
  };

void printInTwoDigits(LiquidCrystal_I2C lcd, int number){
  if(number < 10){
    lcd.print("0");
  }
  lcd.print(number, DEC);
}

void lcdPowerOn(LiquidCrystal_I2C lcd){
  lcd.backlight();
  lcd.clear();
  lcd.home();
}

void lcdPowerOff(LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.home();
  lcd.noBacklight();
}
void printLowBatteryWarning(LiquidCrystal_I2C lcd){
  if(getBatteryPercent() < battery_percentage_alarm_threshold){
    for(int i = 0; i < 3; i++){
      lcd.clear();
      delay(500);
      lcd.setCursor(0,1);
      lcd.print("    LOW BATTERY");
      delay(500);
    }
  }
}

void printHomeScreen(LiquidCrystal_I2C lcd, DS3231 rtc){
  lcd.clear();
  lcd.home();

  lcd.print("Time: ");
  printInTwoDigits(lcd, getHour(rtc));
  lcd.print(":");
  printInTwoDigits(lcd, getMinute(rtc));

  lcd.setCursor(0,1);
  lcd.print("Bat: ");
  lcd.print(getBatteryPercent());
  lcd.print("%");
  lcd.print("      ");
  lcd.print(getVoltage());
  lcd.print("V");


  lcd.setCursor(0,2);
  lcd.print("Open time: ");
  printInTwoDigits(lcd, getOpenAlarmHour());
  lcd.print(":");
  printInTwoDigits(lcd, getOpenAlarmMinute());

  lcd.setCursor(0,3);
  lcd.print("Close time: ");
  printInTwoDigits(lcd, getCloseAlarmHour());
  lcd.print(":");
  printInTwoDigits(lcd, getCloseAlarmMinute());
}


void displayVariableToBeChanged(LiquidCrystal_I2C lcd, const char *prompt, int variableToBeChanged){
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,1);
  lcd.print(prompt);
  lcd.setCursor(10,2);
  lcd.print(variableToBeChanged);
}


int promtVariableChange(LiquidCrystal_I2C lcd, const char *prompt, int variableToBeChanged, int variableMin, int variableMax){
  unsigned long idleTimerBegin = millis();
  displayVariableToBeChanged(lcd, prompt, variableToBeChanged);
  bool exit = false;
  while(!exit){
    if(digitalRead(keypad_button_down_pin) == LOW){
      idleTimerBegin = millis();
      variableToBeChanged--;
      if(variableToBeChanged < variableMin){
        variableToBeChanged = variableMin;
      }
      displayVariableToBeChanged(lcd, prompt, variableToBeChanged);
      Serial.print("variable change: ");
      Serial.println(variableToBeChanged);
      delay(keypad_cooldown);
    }

    if(digitalRead(keypad_button_up_pin) == LOW){
      idleTimerBegin = millis();
      variableToBeChanged++;
      if(variableToBeChanged > variableMax){
        variableToBeChanged = variableMax;
      }
      displayVariableToBeChanged(lcd, prompt, variableToBeChanged);
      Serial.print("variable change: ");
      Serial.println(variableToBeChanged);
      delay(keypad_cooldown);
    }

    if(digitalRead(keypad_button_select_pin) == LOW){
      idleTimerBegin = millis();
      while(digitalRead(keypad_button_select_pin) == LOW){}
      Serial.print("variable commited: ");
      Serial.println(variableToBeChanged);
      exit = true;
    }
    unsigned long idleTimerEnd = millis();
    unsigned long duration = idleTimerEnd - idleTimerBegin;
    if( duration / 1000 > configuration_screen_idle_seconds){
      exit = true;
    }
  }
  delay(keypad_cooldown);
  return variableToBeChanged;
}

void lcdPrintArray(LiquidCrystal_I2C lcd, char entries[][20], int positionSelected){
  lcd.clear();
  lcd.home();
  for(int i = 0; i < 4; i++){
    lcd.setCursor(1,i);
    int printItem = positionSelected > 3 ? positionSelected + i - 3 : i;
    lcd.print(entries[printItem]);
  }
  lcd.setCursor(0, positionSelected > 3 ? 3 : positionSelected);
  lcd.write((uint8_t)0);
}

void printOptionsScreen(LiquidCrystal_I2C lcd, DS3231 rtc, Servo mainServo, Servo lockServo){
  unsigned long idleTimerBegin = millis();

  int clockHour = getHour(rtc);
  int clockMinute = getMinute(rtc);
  int clockMonth = getMonth(rtc);
  int clockDay = getDay(rtc);
  int clockYear = getYear(rtc);
  int openHour = getOpenAlarmHour();
  int openMinute = getOpenAlarmMinute();
  int closeHour = getCloseAlarmHour();
  int closeMinute = getCloseAlarmMinute();
  int motorOpenTimer = getMainServoOpenSecondsOfRotation();
  int motorCloseTimer = getMainServoCloseSecondsOfRotation();
  int motorOpenSpeed = getMainServoOpenSpeed();
  int motorCloseSpeed = getMainServoCloseSpeed();
  int lockServoMaxAngle = getLockServoMaxAngle();
  int lockServoMinAngle = getLockServoMinAngle();
  int doorState = isDoorOpen();

  int sizeOfOptions = sizeof(options) / sizeof(options[0]);
  int positionSelected = 0;
  bool exitOptions = false;

  lcdPrintArray(lcd, options, positionSelected);
  Serial.print("position selected change: ");
  Serial.println(positionSelected);
  while(!exitOptions){
    if(digitalRead(keypad_button_down_pin) == LOW){
      idleTimerBegin = millis();
      positionSelected++;
      if(positionSelected > sizeOfOptions-1){
        positionSelected = sizeOfOptions-1;
      }
      lcdPrintArray(lcd, options, positionSelected);
      Serial.print("position selected change: ");
      Serial.println(positionSelected);
      delay(keypad_cooldown);
    }

    if(digitalRead(keypad_button_up_pin) == LOW){
      idleTimerBegin = millis();
      positionSelected--;
      if(positionSelected < 0){
        positionSelected = 0;
      }
      lcdPrintArray(lcd, options, positionSelected);
      Serial.print("position selected change: ");
      Serial.println(positionSelected);
      delay(keypad_cooldown);
    }

    if(digitalRead(keypad_button_select_pin) == LOW){
      idleTimerBegin = millis();
      while(digitalRead(keypad_button_select_pin) == LOW){}
      Serial.print("position selected commit: ");
      Serial.println(positionSelected);
      delay(keypad_cooldown);
      switch (positionSelected){
        case 0:
            clockYear = promtVariableChange(lcd, "Set clock year:", clockYear, 0, 99);
            clockMonth = promtVariableChange(lcd, "Set clock month:", clockMonth, 1, 12);
            clockDay = promtVariableChange(lcd, "Set clock day:", clockDay, 1, 31);
            clockHour = promtVariableChange(lcd, "Set clock hour:", clockHour, 0, 23);
            clockMinute = promtVariableChange(lcd, "Set clock minute:", clockMinute, 0, 59);
            setYear(rtc, clockYear);
            setMonth(rtc, clockMonth);
            setDay(rtc, clockDay);
            setHour(rtc, clockHour);
            setMinute(rtc, clockMinute);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 1:
            openHour = promtVariableChange(lcd, "Set open hour:", openHour, 0, 23);
            openMinute = promtVariableChange(lcd, "Set open minute:", openMinute, 0, 59);
            setOpenAlarm(rtc, openHour, openMinute);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 2:
            closeHour = promtVariableChange(lcd, "Set close hour:", closeHour, 0, 23);
            closeMinute = promtVariableChange(lcd, "Set close minute:", closeMinute, 0, 59);
            setCloseAlarm(rtc, closeHour, closeMinute);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 3:
            motorOpenTimer = promtVariableChange(lcd, "Door actuation time:", motorOpenTimer, 0, 254);
            setMainServoOpenSecondsOfRotation(motorOpenTimer);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 4:
            motorOpenSpeed = promtVariableChange(lcd, "Motor open speed:", motorOpenSpeed, 5 , 85);
            setMainServoOpenSpeed(motorOpenSpeed);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 5:
            motorCloseTimer = promtVariableChange(lcd, "Door actuation time:", motorCloseTimer, 0, 254);
            setMainServoCloseSecondsOfRotation(motorCloseTimer);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 6:
            motorCloseSpeed = promtVariableChange(lcd, "Motor close speed:", motorCloseSpeed, 5 , 85);
            setMainServoCloseSpeed(motorCloseSpeed);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 7:
            lockServoMinAngle = promtVariableChange(lcd, "Lock min angle:", lockServoMinAngle, 0 , 180);
            setLockServoMinAngle(lockServoMinAngle);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 8:
            lockServoMaxAngle = promtVariableChange(lcd, "Lock max angle:", lockServoMaxAngle, 0 , 180);
            setLockServoMaxAngle(lockServoMaxAngle);
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 9:
            doorState = promtVariableChange(lcd, "Is door open:", doorState, 0 , 1);
            if(doorState == 1){
              setDoorOpen(true);
            }else{
              setDoorOpen(false);
            }
            lcdPrintArray(lcd, options, positionSelected);
            break;
        case 10:
            if(isDoorOpen()){
                close(mainServo, lockServo, getMainServoCloseSecondsOfRotation(), getMainServoSecondsOfLiftOff(), getLockServoSecondsOfActuation());
                delay(5000);
                open(mainServo, lockServo, getMainServoOpenSecondsOfRotation(), getMainServoSecondsOfLiftOff(), getLockServoSecondsOfActuation());
                delay(5000);
            }else {
                open(mainServo, lockServo, getMainServoOpenSecondsOfRotation(), getMainServoSecondsOfLiftOff(), getLockServoSecondsOfActuation());
                delay(5000);
                close(mainServo, lockServo, getMainServoCloseSecondsOfRotation(), getMainServoSecondsOfLiftOff(), getLockServoSecondsOfActuation());
                delay(5000);
            }
            Serial.println("test complete");
            break;
        case 11:
            exitOptions = true;
            break;
        default:
            break;
      }

    }
    unsigned long idleTimerEnd = millis();
    unsigned long duration = idleTimerEnd - idleTimerBegin;
    if( duration / 1000 > configuration_screen_idle_seconds){
      exitOptions = true;
    }
  }
}

void flashScreen(LiquidCrystal_I2C lcd){
  for(int i = 0; i < 3; i++){
    lcd.noBacklight();
    delay(500);
    lcd.backlight();
    delay(500);

  }
}