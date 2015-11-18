/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : ACTUATOR	-	MODEL : Grove RTC
	ROLE : Le Petit Cool Time Setup
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_Clock_h
#define Cool_Clock_h

//Inclusion des librairies
	#include "Arduino.h"
	#include "PetitCool.h"
	#include <Wire.h>
	
	class Cool_Clock
	{
		public:
			void	setDateTimeFromCompile(const char* date, const char* time);
			void	setDateTimeFromApp(uint8_t* DT);
		public:	
			uint8_t 	errorlevel();
		public:
			void 	adjustTime();
			void 	readNow();
			void	printSetup();
		public:
			inline uint8_t	getNow(const uint8_t i)	{ return DateTime[i];	}
		private:
			uint8_t	DateTime[6];
			bool 	ClockErrorLevel;
	};
#endif