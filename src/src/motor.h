#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H
#include <Arduino.h>
#include <configuration.h>
#include <EEPROM.h>
#include <Servo.h>

int getMainServoOpenSecondsOfRotation();

void setMainServoOpenSecondsOfRotation(int seconds);

int getMainServoCloseSecondsOfRotation();

void setMainServoCloseSecondsOfRotation(int seconds);

int getLockServoMinAngle();

void setLockServoMinAngle(int angle);

int getLockServoMaxAngle();

void setLockServoMaxAngle(int angle);

int getMainServoSecondsOfLiftOff();

int getLockServoSecondsOfActuation();

int getMainServoOpenSpeed();

void setMainServoOpenSpeed(int speed);

int getMainServoCloseSpeed();

void setMainServoCloseSpeed(int speed);

void lock(Servo lockServo, int secondsOfLockActuation);


void unlock(Servo lockServo, int secondsOfLockActuation);


void open(Servo mainServo, Servo lockServo, int secondsOfRotation, int secondsOfLiftOff, int secondsOfLockActuation);


void close(Servo mainServo, Servo lockServo, int secondsOfRotation, int secondsOfLiftOff, int secondsOfLockActuation);

bool isDoorOpen();

void setDoorOpen(bool open);

#endif