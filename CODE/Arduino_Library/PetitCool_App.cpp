/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : INTELLIGENCE	-	MODEL : Le Petit Cool
	ROLE : Le Petit Cool Application Intelligence
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

//Inclusion des librairies
	#include "Arduino.h"
	#include <SoftwareSerial.h>
	#include <EEPROM.h>
	#include <SD.h>

	#include "PetitCool_Def.h"
	#include "PetitCool.h"
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  STATIQUES  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static uint8_t DefaultConfig[] = 	{	138,64,					//Compteurs								2
										0,1,64,65,				//Module BT								4	6
										4,0,1,1,1,				//Param BT								5	11
										6,66,73,79,66,79,84,	//Name BT								7	18
										4,1,2,3,4,				//Pin BT								5	23
										1,1,0,					//RTC									3	26
										2,2,6,7,				//Luminosite							4	30
										3,2,37,39,				//DHT									4	34
										4,1,13,					//Pompe									3	37
										5,3,10,11,12,			//Ventilateurs							5	42
										6,3,12,13,14,			//Hydratation							5	47
										7,6,4,5,6,7,8,9,		//Led									8	55
										8,3,45,43,16,17,35,		//Display								7	62
										9,1,41,					//WaterLevel							3	65
										21,0,0,0,0,5,0,1,0,1,0,3,0,0,0,2,3,1,0,0,2,										//Timers		21		86
										13,80,75,0,0,8,22,25,28,22,40,60,25,											//Consignes		13		99
										38,0,3,1,4,2,1,0,0,0,3,1,0,0,0,4,0,5,1,0,1,0,1,0,6,1,0,1,0,1,0,7,0,0,0,0,0,0	//Modes			38		137
									};

static String CoolRead(SoftwareSerial& BlueTooth)
{
	String Res;
	Res.reserve(96);
	Res  = "";
	char recvChar;
	Res = Serial.readStringUntil(10);
	if(BlueTooth.available() > 0)
	{	
		while((BlueTooth.available() > 0)&&(recvChar != '\n'))
		{
			recvChar = BlueTooth.read();
			if(recvChar != '\n') Res.concat(recvChar);
		}
	}
	return Res;
}

static uint8_t char2byte(const char p)
{
    uint8_t v = (uint8_t)(-1);
    if ('0' <= p && p <= '9') v = p - '0';
	return v;
}

static bool IscharByte(const char p)
{
	bool v = false;
	if('0' <= p && p <= '9') v = true;
	return v;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A0  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
PetitCool_App::PetitCool_App()
{}

uint8_t PetitCool_App::begin(bool SDMem, bool EEPROMMem)
{
	InitiateBioApp();
//Chargement de la configuration:
	//Recuperation des types de memoires disponibles
	if(SDMem) SensorsInfo.MemHere[0] = true;
	else SensorsInfo.MemHere[0] = false;
	if(EEPROMMem) SensorsInfo.MemHere[1] = true;
	else SensorsInfo.MemHere[1] = false;	
	//Chargement SD
	if(SensorsInfo.MemHere[0])
	{
		LoadFromSD();
		return 2;
	}
	//Chargement EEPROM
	if((!SensorsInfo.MemHere[0])&&(SensorsInfo.MemHere[1]))
	{
		SensorsInfo.MemHere[1] = true;
		LoadFromEEPROM();
		return 1;
	}
	//Chargement par Defaut:	
	else
	{
		defaultLoading();
		return 0;
	}
}

void PetitCool_App::InitiateBioApp()
{
	for(uint8_t i = 0; i < 4; i++)	{	CommTest[i] = false;	}
	for(uint8_t i = 0; i < 2; i++)	{	SensorsInfo.MemHere[i] = false;	}
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)	{	SensorsInfo.Type[i] = 0;	}
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)	{	for(uint8_t j = 0; j < 14; j++)	{	SensorsInfo.Pins[i][j] = 0;	SensorsInfo.Mode[i][j] = 0;	}	}
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)	{	for(uint8_t j = 0; j < 7; j++)	{	SensorsInfo.Error[i][j] = 0;	}	}
	for(uint8_t i = 0; i < 7; i++)	{	BTInfo.Name[i] = 0;	}
	for(uint8_t i = 0; i < 4; i++)	{	BTInfo.PinCode[i] = 0;	}
	for(uint8_t i = 0; i < 4; i++)	{	BTInfo.Params[i] = 0;	}
	for(uint8_t i = 0; i < 6; i++)	{	AppData.ClockData[i] = 0;	}
	for(uint8_t i = 0; i < NB_CONS; i++)	{	AppData.Consignes[i] = 0;	}	
	(AppData.Order).reserve(54);
	for(uint8_t i = 0; i < 6; i++)	{	Timers.General[i] = 0;	}
	for(uint8_t i = 6; i < 9; i++)	{	Timers.General[i] = millis();	}
	Timers.General[0] = 5000;
	for(uint8_t i = 0; i < 18; i++)
	{
		for(uint8_t j = 0; j < 2; j++)	{	Timers.Led[i][j] = 0;	}
		Timers.Led[i][3] = millis();
	}
	for(uint8_t i = 0; i < TIMERS_TYPE; i++)
	{
		for(uint8_t j = 0; j < 14; j++)		{	Timers.Sensor[i][j] = 0;		}
		for(uint8_t j = 14; j < 21; j++)	{	Timers.Sensor[i][j] = millis();	}
	}
	StringBuffer0_0.reserve(21);
	StringBuffer0_1.reserve(21);
	StringBuffer0_2.reserve(21);
	StringBuffer0_3.reserve(21);
	StringBuffer0_4.reserve(21);
	for(uint8_t i = 0; i < 6; i++)
	{
		(StringBuffer1_0[i]).reserve(21);
		(StringBuffer1_1[i]).reserve(21);
		
	}
	StringBuffer2_0.reserve(255);
	StringBuffer2_1.reserve(124);
	return;
}

