/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : ACTUATOR	-	MODEL : SUNNON
	ROLE : Le Petit Cool Fan
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_Fan_h
#define Cool_Fan_h

	#include "Arduino.h"
	#include "PetitCool.h"

/*/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////	 
	//FanMode[0]		ON/OFF	->	Demonstration
	//FanMode[1]		TIMER	->	Personnalisable
	//FanMode[2]		TEMP	->	Par Temperature
/////////////////////////////////////////////////////////*/
/////////////////////////////////////////////////////////*/
	
	class Cool_Fan
	{
		public:
			Cool_Fan();
			void			begin(uint8_t ConnectPin);
			uint8_t			errorlevel();
			inline	void	setError(uint8_t error)	{	ErrorLevel = error;	}
		public:
			void	setVal(uint8_t Power);
			inline	uint8_t	getVal()				{	return Power;		}
			inline	void	setMode(uint8_t mode)	{	Mode = mode;		}
			inline	uint8_t	getMode()				{	return Mode;		}
			inline	bool	getPhase()				{	return TimerPhase;	}
			inline	void	setPhase(bool Phase)	{	TimerPhase = Phase;	}
		private:
			uint8_t		FanPin;
			uint8_t		Mode;
			uint8_t		ErrorLevel;
			uint8_t		Power;
			bool		TimerPhase;
	};
#endif