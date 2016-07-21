#ifndef HDC1050H
#define HDC1050H

/*
 * HDC1050 Temperature/Humidity Sensor Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 10/2015
 */
#include <Arduino.h>

class HDC1050 {
public:
	static const byte Addr = 0x40;

	static const byte REG_Temperature = 0x00;
	static const byte REG_Humidity = 0x01;
	static const byte REG_Config = 0x02;
	static const byte REG_SER_1 = 0xfb;
	static const byte REG_SER_2 = 0xfc;
	static const byte REG_SER_3 = 0xfd;
	static const byte REG_ManufactureID = 0xfe; //0x5449
	static const byte REG_DeviceID = 0xff; //0x1050

	static const byte BIT_T_RES = 2; 
	static const byte BIT_H_RES = 0;
	static const byte BIT_BATTERY_OK = 3;
	static const byte BIT_ACQ_MODE = 4;
	static const byte BIT_HEATER = 5;
	static const byte BIT_RST = 7;
	
	static const byte T_RES_14 = 0;
	static const byte T_RES_11 = 1;

	static const byte H_RES_14 = 0;
	static const byte H_RES_11 = 1;
	static const byte H_RES_8 = 2;
	
	HDC1050();
	bool isConnected();
	unsigned int getManufacturerID();
	unsigned int getDeviceID();
	String getSerialID();
	void setTemperatureRes(byte res);
	void setHumidityRes(byte res);
	void turnOnHeater(bool heaterOn);
	bool batteryOK();
	float getTemperatureHumidity(float &t, float &h);
	void updateConfigRegister();
	void readRegister(byte regAddr, byte numOfBytes);

	byte configReg; //higher 8 bits of the configuration register
	byte buf[4];		
private:
};
#endif

