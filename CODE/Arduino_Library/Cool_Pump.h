/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : ACTUATOR	-	MODEL : JP - 180
	ROLE : Le Petit Cool Pump
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_Pompe_h
#define Cool_Pompe_h

//Inclusion des librairies
	#include "Arduino.h"
	#include "PetitCool.h"

/*/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////	 
	//PumpMode[0]		ON/OFF	->	Demonstration
	//PumpMode[1]		TIMER	->	Hydroponie
	//PumpMode[2]		MOIST	->	Par humidité du substrat
/////////////////////////////////////////////////////////*/
/////////////////////////////////////////////////////////*/
	
	class Cool_Pump
	{
		public:
			Cool_Pump();
			void 			begin(uint8_t ConnectPin);
			uint8_t			errorlevel();
			inline	void 	setError(uint8_t error)	{	ErrorLevel = error;	}
		public:
			void	switchVal();
			void	setVal(uint8_t Power);
			inline 	uint8_t	getVal()				{	return IO;	}
			inline	void	setMode(uint8_t	mode)	{	Mode = mode;	}
			inline	uint8_t getMode()				{	return Mode;	}
			inline	uint8_t	getPhase()				{	return TimerPhase;	}
			inline	void	setPhase(bool Phase)	{	TimerPhase = Phase;	}
			
		private:
			uint8_t	Mode;
			uint8_t	PumpPin;
			uint8_t	ErrorLevel;
			uint8_t	IO;
			bool 	TimerPhase;
	};
#endif