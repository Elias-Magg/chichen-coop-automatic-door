#include <motor.h>

int getMainServoOpenSecondsOfRotation(){
  int eppromEntry = EEPROM.read(main_servo_open_seconds_of_rotation_eeprom_address);
  if(eppromEntry == 255){
    return main_servo_open_seconds_of_rotation;
  }
  return eppromEntry;
}

void setMainServoOpenSecondsOfRotation(int seconds){
  EEPROM.write(main_servo_open_seconds_of_rotation_eeprom_address, seconds);
}

int getMainServoCloseSecondsOfRotation(){
  int eppromEntry = EEPROM.read(main_servo_close_seconds_of_rotation_eeprom_address);
  if(eppromEntry == 255){
    return main_servo_close_seconds_of_rotation;
  }
  return eppromEntry;
}

void setMainServoCloseSecondsOfRotation(int seconds){
  EEPROM.write(main_servo_close_seconds_of_rotation_eeprom_address, seconds);
}


void lock(Servo lockServo, int secondsOfLockActuation){
  Serial.println("locking");

  lockServo.write(getLockServoMaxAngle());

  lockServo.attach(lock_servo_pin);

  delay(secondsOfLockActuation * 1000);

  delay(50);

  lockServo.detach();

  lockServo.write(getLockServoMaxAngle());

}


void unlock(Servo lockServo, int secondsOfLockActuation){
  Serial.println("unlocking");

  lockServo.write(getLockServoMinAngle());

  lockServo.attach(lock_servo_pin);

  delay(secondsOfLockActuation * 1000);

  delay(50);

  lockServo.detach();

  lockServo.write(getLockServoMinAngle());

}


void open(Servo mainServo, Servo lockServo, int secondsOfRotation, int secondsOfLiftOff, int secondsOfLockActuation){

  if(!isDoorOpen()){

    Serial.print("Opening door. Seconds: ");

    Serial.println(secondsOfRotation);

    unlock(lockServo, secondsOfLockActuation);

    mainServo.attach(main_servo_pin);

    mainServo.write(90 - getMainServoOpenSpeed());

    for(int i=0; i < secondsOfRotation + secondsOfLiftOff; i++){
      delay(1000);
    }

    mainServo.write(main_servo_range_stop);

    mainServo.detach();

    delay(50);

    lock(lockServo, secondsOfLockActuation);

    setDoorOpen(true);
  } else {
    Serial.println("Door already open");
  }
}


void close(Servo mainServo, Servo lockServo, int secondsOfRotation, int secondsOfLiftOff, int secondsOfLockActuation){
  
  if(isDoorOpen()){
    Serial.print("Closing door. Seconds: ");
    Serial.println(secondsOfRotation);

    Serial.println("Lifting door");

    mainServo.attach(main_servo_pin);

    mainServo.write(90 - getMainServoOpenSpeed());

    for(int i=0; i < secondsOfLiftOff; i++){
      delay(1000);
    }

    mainServo.write(main_servo_range_stop);

    delay(50);

    mainServo.detach();

    delay(50);

    unlock(lockServo, secondsOfLockActuation);

    mainServo.attach(main_servo_pin);

    mainServo.write(90 + getMainServoCloseSpeed());

    for(int i=0; i < secondsOfRotation + secondsOfLiftOff; i++){
      delay(1000);
    }

    mainServo.write(main_servo_range_stop);

    delay(50);

    mainServo.detach();

    delay(50);

    // lock(lockServo, secondsOfLockActuation);

    setDoorOpen(false);
  } else {
    Serial.println("Door already closed");
  }
}

bool isDoorOpen(){
    int eppromEntry = EEPROM.read(door_state_eeprom_address);
    if(eppromEntry == 255){
        return false;
    }
    return eppromEntry;
}

void setDoorOpen(bool open){
    EEPROM.write(door_state_eeprom_address, open);
}


int getLockServoMinAngle(){
  int eppromEntry = EEPROM.read(lock_servo_range_min_eeprom_address);
    if(eppromEntry == 255){
        return lock_servo_range_min;
    }
    return eppromEntry;
}

void setLockServoMinAngle(int angle){
  EEPROM.write(lock_servo_range_min_eeprom_address, angle);
}

int getLockServoMaxAngle(){
  int eppromEntry = EEPROM.read(lock_servo_range_max_eeprom_address);
    if(eppromEntry == 255){
        return lock_servo_range_max;
    }
    return eppromEntry;
}

void setLockServoMaxAngle(int angle){
  EEPROM.write(lock_servo_range_max_eeprom_address, angle);
}

int getMainServoSecondsOfLiftOff(){
  return main_servo_seconds_of_lift_off;
}

int getLockServoSecondsOfActuation(){
  return lock_servo_seconds_of_actuation;
}

int getMainServoOpenSpeed(){
  int eppromEntry = EEPROM.read(main_servo_open_speed_eeprom_address);
    if(eppromEntry == 255){
        return main_servo_open_speed;
    }
    return eppromEntry;
}

void setMainServoOpenSpeed(int speed){
  EEPROM.write(main_servo_open_speed_eeprom_address, speed);
}

int getMainServoCloseSpeed(){
  int eppromEntry = EEPROM.read(main_servo_close_speed_eeprom_address);
    if(eppromEntry == 255){
        return main_servo_close_speed;
    }
    return eppromEntry;
}

void setMainServoCloseSpeed(int speed){
  EEPROM.write(main_servo_close_speed_eeprom_address, speed);
}