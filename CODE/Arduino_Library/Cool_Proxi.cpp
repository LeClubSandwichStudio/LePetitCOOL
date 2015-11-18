/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MOD : Grove Ultrasonic Ranger
	ROLE : Le Petit Cool Water Level Sensor
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#include "Arduino.h"
#include "Cool_Proxi.h"

Cool_Proxi::Cool_Proxi()
{}

void Cool_Proxi::begin(uint8_t Pin)
{
	WLPin = Pin;
	Level = 0;
	PulseDuration = 0;
	errorlevel();
	pinMode(WLPin, INPUT);
}

	uint8_t Cool_Proxi::errorlevel()
{	//MOYENNE MOBILE A STABILISER//
//Initialisation:
	uint8_t NbTry = 2;
	uint8_t Temp[NbTry];
	uint8_t M[2*NbTry];
	uint8_t Test = 0;
//Test de fonctionnement:
	for(uint8_t i=0; i<NbTry; i++)
	{
		M[2*i] = digitalRead(WLPin);
		delay(1);
		M[2*i+1] = digitalRead(WLPin);
		delay(1);
		Temp[i] = (M[2*i]>M[2*i+1])?(M[2*i]-M[2*i+1]):(M[2*i+1]-M[2*i]);
		Temp[i] = (Temp[i]>1)?0:1;
	}
	for(int i=0; i<NbTry-1; i++)
	{
		Test += (Temp[2*i]==Temp[2*i+1]);
	}
//Niveau d erreur:
	ErrorLevel = (!Test)?1:0;
	return ErrorLevel;
}

void Cool_Proxi::ReadVal_cm()
{
	pinMode(WLPin, OUTPUT);
	digitalWrite(WLPin, LOW);
	delayMicroseconds(2);
	digitalWrite(WLPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(WLPin,LOW);
	pinMode(WLPin,INPUT);
	PulseDuration = pulseIn(WLPin,HIGH);
	Level = PulseDuration/29/2;
}