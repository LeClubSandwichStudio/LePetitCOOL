/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove Moisture Sensor
	ROLE : 	Le Petit Cool Moisture Sensor
			Le Cool Stick Moisture Sensor
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_Moist_h
#define Cool_Moist_h

//Inclusion des librairies
	#include "Arduino.h"
	#include "PetitCool.h"

	class Cool_Moist
	{
		public:
			Cool_Moist();
			void begin(uint8_t ConnectPin);
			uint8_t errorlevel();
			inline	void	setError(uint8_t err)	{	ErrorLevel = err;	}
		public:
			inline void 	readVal()	{	MoistVal = int(map(analogRead(MoistPin), 0, 1023, 0 ,100));	}
			inline uint8_t	getVal()	{	return MoistVal;	}
		private:
			uint8_t MoistPin;
			uint8_t ErrorLevel;
			uint8_t MoistVal;
	};
#endif