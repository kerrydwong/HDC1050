/*
 * HDC1050 Temperature/Humidity Sensor Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 10/2015
 */

#include "HDC1050.h"
#include <Wire.h>

HDC1050::HDC1050()
{
	configReg = 0x10; //POR default
}

bool HDC1050::isConnected()
{
	byte ret;

	Wire.beginTransmission(Addr);
	ret = Wire.endTransmission();
	if ((ret == 0) && (getManufacturerID() == 0x5449))
		return true;
	return false;
}

bool HDC1050::readRegister(byte regAddr, byte numOfBytes, int retryCount = 200)
{
	/* Input:
	*     regAddr: I2C internal register address to read from 
	*     numOfBytes: number of bytes to read from register
	*     retryCount: retryCount times for I2C connection to be up & running
	* Output:
	*     return 'true' if read was with success
	*            'false' if cannot read data from I2C register
	* Remarks:
	*     read information to HDC1050 variable 'buf'
	*/

	Wire.beginTransmission(Addr);
	Wire.write(regAddr);
	Wire.endTransmission();

	if (regAddr == REG_Temperature || regAddr == REG_Humidity) {
		delay(50);
	}

	Wire.requestFrom(Addr, numOfBytes);

	while (!Wire.available() && retryCount > 0) {
	    delay(5);
	    retryCount--;
	}
	if (retryCount == 0)
	    return false;

	for (int i=0; i < numOfBytes; i++) {
		buf[i] = Wire.read();
	}
	return true;
}

unsigned int HDC1050::getManufacturerID()
{
	if (readRegister(REG_ManufactureID, 2)) 
		return buf[0] << 8 | buf[1];
	else
		return 0;
}

unsigned int HDC1050::getDeviceID()
{
	if (readRegister(REG_DeviceID, 2))
		return buf[0] << 8 | buf[1];
	else
		return 0;
}

String HDC1050::getSerialID()
{
	bool success = true;

	success = success && readRegister(REG_SER_1, 2);
	String s1 = String(buf[0] << 8 | buf[1], HEX);
	success = success && readRegister(REG_SER_2, 2);
	String s2 = String(buf[0] << 8 | buf[1], HEX);
	success = success && readRegister(REG_SER_3, 2);
	String s3 = String(buf[0] << 8 | buf[1], HEX);

	if (success)
		return String(s1 + s2 + s3);
	else
		return String("");
}

void HDC1050::updateConfigRegister()
{
	Wire.beginTransmission(Addr);
	Wire.write(REG_Config);
	Wire.write(configReg);
	Wire.write(byte(0x00));
	Wire.endTransmission();
}

void HDC1050::setTemperatureRes(byte res) 
{
	if (res > 1) res = T_RES_14;
	
	configReg &= ~(1 <<  BIT_T_RES);
	configReg |= res << BIT_T_RES;	
}

void HDC1050::setHumidityRes(byte res) 
{
	if (res > 2) res = H_RES_14;
	
	configReg &= ~(0x3 << BIT_H_RES);
	configReg |= res << BIT_H_RES;
}

void HDC1050::turnOnHeater(bool heaterOn) 
{
	if (heaterOn) 
		configReg |= 1 << BIT_HEATER;
	else
		configReg &= ~(1 << BIT_HEATER);
}

bool HDC1050::batteryOK()
{
	readRegister(REG_Config, 2);
	configReg = buf[0];

	return (configReg & (1 << BIT_BATTERY_OK)) == 0;
}

float HDC1050::getTemperatureHumidity(float &t, float &h)
{
	readRegister(REG_Temperature, 4);
	unsigned int th, tl, hh, hl;

	th = buf[0];
	tl = buf[1];
	hh = buf[2];
	hl = buf[3];

	t = (th << 8 | tl) * 165.0 / 65536.0 - 40.0;
	h = (hh << 8 | hl) * 100.0 / 65536.0;	
	
	return 1.8 * t + 32.0;
}

