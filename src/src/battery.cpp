#include <battery.h>

const float referenceVoltage = 5.45;
const float division = 1023.0; 
const float mvc = referenceVoltage / division * 1000;
const int numberOfSamples = 20;
const int delayBetweenSamples = 10;
const float maxBatVoltage = 4.2;
const float minBatVoltage = 3.0;
const float range = maxBatVoltage - minBatVoltage;


int getBatteryPercent(){
  float voltage = getVoltage();
  float rangeVolts = voltage - minBatVoltage;
  int percent = (rangeVolts / range) * 100;
  
  if(percent > 100){
    percent = 100;
  }
  if(percent < 0) {
    percent = 0;
  }

  return percent;
}

float getVoltage(){
  float medianVoltage = 0;
  for (int i = 0; i < numberOfSamples; i++)
  {
    float countTmp = analogRead(battery_sense_pin);
    medianVoltage += countTmp * mvc;
  }
  medianVoltage = medianVoltage / numberOfSamples / 1000;
  Serial.print("battery voltage: ");
  Serial.println(medianVoltage);
  return medianVoltage;
}