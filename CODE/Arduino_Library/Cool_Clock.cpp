/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : ACTUATOR	-	MODEL : Grove RTC
	ROLE : Le Petit Cool Time Setup
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#include "Arduino.h"
#include "PetitCool.h"
#include <Wire.h>

///////////////////////////////////////////////////////////////////////////////
////////////////////////////CONVERSION DE TYPE/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static uint8_t decbin2bin (uint8_t val)		{	return val - 6 * (val >> 4);	}

static uint8_t bin2decbin (uint8_t val)		{	return val + 6 * (val / 10);	}

static uint8_t char2byte(const char p)
{
    uint8_t v = 0;
    if ('0' <= p && p <= '9')	v = p - '0';
    return v;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////CONSTRUCTEURS///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Cool_Clock::setDateTimeFromCompile(const char* date, const char* time)
{
	WIRE.begin();
	DateTime[0] = char2byte(date[10]) + 10*char2byte(date[9]);
    switch(date[0])
	{
        case 'J': DateTime[1] = date[1] == 'a' ? 1 : DateTime[1] = date[2] == 'n' ? 6 : 7; break;
        case 'F': DateTime[1] = 2; break;
        case 'A': DateTime[1] = date[2] == 'r' ? 4 : 8; break;
        case 'M': DateTime[1] = date[2] == 'r' ? 3 : 5; break;
        case 'S': DateTime[1] = 9; break;
        case 'O': DateTime[1] = 10; break;
        case 'N': DateTime[1] = 11; break;
        case 'D': DateTime[1] = 12; break;
    }
    DateTime[2] = 10*char2byte(date[4])+char2byte(date[5]);
    DateTime[3] = 10*char2byte(time[0])+char2byte(time[1]);
    DateTime[4] = 10*char2byte(time[3])+char2byte(time[4]);
    DateTime[5] = 10*char2byte(time[6])+char2byte(time[7]);
	printSetup();
	return;
}

void Cool_Clock::setDateTimeFromApp(uint8_t* DT)
{
	WIRE.begin();
	for(uint8_t i = 0; i < 6; i++) {	DateTime[i] = DT[i];	}
	printSetup();
	return;
}

void Cool_Clock::printSetup()
{
	WIRE.beginTransmission(CLOCK_ADDRESS);
	WIRE.write(0);
	for(int i = 5; i >= 0; i--)
	{
		WIRE.write(bin2decbin(DateTime[i]));
		if(i == 3) WIRE.write(bin2decbin(0));
	}
	WIRE.write(0);
	WIRE.endTransmission();
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////ERRORLEVEL///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint8_t Cool_Clock::errorlevel()
{
	WIRE.beginTransmission(CLOCK_ADDRESS);
	WIRE.write(0);
	WIRE.endTransmission();
	WIRE.requestFrom(CLOCK_ADDRESS, 1);
	uint8_t ss = WIRE.read();
	ClockErrorLevel = !(ss>>7);
	ClockErrorLevel = !ClockErrorLevel;
	uint8_t EL = ClockErrorLevel?1:0;
	return EL;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////FONCTIONS RTC///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Cool_Clock::adjustTime()
{
	readNow();
	printSetup();
	return;
}

void Cool_Clock::readNow()
{
//Initialisation de la transmission:
	WIRE.beginTransmission(CLOCK_ADDRESS);
	WIRE.write(0);
	WIRE.endTransmission();
//Lecture:
	WIRE.requestFrom(CLOCK_ADDRESS, 7);
	DateTime[5] = decbin2bin(WIRE.read() & 0x7F);
	DateTime[4] = decbin2bin(WIRE.read());
	DateTime[3] = decbin2bin(WIRE.read());
	WIRE.read();
	DateTime[2] = decbin2bin(WIRE.read());
	DateTime[1] = decbin2bin(WIRE.read());
	DateTime[0] = decbin2bin(WIRE.read());
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////