void PetitCool_App::CleanBuffer()
{
	for(uint8_t i = 0; i < 2; i++)	{	Uint8_cmpt[i] = 0;	}
	for(uint8_t i = 0; i < 96; i++)
	{
		Uint8_Buffer0_0[i] = 0;
		Uint8_Buffer0_1[i] = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A1  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
void PetitCool_App::LoadFromEEPROM()
{
//Initialisation:
	//Recuperation longueur liste a lire:
	Uint8_Buffer0_1[0] = EEPROM.read(0);
	//Initialisation compteur position:
	Uint8_cmpt[1] = 0;		//Liste generale
	Uint8_cmpt[0] = 0;		//Sous Liste Assignation
	//Ecriture:
	Serial.println(F("CHARGEMENT EEPROM..."));
	Serial.print(F("  Total nb of char to read : "));
	Serial.println(Uint8_Buffer0_1[0]);
//Recuperation Configuration:
	//Recuperation longueur sous liste configuration:
	Uint8_Buffer0_1[155] = EEPROM.read(1) - 1;	
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	//Enregistrement de la sous liste configuration:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = EEPROM.read(i+Uint8_cmpt[1]);	}
	//Assignation configuration:
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignSensor();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tConfig -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tConfig <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]); */
//Recuperation Timers:	
	//Recuperation longueur sous liste timers:
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	Uint8_Buffer0_1[155] = EEPROM.read(Uint8_cmpt[1]) - 1;
	//Enregistrement de la sous liste timers:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Uint8_Buffer0_0[i] = EEPROM.read(i+Uint8_cmpt[1]);	}
	//Assignation Timers:
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignTimers();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tTimers -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tTimers <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]);	 */
//Recuperation Consignes:
	//Recuperation sous liste consignes:
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	Uint8_Buffer0_1[155] = EEPROM.read(Uint8_cmpt[1]) - 1;
	//Enregestriement de la sous liste consignes:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = EEPROM.read(i+Uint8_cmpt[1]);	}
	//Assignation Consignes:
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignConsignes();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tConsignes -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tConsignes <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]);	 */
//Recuperation Modes:
	//Recuperation longueur sous liste modes:
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	Uint8_Buffer0_1[155] = EEPROM.read(Uint8_cmpt[1]) - 1;
	//Enregistrement de la sous liste modes:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = EEPROM.read(i+Uint8_cmpt[1]);	}
	//Assignation Modes:
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignModes();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tModes -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tModes <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]); */	
//Nettoyage:
	Serial.println(F("  Done.\n"));
	CleanBuffer();
	return;
}

void PetitCool_App::LoadFromSD()
{
//Ouverture du fichier config et lecture:
	if(SD.exists("MemConf.txt"))
	{
	//Initialisation:
		StringBuffer2_0 = "";
		//Copie du fichier:
		File FtoW = SD.open("MemConf.txt", FILE_READ);
		if(FtoW)
		{
			while(FtoW.available()) {	StringBuffer2_0.concat(char(FtoW.read()));	}
			FtoW.flush();
			FtoW.close();
		}
		//Recuperation longueur liste a lire:
		Uint8_Buffer0_1[0] = uint8_t(StringBuffer2_0.charAt(0));
		//Initialisation compteur position:
		Uint8_cmpt[1] = 0;		//Liste generale
		Uint8_cmpt[0] = 0;		//Sous Liste Assignation
		//Ecriture:
		Serial.println(F("CHARGEMENT DEPUIS SD..."));
		Serial.print(F("  Total nb of char to read : "));
		Serial.println(Uint8_Buffer0_1[0]);
	//Recuperation Configuration:
		//Recuperation longueur sous liste configuration:
		Uint8_Buffer0_1[155] = uint8_t(StringBuffer2_0.charAt(1)) - 1;
		Uint8_cmpt[0] = 0;
		Uint8_cmpt[1] += 1;
		//Enregistrement de la sous liste configuration:
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = uint8_t(StringBuffer2_0.charAt(i+Uint8_cmpt[1]));	}
		//Assignation configuration:
		while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignSensor();	}
		Uint8_cmpt[1] += Uint8_cmpt[0];
		/* //Ecrirure Configuration:
		Serial.print(F("\tConfig -> Nb char to read : "));
		Serial.println(Uint8_Buffer0_1[155]);
		Serial.print(F("\t\t "));
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
		Serial.print(F("\n\tConfig <- Nb of read char : "));
		Serial.println(Uint8_cmpt[0]);
		Serial.print(F("  Nb of char read : "));
		Serial.print(Uint8_cmpt[1] + 1);
		Serial.print(F("/"));
		Serial.println(Uint8_Buffer0_1[0]); */
	//Recuperation Timers:
		//Recuperation longueur sous liste timers:
		Uint8_cmpt[0] = 0;
		Uint8_cmpt[1] += 1;
		Uint8_Buffer0_1[155] = uint8_t(StringBuffer2_0.charAt(Uint8_cmpt[1])) - 1;
		//Enregistrement de la sous liste timers:
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Uint8_Buffer0_0[i] = uint8_t(StringBuffer2_0.charAt(i+Uint8_cmpt[1]));	}
		//Assignation Timers:
		while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignTimers();	}
		Uint8_cmpt[1] += Uint8_cmpt[0];
		/* //Ecriture:
		Serial.print(F("\tTimers -> Nb char to read : "));
		Serial.println(Uint8_Buffer0_1[155]);
		Serial.print(F("\t\t "));
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
		Serial.print(F("\n\tTimers <- Nb of read char : "));
		Serial.println(Uint8_cmpt[0]);
		Serial.print(F("  Nb of char read : "));
		Serial.print(Uint8_cmpt[1] + 1);
		Serial.print(F("/"));
		Serial.println(Uint8_Buffer0_1[0]); */
	//Recuperation Consignes:
		//Recuperation sous liste consignes:
		Uint8_cmpt[0] = 0;
		Uint8_cmpt[1] += 1;
		Uint8_Buffer0_1[155] = uint8_t(StringBuffer2_0.charAt(Uint8_cmpt[1])) - 1;
		//Enregestriement de la sous liste consignes:
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = uint8_t(StringBuffer2_0.charAt(i+Uint8_cmpt[1]));	}
		//Assignation Consignes:
		while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignConsignes();	}
		Uint8_cmpt[1] += Uint8_cmpt[0];
		/* //Ecriture:
		Serial.print(F("\tConsignes -> Nb char to read : "));
		Serial.println(Uint8_Buffer0_1[155]);
		Serial.print(F("\t\t "));
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
		Serial.print(F("\n\tConsignes <- Nb of read char : "));
		Serial.println(Uint8_cmpt[0]);
		Serial.print(F("  Nb of char read : "));
		Serial.print(Uint8_cmpt[1] + 1);
		Serial.print(F("/"));
		Serial.println(Uint8_Buffer0_1[0]); */
	//Recuperation Modes:
		//Recuperation longueur sous liste modes:
		Uint8_cmpt[0] = 0;
		Uint8_cmpt[1] += 1;
		Uint8_Buffer0_1[155] = uint8_t(StringBuffer2_0.charAt(Uint8_cmpt[1])) - 1;
		//Enregistrement de la sous liste modes:
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = uint8_t(StringBuffer2_0.charAt(i+Uint8_cmpt[1]));	}
		//Assignation Modes:
		Uint8_cmpt[0]++;
		while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignModes();	}
		Uint8_cmpt[1] += Uint8_cmpt[0];
		/* //Ecriture:
		Serial.print(F("\tModes -> Nb char to read : "));
		Serial.println(Uint8_Buffer0_1[155]);
		Serial.print(F("\t\t "));
		for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
		Serial.print(F("\n\tModes <- Nb of read char : "));
		Serial.println(Uint8_cmpt[0]);
		Serial.print(F("  Nb of char read : "));
		Serial.print(Uint8_cmpt[1] + 1);
		Serial.print(F("/"));
		Serial.println(Uint8_Buffer0_1[0]); */
	//Nettoyage:	
		StringBuffer2_0 = "";
		Serial.println("  Done.\n");
		CleanBuffer();
		return;
	}
	else
	{
		Serial.println(F("CHARGEMENT DEPUIS SD..."));
		Serial.print("\tPAS DE FICHIERS DISPONIBLES...\n\t");
		if(EEPROM.read(0) != 0)
		{
			SensorsInfo.MemHere[1] = true;
			LoadFromEEPROM();
			return;
		}
		else
		{
			defaultLoading();
			CleanBuffer();
			return;
		}
	}
}

