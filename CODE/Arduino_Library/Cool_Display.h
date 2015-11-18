/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : ACTUATOR	-	MODEL : Petit Cool Display System
	ROLE : 	Le Petit Cool Display Management
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef Cool_Display_h
#define Cool_Display_h

//Inclusion des librairies
	#include "Arduino.h"
	#include "PetitCool.h"

	class Cool_Display
	{
		public:
			Cool_Display();
			void 	begin(int DigitClk, int DigitDio, int LedBarClk, int LedBarDio, int ButtonPin);
			int 	errorlevel();
			void 	setError(int error);
			void	testError(int i);
		public:
			inline void 	setInfoFromSensor(uint8_t index, uint16_t Data)	{	SensorInfo[index] = Data;	}
			inline void 	setLevelFromMem(uint8_t index, uint8_t Data)	{	Quality[index] = Data;	}
		private:
			uint8_t			BioDispErrorLevel;
			uint16_t		SensorInfo[13];
			uint8_t			Quality[12];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////*4-Digit*///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		public:
			void 	setDigitBright(uint8_t bright);
			bool 	DigitWriteByte(uint8_t Data);
			void 	DigitWriteData(uint8_t ToDisplay[]);
			void 	DigitDisplay(int Size, uint8_t Data[]);
			void 	DigitInfo(uint16_t Data);
			void 	DispInfo_4Digit(int Size, uint8_t Message[], uint16_t Data);
		private:
			uint8_t		DigitClk;
			uint8_t		DigitDio;
			uint8_t 	DigitNb;
			uint8_t 	StartDigit;
			int 		DigitBright;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////*LedBar*////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		public:
			void 	LightLedSet(unsigned int LedSet);
			void 	SendBarData(unsigned int Data);
			void 	BarQualityControl(uint8_t Data);
			void	BarTakePhoto();
			void	BarSecondDisplay();
			void	LedBarLightSet(uint8_t Level, bool RightToLeft = true);
			void	LedBarLightLed(uint8_t LedIndex, bool RightToLeft = true);
			void	LedBarBlinkLevel(uint8_t LedIndex, bool Warning = false);
		private:
			uint8_t			LedBarClk;
			uint8_t			LedBarDio;
			uint8_t 		LedBarBright[10];
			uint8_t			Led[10];
			unsigned int	LedBarDisplay;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////*Bouton*////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////				
		public:
			void 	ButtonControl();
			void 	ButtonAction(int Count);
		private:
			uint8_t			ButtonPin;
			int 			PushCounter;
			unsigned long 	PushResetTime;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////*Circular Led*/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	};
#endif