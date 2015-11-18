/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : ACTUATOR	-	MODEL : Petit Cool Led System
	ROLE : 	Le Petit Cool Led Management
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_Led_h
#define Cool_Led_h

//Inclusion des librairies
	#include "Arduino.h"
	#include "PetitCool.h"

	class Cool_Led
	{
		public:
			Cool_Led();
			void	begin(uint8_t ConnectPin);
			uint8_t	errorlevel();
			inline	void	setError(uint8_t err)	{	ErrorLevel = err;}
		public:
			void	setVal(uint8_t Pow);
			inline void		setMode(uint8_t mode)	{	Mode = mode;		}
			inline uint8_t	getMode()				{	return Mode;		}
			inline void		setPhase(bool Phase)	{	TimerPhase = Phase;	}
			inline uint8_t	getVal()				{	return Power;	}
			inline bool		getPhase()				{	return TimerPhase;	}
		private:
			uint8_t LedPin;
			uint8_t ErrorLevel;
			uint8_t Power;
			uint8_t Mode;
			bool	TimerPhase;
	};
#endif