void PetitCool_App::defaultLoading()
{
//Initialisation:
	//Recuperation longueur liste a lire:
	Uint8_Buffer0_1[0] = DefaultConfig[0];
	//Initialisation compteur position:
	Uint8_cmpt[1] = 0;		//Liste generale
	Uint8_cmpt[0] = 0;		//Sous Liste Assignation
	//Ecriture:
	Serial.println(F("CHARGEMENT PAR DEFAUT..."));
	Serial.print(F("  Total nb of char to read : "));
	Serial.println(Uint8_Buffer0_1[0]);
//Recuperation Configuration:
	//Recuperation longueur sous liste configuration:
	Uint8_Buffer0_1[155] = DefaultConfig[1] - 1;
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	//Enregestriement de la sous liste configuration:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = DefaultConfig[i+Uint8_cmpt[1]];	}
	//Assignation configuration:
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignSensor();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tConfig -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tConfig <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]); */
//Recuperation Timers:
	//Recuperation longueur sous liste timers:
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	Uint8_Buffer0_1[155] = DefaultConfig[Uint8_cmpt[1]] - 1;
	//Enregistrement de la sous liste timers:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Uint8_Buffer0_0[i] = DefaultConfig[i+Uint8_cmpt[1]];	}
	//Assignation Timers:
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignTimers();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tTimers -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tTimers <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]); */
//Recuperation Consignes:
	//Recuperation sous liste consignes:
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	Uint8_Buffer0_1[155] = DefaultConfig[Uint8_cmpt[1]] - 1;
	//Enregestriement de la sous liste consignes:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = DefaultConfig[i+Uint8_cmpt[1]];	}
	//Assignation Consignes:
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignConsignes();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tConsignes -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tConsignes <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]); */
//Recuperation Modes:
	//Recuperation longueur sous liste modes:
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] += 1;
	Uint8_Buffer0_1[155] = DefaultConfig[Uint8_cmpt[1]] - 1;
	//Enregistrement de la sous liste modes:
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++)	{	Uint8_Buffer0_0[i] = DefaultConfig[i+Uint8_cmpt[1]];	}
	//Assignation Modes:
	Uint8_cmpt[0]++;
	while(Uint8_cmpt[0] < Uint8_Buffer0_1[155])	{	AssignModes();	}
	Uint8_cmpt[1] += Uint8_cmpt[0];
	/* //Ecriture:
	Serial.print(F("\tModes -> Nb char to read : "));
	Serial.println(Uint8_Buffer0_1[155]);
	Serial.print(F("\t\t "));
	for(uint8_t i = 0; i < Uint8_Buffer0_1[155] + 1; i++) {	Serial.print(Uint8_Buffer0_0[i]); Serial.print(F(" "));	}
	Serial.print(F("\n\tModes <- Nb of read char : "));
	Serial.println(Uint8_cmpt[0]);
	Serial.print(F("  Nb of char read : "));
	Serial.print(Uint8_cmpt[1] + 1);
	Serial.print(F("/"));
	Serial.println(Uint8_Buffer0_1[0]); */
