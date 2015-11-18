/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove DHT Series (11 - 22)
	ROLE : 	Le Petit Cool Temperature and Humidity Sensor
			Le Cool Stick Temperature and Humidity Sensor
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_DHT_h
#define Cool_DHT_h

	#include "Arduino.h"
	#include "PetitCool.h"

	class Cool_DHT
	{
		public:
			Cool_DHT();
			void	begin(uint8_t ConnectPin);
			void 	TypeDetection();
			uint8_t	errorlevel();
			inline	void	setError(uint8_t Error)	{	ErrorLevel = Error;	}
		public:
			void	readSensor();
			inline	uint8_t		getType()		{	return SensorType;	}
			inline	int16_t	getTemperature()	{	return Cool_Temp;	}
			inline	int8_t	getHumidity()		{	return Cool_Hum;	}
		private:
			uint8_t		DHTPin;
			uint8_t		SensorType;
			uint8_t		ErrorLevel;
			int16_t		Cool_Temp;
			int8_t		Cool_Hum;
	};
#endif