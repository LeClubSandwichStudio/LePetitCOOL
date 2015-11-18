/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove Luminosity Sensor
	ROLE : 	Le Petit Cool Luminosity Sensor
			Le Cool Stick Luminosity Sensor
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_Lum_h
#define Cool_Lum_h

//Inclusion des librairies
	#include "Arduino.h"
	#include "PetitCool.h"

	class Cool_Lum
	{
		public:
			Cool_Lum();
			void 	begin(uint8_t ConnectPin);
			uint8_t errorlevel();
			inline	void	setError(uint8_t Error)	{	ErrorLevel = Error;	}
		public:
			inline	void 	readData()			{	LVal = analogRead(LumPin);	}
			inline 	void 	readRVal()			{	RVal = (1023-analogRead(LumPin))*10/analogRead(LumPin);	}
			inline 	void 	readBrightness()	{	BVal = int(map(analogRead(LumPin), 0,1023,0,100));	}
		public:
			inline 	int		getVal()	{	return LVal;	}
			inline 	float 	getRVal()	{	return RVal;	}
			inline 	int 	getBVal()	{	return BVal;	}
		private:
			uint8_t	LumPin;
			uint8_t ErrorLevel;
			int		LVal;
			float	RVal;
			int		BVal;	
	};
#endif