//Nettoyage:
	Serial.println(F("  Done.\n"));
	CleanBuffer();
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A2  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool_App::AssignSensor()
{
//Lecture du Type et du nombre:
	Uint8_Buffer0_1[1] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	SensorsInfo.Type[Uint8_Buffer0_1[1]] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
/* //Ecriture:	
	Serial.print(F("\t\tSensor Type "));
	Serial.print(Uint8_Buffer0_1[1]);
	Serial.print(F(" #"));
	Serial.print(SensorsInfo.Type[Uint8_Buffer0_1[1]]); */
//Exception Bluetooth:
	if(Uint8_Buffer0_1[1] == 0) getBTParam();
//Autres Capteurs:
	else if(Uint8_Buffer0_1[1] == 8) assignDisplay(Uint8_Buffer0_1[1]);
/* Camera
	else if(Uint8_Buffer0_1[1] == 10)
	{
		assignCamera(Uint8_Buffer0_1[1]);
		return;
	}*/
	else
	{
		assignSinglePin(Uint8_Buffer0_1[1]);
		assignDoublePin(Uint8_Buffer0_1[1]);
	}
	return;
}

void PetitCool_App::AssignTimers()
{
//Identification du capteur associe:
	Uint8_Buffer0_1[1] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	Uint8_Buffer0_1[2] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	if(Uint8_Buffer0_1[1] == 0)	AssignTimersGeneral();
	else if(Uint8_Buffer0_1[1] == 7) AssignTimersLed();
	else AssignTimersSensors();
	return;
}

void PetitCool_App::AssignConsignes()
{
//Recuperation de la consigne:
	Uint8_cmpt[0]++;
	AppData.Consignes[Uint8_cmpt[0] - 1] = Uint8_Buffer0_0[Uint8_cmpt[0]];
//Ecriture:
	/* Serial.print(F("\t\tConsigne #"));
	Serial.print(Uint8_cmpt[0] - 1);
	Serial.print(F(" set to "));
	Serial.println(AppData.Consignes[Uint8_cmpt[0] - 1]); */
	return;
}

	void PetitCool_App::AssignModes()
{	//	A COMPLETER	-	A DECOUPER	//
//Lecture du Type et du nombre:
	Uint8_Buffer0_1[1] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
/* //Ecriture:	
	Serial.print(F("\t\tMode Type "));
	Serial.print(Uint8_Buffer0_1[1]); */
	//Mode SAISON
	if(Uint8_Buffer0_1[1] == 0)
	{
		Uint8_Buffer0_1[2] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
		for(uint8_t i = 0; i < 14; i++)	{	SensorsInfo.Mode[0][i] = Uint8_Buffer0_1[2];	}
		/* //Ecriture:
		Serial.print(F(" : Saison -> "));
		Serial.print(SensorsInfo.Mode[0][0]);
		Serial.println(); */
		return;
	}
	//Mode HYDRO
	else if(Uint8_Buffer0_1[1] == 1)
	{
		Uint8_Buffer0_1[2] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
		for(uint8_t j = 0; j < 14; j++)	{	SensorsInfo.Mode[1][j] = Uint8_Buffer0_1[2];	}
		for(uint8_t j = 0; j < SensorsInfo.Type[4]; j++)
		{
			SensorsInfo.Mode[4][2*j] = SensorsInfo.Mode[1][2*j];
			SensorsInfo.Mode[4][2*j+1] = 0;
		}
		/* //Ecriture:
		Serial.print(F(" Hydro : "));
		Serial.print(SensorsInfo.Mode[1][0]);
		Serial.println();
		Serial.print(F("\t\t Ecrasement Pompe : "));
		Serial.print(SensorsInfo.Mode[4][0]);
		Serial.println(); */
		return;
	}
	//Mode Pompe:
	else if((Uint8_Buffer0_1[1] == 4))
	{
		for(uint8_t j = 0; j < SensorsInfo.Type[Uint8_Buffer0_1[1]]; j++)
		{	SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j+1] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];	}
		/* //Ecriture:
		Serial.print(F(" Pompe : "));
		for(uint8_t i = 0; i < 14; i++)	{	Serial.print(SensorsInfo.Mode[4][i]);	Serial.print(F(" "));	}
		Serial.println(); */
		return;
	}
	//Mode Capteurs:
	else if((Uint8_Buffer0_1[1] == 2))
	{
		for(uint8_t j = 0; j < SensorsInfo.Type[Uint8_Buffer0_1[1]]; j++)
		{	
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j+1] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
		}
		/* //Ecriture:
		Serial.print(F(" Lum "));
		for(uint8_t i = 0; i < 14; i++)	{	Serial.print(SensorsInfo.Mode[Uint8_Buffer0_1[1]][i]);	Serial.print(F(" "));	}
		Serial.println(); */
		return;
	}
	else if((Uint8_Buffer0_1[1] == 3))
	{
		for(uint8_t j = 0; j < SensorsInfo.Type[Uint8_Buffer0_1[1]]; j++)
		{	
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j+1] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
		}
		/* //Ecriture:
		Serial.print(F(" DHT "));
		for(uint8_t i = 0; i < 14; i++)	{	Serial.print(SensorsInfo.Mode[Uint8_Buffer0_1[1]][i]);	Serial.print(F(" "));	}
		Serial.println(); */
		return;
	}
	else if((Uint8_Buffer0_1[1] == 5))
	{
		for(uint8_t j = 0; j < SensorsInfo.Type[Uint8_Buffer0_1[1]]; j++)
		{	
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j+1] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
		}
		/* //Ecriture
		Serial.print(F(" Moist "));
		for(uint8_t i = 0; i < 14; i++)	{	Serial.print(SensorsInfo.Mode[Uint8_Buffer0_1[1]][i]);	Serial.print(F(" "));	}
		Serial.println(); */
		return;
	}
	else if((Uint8_Buffer0_1[1] == 6))
	{
		for(uint8_t j = 0; j < SensorsInfo.Type[Uint8_Buffer0_1[1]]; j++)
		{	
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
			SensorsInfo.Mode[Uint8_Buffer0_1[1]][2*j+1] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
		}
		/* //Ecriture
		Serial.print(F(" Fan "));
		for(uint8_t i = 0; i < 14; i++)	{	Serial.print(SensorsInfo.Mode[Uint8_Buffer0_1[1]][i]);	Serial.print(F(" "));	}
		Serial.println(); */
		return;
	}
	//Mode LED <-> SAISON
	else if(Uint8_Buffer0_1[1] == 7)
	{
		for(uint8_t j = 0; j < SensorsInfo.Type[7]; j++)
		{
			SensorsInfo.Mode[7][2*j] = SensorsInfo.Mode[0][0];
			SensorsInfo.Mode[7][2*j+1] = Uint8_Buffer0_0[(Uint8_cmpt[0])++];
		}
		/* //Ecriture:
		Serial.print(F(" Led "));
		for(uint8_t i = 0; i < 14; i++)	{	Serial.print(SensorsInfo.Mode[Uint8_Buffer0_1[1]][i]);	Serial.print(F(" "));	}
		Serial.println(); */		
		return;
	}
	else return;
}

