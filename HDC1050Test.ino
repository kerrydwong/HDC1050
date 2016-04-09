/*
 * HDC1050 Temperature/Humidity Sensor Library
 * Example
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 10/2015
 */

#include <Wire.h>
#include <HDC1050.h>

HDC1050 hdcSensor;

float tc, tf, h;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  Serial.println(hdcSensor.getManufacturerID(), HEX);
  Serial.println(hdcSensor.getDeviceID(), HEX);
  Serial.println(hdcSensor.getSerialID());
  Serial.println(hdcSensor.batteryOK());

  hdcSensor.turnOnHeater(true);
  hdcSensor.setTemperatureRes(HDC1050::T_RES_14);
  hdcSensor.setHumidityRes(HDC1050::H_RES_8);
  hdcSensor.updateConfigRegister();
}

void loop() {
  tf = hdcSensor.getTemperatureHumidity(tc, h);

  Serial.print(tc);
  Serial.print("  ");
  Serial.print(tf);
  Serial.print("  ");
  Serial.println(h);
}