void PetitCool_App::getBTParam()
{
	readBTpin();
	readBTparams();
	readBTname();
	readBTPinCode();
}

void PetitCool_App::accessBT(SoftwareSerial* BlueTooth)
{
	BT = BlueTooth;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A3  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool_App::assignDisplay(const uint8_t n)
{	
	if(n == 8)
	{
		switch(SensorsInfo.Type[n])
		{
			case 1:				//LedBar
			//Recuperation:	
				SensorsInfo.Pins[n][0] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
				SensorsInfo.Pins[n][1] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
			/* //Ecriture:
				Serial.print(F(" LedBar : "));
				Serial.print(SensorsInfo.Pins[n][0]);
				Serial.print(F(" - "));
				Serial.println(SensorsInfo.Pins[n][1]); */
				break;
			case 2:				//4-Digit + Bouton
			//Recuperation:	
				SensorsInfo.Pins[n][0] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
				SensorsInfo.Pins[n][1] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
				SensorsInfo.Pins[n][2] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
			/* //Ecriture:
				Serial.print(F(" 4-Digit : "));
				Serial.print(SensorsInfo.Pins[n][0]);
				Serial.print(F(" - "));
				Serial.print(SensorsInfo.Pins[n][1]);
				Serial.print(F("\tBouton : "));
				Serial.println(SensorsInfo.Pins[n][2]); */
				break;
			case 3:				//4-Digit + LedBar + Bouton
			//Recuperation:	
				SensorsInfo.Pins[n][0] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
				SensorsInfo.Pins[n][1] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
				SensorsInfo.Pins[n][2] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
				SensorsInfo.Pins[n][3] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
				SensorsInfo.Pins[n][4] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
			/* //Ecriture:
				Serial.print(F(" 4-Digit : "));
				Serial.print(SensorsInfo.Pins[n][0]);
				Serial.print(F(" - "));
				Serial.print(SensorsInfo.Pins[n][1]);
				Serial.print(F("\tLedBar : "));
				Serial.print(SensorsInfo.Pins[n][2]);
				Serial.print(F(" - "));
				Serial.print(SensorsInfo.Pins[n][3]);
				Serial.print(F("\tBouton : "));
				Serial.println(SensorsInfo.Pins[n][4]); */
				break;
			default:
				break;
		}
	}
	return;
}

void PetitCool_App::assignSinglePin(const uint8_t n)
{	//GESTION DES CAPTEURS -> A COMPLETER//
	if(n != 8)
	{
	//Recuperation:	
		for(uint8_t k = 0; k < SensorsInfo.Type[n]; k++)
		{	SensorsInfo.Pins[n][k] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];	}
	/* //Ecriture:
		Serial.print(F(" Pin : "));
		for(uint8_t k = 0; k < SensorsInfo.Type[n]; k++)
		{
			Serial.print(SensorsInfo.Pins[n][k]);
			if(k != SensorsInfo.Type[n] - 1) Serial.print(F("-"));
			else Serial.println();
		} */
	}
	return;
}

void PetitCool_App::assignDoublePin(const uint8_t n)
{	//GESTION DES CAPTEURS -> A COMPLETER//
	if((n != 8)&&(n  == 10))
	{
	//Recuperation:	
		for(uint8_t k = 0; k < 2*SensorsInfo.Type[n]; k++)
		{	SensorsInfo.Pins[n][k] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];	}
	/* //Ecriture:
		Serial.print(F(" Pin : "));
		for(uint8_t k = 0; k < 2*SensorsInfo.Type[n]; k++)
		{
			Serial.print(SensorsInfo.Pins[n][k]);
			if(k != 2*SensorsInfo.Type[n] - 1) Serial.print(F("-"));
			else Serial.println();
		} */
	}
	return;
}

void PetitCool_App::AssignTimersGeneral()
{
//Recuperation du Timer:	
	Uint8_Buffer0_1[3] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	Uint8_Buffer0_1[4] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	Uint8_Buffer0_1[5] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
//Conversion et enregistrement du timer:	
	Timers.General[Uint8_Buffer0_1[2]] = Uint8_Buffer0_1[3]*3600;
	Timers.General[Uint8_Buffer0_1[2]] += Uint8_Buffer0_1[4]*60;
	Timers.General[Uint8_Buffer0_1[2]] += Uint8_Buffer0_1[5];
	Timers.General[Uint8_Buffer0_1[2]] *= 1000;
/* //Ecriture	
	Serial.print(F("\t\tGeneral "));
	Serial.print(Uint8_Buffer0_1[2]);
	Serial.print(F(" : "));
	Serial.print(Uint8_Buffer0_1[3]);
	Serial.print(F(" hh "));
	Serial.print(Uint8_Buffer0_1[4]);
	Serial.print(F(" mm "));
	Serial.print(Uint8_Buffer0_1[5]);
	Serial.println(F(" ss"));
	Serial.print(F("\t\t Soit "));
	Serial.print(Timers.General[Uint8_Buffer0_1[2]]);
	Serial.println(F(" ms")); */
	return;
}

void PetitCool_App::AssignTimersLed()
{
//Recuperation du Timer:	
	Uint8_Buffer0_1[4] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	Uint8_Buffer0_1[5] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	Uint8_Buffer0_1[6] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
//Conversion et Enregistrement du timer:	
	Timers.Led[Uint8_Buffer0_1[2]][1] = Uint8_Buffer0_1[4]*3600;
	Timers.Led[Uint8_Buffer0_1[2]][1] += Uint8_Buffer0_1[5]*60;
	Timers.Led[Uint8_Buffer0_1[2]][1] += Uint8_Buffer0_1[6];
	Timers.Led[Uint8_Buffer0_1[2]][1] *= 1000;
/* //Ecriture:
	Serial.print(F("\t\tLed #"));
	Serial.print(Uint8_Buffer0_1[2]);
	Serial.print(F(" : "));
	Serial.print(Uint8_Buffer0_1[3]);
	Serial.print(F(" hh "));
	Serial.print(Uint8_Buffer0_1[4]);
	Serial.print(F(" mm "));
	Serial.print(Uint8_Buffer0_1[5]);
	Serial.println(F(" ss"));
	Serial.print(F("\t\t Soit "));
	Serial.print(Timers.General[Uint8_Buffer0_1[2]]);
	Serial.println(F(" ms")); */
	return;
}

void PetitCool_App::AssignTimersSensors()
{
/* //Ecriture:
	Serial.print(F("\t\tSensor Type "));
	Serial.print(Uint8_Buffer0_1[1]);
	Serial.print(F(" #")); */
//Identification SensorType -> TimerType:
	switch(Uint8_Buffer0_1[1])
	{
		case 3:
			Uint8_Buffer0_1[1] = 0;
			break;
		case 4:
			Uint8_Buffer0_1[1] = 1;
			break;
		case 5:
			Uint8_Buffer0_1[1] = 2;
			break;
		case 6:
			Uint8_Buffer0_1[1] = 3;
			break;
		case 8:
			Uint8_Buffer0_1[1] = 4;
			break;
		default:
			break;
	}
//Recuperation du Timer:
	Uint8_Buffer0_1[3] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	Uint8_Buffer0_1[4] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	Uint8_Buffer0_1[5] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
//Conversion et enregsitrement du timer:	
	Timers.Sensor[Uint8_Buffer0_1[1]][Uint8_Buffer0_1[2]] = Uint8_Buffer0_1[3]*3600;
	Timers.Sensor[Uint8_Buffer0_1[1]][Uint8_Buffer0_1[2]] += Uint8_Buffer0_1[4]*60;
	Timers.Sensor[Uint8_Buffer0_1[1]][Uint8_Buffer0_1[2]] += Uint8_Buffer0_1[5];
	Timers.Sensor[Uint8_Buffer0_1[1]][Uint8_Buffer0_1[2]] *= 1000;
/* //Ecriture:
	Serial.print(Uint8_Buffer0_1[2]);
	Serial.print(F(" : "));
	Serial.print(Uint8_Buffer0_1[3]);
	Serial.print(F(" hh "));
	Serial.print(Uint8_Buffer0_1[4]);
	Serial.print(F(" mm "));
	Serial.print(Uint8_Buffer0_1[5]);
	Serial.println(F(" ss"));
	Serial.print(F("\t\t Soit "));
	Serial.print(Timers.Sensor[Uint8_Buffer0_1[1]][Uint8_Buffer0_1[2]]);
	Serial.println(F(" ms")); */
	return;
}

void PetitCool_App::readBTpin()
{
//Recuperation:
	SensorsInfo.Pins[0][0] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	SensorsInfo.Pins[0][1] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
/* //Ecriture:
	Serial.print(F(" Pin : "));
	Serial.print(SensorsInfo.Pins[0][0]);
	Serial.print(F(" - "));
	Serial.println(SensorsInfo.Pins[0][1]); */
	return;
}

void PetitCool_App::readBTparams()
{
//Recuperation
	Uint8_Buffer0_1[2] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	for(uint8_t k = 0; k < Uint8_Buffer0_1[2]; k++)	{	BTInfo.Params[k] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];	}
/* //Ecriture:
	Serial.print(F("\t\t Param ["));
	Serial.print(Uint8_Buffer0_1[2]);
	Serial.print(F("] : "));
	for(uint8_t k = 0; k < Uint8_Buffer0_1[2]; k++)
	{
		Serial.print(BTInfo.Params[k]);
		if(k != Uint8_Buffer0_1[2] - 1) Serial.print(F("-"));
		else Serial.println();
	} */
//Nettoyage:
	Uint8_Buffer0_1[2] = 0;
	return;
}

void PetitCool_App::readBTname()
{
//Recuperation:
	Uint8_Buffer0_1[2] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	for(uint8_t k = 0; k < Uint8_Buffer0_1[2]; k++)	{	BTInfo.Name[k] = char(Uint8_Buffer0_0[++(Uint8_cmpt[0])]);	}
/* //Ecriture:
	Serial.print(F("\t\t Name ["));
	Serial.print(Uint8_Buffer0_1[2]);
	Serial.print(F("] : "));
	for(uint8_t k = 0; k < Uint8_Buffer0_1[2]; k++)
	{
		Serial.print(BTInfo.Name[k]);
		if(k != Uint8_Buffer0_1[2] - 1) Serial.print(F("-"));
		else Serial.println();
	} */
//Nettoyage:
	Uint8_Buffer0_1[2] = 0;
	return;
}

void PetitCool_App::readBTPinCode()
{
//Recuperation:	
	Uint8_Buffer0_1[2] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];
	for(uint8_t k = 0; k< Uint8_Buffer0_1[2]; k++)	{	BTInfo.PinCode[k] = Uint8_Buffer0_0[++(Uint8_cmpt[0])];	}
/* //Ecriture:
	Serial.print(F("\t\t Code ["));
	Serial.print(Uint8_Buffer0_1[2]);
	Serial.print(F("] : "));
	for(uint8_t k = 0; k < Uint8_Buffer0_1[2]; k++)
	{
		Serial.print(BTInfo.PinCode[k]);
		if(k != Uint8_Buffer0_1[2] - 1) Serial.print(F("-"));
		else Serial.println();
	} */
//Nettoyage:
	Uint8_Buffer0_1[2] = 0;
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////  COMMUNICATION--A0  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool_App::setDateFromExternal()
{
//Communication:
	BeginComm("R:T", "A:T\n");
	CommBodyReceive(6, "!?;", "R:N", "N:T");
	CoolFlush(*BT);
//Sauvegarde donnees recuperees:
	CommSaveDateTime();
	CommPrintDateTime();
//Nettoyage:
	for(uint8_t i = 0; i<6; i++)	{	StringBuffer1_0[i] = "";	}
	return;
}

bool PetitCool_App::getOrderFromExternal()
{	//REVOIR CONDITION DE COMMUNICATION//
	if((!Serial.available())&&(!(*BT).available()))	return false;
//Initialisation:
	StringBuffer1_0[0] = CoolRead(*BT);
	if(!StringBuffer1_0[0].equals("R:A")) return false;
//Communication:
	BeginComm("R:A", "\tA:A\n");
	CommBodyReceive(1, "!?", "R:O", "N:O");
//Ecriture:
	AppData.Order = StringBuffer1_0[0];
//Nettoyage:
	CoolFlush(*BT);
	for(uint8_t i = 0; i < 4; i++)	{	CommTest[i] = false;	}
	StringBuffer1_0[0] = "";
	return true;
}

void PetitCool_App::sendConfigToExternal(uint8_t* Config)
{
	if((!Serial.available())&&((*BT).available() > 0))	return; 		//Application Indisponible
//Initialisation:
	StringBuffer2_0 = "?";
	//Configuration machine:
	for(uint8_t i = 0; i < Config[0]; i++) {	StringBuffer2_0.concat(char(Config[i]));	}
	StringBuffer2_0.concat("!\n");
//Envoi de la configuration:
	BeginComm("R:C", "A:C\n");
	CommBodySend("N:C", "C:N");
//Nettoyage:
	CoolFlush(*BT);
	StringBuffer2_0 = "";
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////  COMMUNICATION--A1  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool_App::BeginComm(const String Requested, const String Accept)
{
	Uint8_cmpt[0] = 1;
	while(!CommTest[0])
	{
		if(Uint8_cmpt[0] > 25) break;
		StringBuffer0_0 = CoolRead(*BT);
		CoolPrint(F("\tRetry#"), *BT);
		CoolFlush(*BT);
		CoolPrintln(Uint8_cmpt[0], *BT);
		CommTest[0] = StringBuffer0_0.equals(Requested);
		Uint8_cmpt[0]++;
	}
	switch(CommTest[0])
	{
		case true:
			CommTest[0] = !CommTest[0];
			CoolPrint(F("\t"), *BT);
			CoolPrint(Accept, *BT);
			break;
		case false:
			CommTest[3] = !CommTest[0];
			CoolPrint(F("\tFAILED\n"), *BT);
			break;
	}
	CoolFlush(*BT);
	Uint8_cmpt[0] = 0;
	StringBuffer0_0 = "";
	return;
}

void PetitCool_App::EndComm()
{
	while(!(CommTest[0]))
	{
		StringBuffer0_2 = CoolRead(*BT);
		CommTest[0] = StringBuffer0_2.equals("EOC");
		CommTest[3] = CommTest[0];
	}
	CommTest[0] = false;
	CoolPrint(F("EOC\n"), *BT);
	CoolFlush(*BT);
	StringBuffer0_2 = "";
	return;
}

void PetitCool_App::CommBodyReceive(const uint8_t nb_do, const String SpecialChar, const String Requested, const String Accept)
{
	StringBuffer0_0 = Requested;
	StringBuffer0_1 = Accept;
	while(!CommTest[3])
	{
		for(uint8_t i = 0; i < nb_do; i++)
		{
			while((!CommTest[0])&&(!CommTest[1]))
			{
				while((!CommTest[0])&&(!CommTest[2]))
				{
					StringBuffer1_1[i] = CoolRead(*BT);
					CommReceiveData(SpecialChar, i);					
				}
				CommReceiveNext(i, SpecialChar);
			}
			CommEndLoop();
		}
		EndComm();
	}
	CommTest[3] =false;
	StringBuffer0_0 = "";
	StringBuffer0_1 = "";
	for(uint8_t i = 0; i < 6; i++)	{	StringBuffer1_1[i] = "";	}
	return;
}

void PetitCool_App::CommBodySend(const String Requested, const String Accept)
{
//Initialisation:
	StringBuffer0_0 = Requested;
	StringBuffer0_1 = Accept;
//Communication:
	while(!CommTest[3])
	{
		while((!CommTest[0])&&(!CommTest[1]))	{	CommSendData();	}
		EndComm();
	}
//Nettoyage:
	StringBuffer0_0 = "";
	StringBuffer0_1 = "";
	for(uint8_t i = 0; i < 4; i++)	{	CommTest[i] = false;	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////  COMMUNICATION--A2  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool_App::CommReceiveData(const String SpecialChar, const uint8_t i)
{
	if(SpecialChar.length() == 3)
	{
		CommTest[2] = (((StringBuffer1_1[i]).lastIndexOf(SpecialChar[0]) >= 0)&&((StringBuffer1_1[i]).lastIndexOf(SpecialChar[1]) >= 0));
		Uint8_cmpt[1] = (StringBuffer1_1[i]).lastIndexOf(SpecialChar[0])-(StringBuffer1_1[i]).lastIndexOf(SpecialChar[1]);
		StringBuffer1_1[i] = (CommTest[2])?(StringBuffer1_1[i]).substring((StringBuffer1_1[i]).lastIndexOf(SpecialChar[1])):"";
		CommTest[2] = (CommTest[2])&&(Uint8_cmpt[1] >= 4);
		CommTest[2] = (CommTest[2])&&((StringBuffer1_1[i]).charAt(0) == SpecialChar[1])&&((StringBuffer1_1[i]).charAt(2) == SpecialChar[2]);
		CommTest[2] = (CommTest[2])&&((StringBuffer1_1[i]).charAt(Uint8_cmpt[1]) == SpecialChar[0]);
		CommTest[2] = (CommTest[2])&&(char2byte((StringBuffer1_1[i]).charAt(1)) == i)&&(IscharByte((StringBuffer1_1[i]).charAt(3)));
		CommTest[1] = CommTest[2];
		Uint8_cmpt[1] = 0;
		return;
	}
	else if(SpecialChar.length() == 2)
	{
		//Traitement des donnees recues:
		CommTest[2] = (((StringBuffer1_1[i]).lastIndexOf(SpecialChar[0]) >= 0)&&((StringBuffer1_1[i]).lastIndexOf(SpecialChar[1]) >= 0));					//Verification de presence des caracteres limitants
		Uint8_cmpt[1] = (StringBuffer1_1[i]).lastIndexOf(SpecialChar[0])-(StringBuffer1_1[i]).lastIndexOf(SpecialChar[1]);									//Recuperation de la longueur de la phrase
		StringBuffer1_1[i] = (CommTest[2])?(StringBuffer1_1[i]).substring((StringBuffer1_1[i]).lastIndexOf(SpecialChar[1])):"";								//Suppression des redondances
		//Avancement de la communication:
		CommTest[2] = (CommTest[2])&&((StringBuffer1_1[i]).charAt(0) == SpecialChar[1])&&((StringBuffer1_1[i]).charAt(Uint8_cmpt[1]) == SpecialChar[0]);	//Verification de l'enregistrement des donnees traites:
		CommTest[1] = CommTest[2];
		//Nettoyage:
		Uint8_cmpt[1] = 0;
		return;
	}
	else return;
}

void PetitCool_App::CommSendData()
{
	int Size = StringBuffer2_0.length();
	uint8_t NbTry = Size/SEND_MAX_LENGTH + 1;
	while(!CommTest[1])
	{
	//Decoupe et Envoi des donnees:
		for(uint8_t i = 0; i < NbTry; i++)
		{
			for(uint8_t j = 0; j < SEND_MAX_LENGTH; j++)
			{
				if(SEND_MAX_LENGTH*i+j > Size + 1) break;
				else CoolPrint(StringBuffer2_0.charAt(SEND_MAX_LENGTH*i+j), *BT);
			}
		}
		StringBuffer2_1 = CoolRead(*BT);
		CommTest[1] = StringBuffer2_1.equals(StringBuffer0_0);
	}
	while(!CommTest[0])
	{
		CoolPrintln(StringBuffer0_0, *BT);
		StringBuffer2_1 = CoolRead(*BT);
		CommTest[0] = StringBuffer2_1.equals(StringBuffer0_1);
	}
	CoolFlush(*BT);
	StringBuffer2_1 = "";
	return;
}

void PetitCool_App::CommReceiveNext(const uint8_t i, const String SpecialChar)
{
	
	StringBuffer0_2 = StringBuffer0_0;
	StringBuffer0_3 = StringBuffer0_1;
	StringBuffer0_2.concat(i);
	StringBuffer0_3.concat(i);	
	CoolPrint(F("Received\n"), *BT);
	if(SpecialChar.length() == 2)
	{
		StringBuffer1_1[i] = (StringBuffer1_1[i]).substring((StringBuffer1_1[i]).lastIndexOf(SpecialChar[1])+1, (StringBuffer1_1[i]).lastIndexOf(SpecialChar[0]));
		StringBuffer1_0[i] = StringBuffer1_1[i];
	}
	else StringBuffer1_0[i] = (StringBuffer1_1[i]).substring((StringBuffer1_1[i]).lastIndexOf(SpecialChar[2])+1, (StringBuffer1_1[i]).lastIndexOf(SpecialChar[0]));
//Passage a la phrase suivante:
	while((!CommTest[0])&&(CommTest[1]))
	{
		StringBuffer0_4 = CoolRead(*BT);
		CoolPrintln(StringBuffer0_3, *BT);
		CommTest[0] = StringBuffer0_4.equals(StringBuffer0_2);		
	}
	CoolPrint(StringBuffer0_4, *BT);
	CoolFlush(*BT);
	StringBuffer0_2 = "";
	StringBuffer0_3 = "";
	StringBuffer0_4 = "";
	return;
}

void PetitCool_App::CommEndLoop()
{
	for(uint8_t i = 0; i < 3; i++)	{	CommTest[i] = false;	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////  COMMUNICATION--A3  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool_App::CommPrintDateTime()
{
	if((StringBuffer1_0[0]).equals("")) return;
	CoolFlush(*BT);
	CoolPrint(F("	App: ["), *BT);
	for(int i=0; i<6; i++)
	{
		CoolPrint(StringBuffer1_0[i], *BT);
		if(i != 5 ) CoolPrint(F("  "), *BT);
	}
	CoolPrintln(F("]"), *BT);
	CoolFlush(*BT);
	return;
}

void PetitCool_App::CommSaveDateTime()
{
	if((StringBuffer1_0[0]).equals(""))
	{
		for(uint8_t i = 0; i<6; i++)	{	AppData.ClockData[i] = 0;}
		return;
	}
	for(uint8_t i=0; i<6; i++)
	{
		Uint8_cmpt[0] = 0;
		Uint8_cmpt[1] = (StringBuffer1_0[i]).length();
		for(uint8_t j=0; j<Uint8_cmpt[1]; j++)
		{
			uint8_t Power = 1;
			for(uint8_t k = Uint8_cmpt[1]-(j+1); k > 0; k--) Power *= 10;
			Uint8_cmpt[0] += Power*char2byte((StringBuffer1_0[i]).charAt(j));
		}
		Uint8_Buffer0_0[i] = Uint8_cmpt[0];
	}
//Ecriture:
	for(uint8_t i = 0; i < 6; i++) {	AppData.ClockData[i] = Uint8_Buffer0_0[i];	}
//Nettoyage:	
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] = 0;
	for(uint8_t i = 0; i < 96; i++)	{	Uint8_Buffer0_0[i] = 0;	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////