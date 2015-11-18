/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : INTELLIGENCE	-	MODEL : Le Petit Cool
	ROLE : Le Petit Cool Machine Intelligence
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

//Inclusion des librairies
	#include "Arduino.h"
	#include <Wire.h>
	#include <SoftwareSerial.h>
	#include <SD.h>
	#include <string.h>
	
	#include "PetitCool.h"
	#include "PetitCool_Def.h"
	#include "PetitCool_App.h"
	#include "Cool_Clock.h"
	#include "Cool_Lum.h"
	#include "Cool_DHT.h"
	#include "Cool_Pump.h"
	#include "Cool_Fan.h"
	#include "Cool_Moist.h"
	#include "Cool_Led.h"
	#include "Cool_Display.h"
	#include "Cool_Proxi.h"
	//#include "CoolCam.h"

/*/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////	 
	//SensorType[0]		BlueTooth
	//SensorType[1]		Lumiere
	//SensorType[2]		DHT
	
	//Params[0]			Mode de Connection
	//Params[1]			Connexion Materiel Appaire Seulement
	//Params[2]			Appairage automatique
	//Params[3]			Mode de Reception
/////////////////////////////////////////////////////////*/
/////////////////////////////////////////////////////////*/
void PetitCool::WriteCompileSD()
{
//Initialisation:
	Application.setMemHere(0, true);
	StringBuffer2 = "";
	StringBuffer3 = "";
//Effacement des anciens fichiers presents:
	if(SD.exists("Hello.txt"))
	{
		SD.remove("Hello.txt");
		StringBuffer3.concat(__DATE__);
		StringBuffer3.concat(" @ ");
		StringBuffer3.concat(__TIME__);
		StringBuffer3.concat("\t- Hello.txt\n");
	}
//Ecriture du fichier "Hello.txt":
	//Remplissage du buffer
	StringBuffer2.concat(F("\nBonjour,\nJe suis votre Petit Cool,\ndont la derniere mise a jour software a ete effectuee le :\n"));
	StringBuffer2.concat(String(__DATE__));
	StringBuffer2.concat(F(" @ "));
	StringBuffer2.concat(String(__TIME__));
	StringBuffer2.concat('\n');
	//Ecriture sur SD
	FtoW = SD.open("Hello.txt", FILE_WRITE);
	if(FtoW)
	{
		FtoW.print(StringBuffer2);
		FtoW.close();
		StringBuffer3.concat(__DATE__);
		StringBuffer3.concat(" @ ");
		StringBuffer3.concat(__TIME__);
		StringBuffer3.concat("\t+ Hello.txt\n");
	}
	else Serial.println(F("\tFAILED -- Error writing Hello"));
//Lecture du fichier "Hello.txt":
	//Lecture depuis SD
	FtoW = SD.open("Hello.txt", FILE_READ);
	if(FtoW)
	{
		StringBuffer2 = "";
		while(FtoW.available()) {	StringBuffer2.concat(char(FtoW.read()));	}
		FtoW.close();
		StringBuffer3.concat(__DATE__);
		StringBuffer3.concat(" @ ");
		StringBuffer3.concat(__TIME__);
		StringBuffer3.concat("\tR Hello.txt\n");
	}
	else Serial.println(F("\tFAILED -- Error reading Hello"));
	//Ecriture Serial
	Serial.print(F("Lecture du fichier Hello.txt :\n\t"));
	Serial.println(StringBuffer2);
//Ecriture Journal SD:
	FtoW= SD.open("PCLog.log", FILE_WRITE);
	if(FtoW)
	{
		FtoW.println(StringBuffer3);
		FtoW.flush();
		FtoW.close();
	}
	else Serial.println(F("\tFAILED -- Error writing Log"));
//Nettoyage:
	StringBuffer2 = "";
	StringBuffer3 = "";
	return;
}

const char* AssociateSensorName[SENSOR_TYPE] = {	"BlueTooth",			//	TYPE	0
													"RTC",					//	TYPE	1
													"Luminosite",			//	TYPE	2
													"DHT",					//	TYPE	3
													"Pompe",				//	TYPE	4
													"Fans",					//	TYPE	5
													"Moist",				//	TYPE	6
													"Led",					//	TYPE	7
													"Display",				//	TYPE	8
													"Niveau d'eau"			//	TYPE	9
												};

static uint8_t	LedSeasonPower[] = {	255, 51, 25, 51, 255, 204,
										255, 204, 204, 127, 204, 127,
										51, 255, 255, 0, 51, 25,
										127, 0, 0, 75, 204,	75
									};
///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A0  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
PetitCool::PetitCool()
{}

void PetitCool::begin()
{
//Initialisation Machine:
	InitiatePetitCool();
//Recuperation de la configuration machine:
	bool Mem[2] = {true, true};
	pinMode(SS, OUTPUT);
	if(!SD.begin(CS_Pin, MOSI_Pin, MISO_Pin, CLK_Pin)) Mem[0] = false;
	else
	{
		Serial.println(F("Carte SD Presente...\t"));
		WriteCompileSD();
	}	
	if(EEPROM.read(0) == 0) Mem[1] = false;
	Uint8_Buffer[0] = Application.begin(Mem[0], Mem[1]);
//Association des capteurs et messages d'erreurs:
	InitSensors();
	InitErrors();
	FactoryInit();
//Demonstration:
	Fireworks();
	return;
}

void PetitCool::InitiatePetitCool()
{
	for(uint8_t i = 0; i < 4; i++)	{	CommTest[i] = false;	}
	for(uint8_t i = 0; i < 255; i++)	{	SensorMemList[i] = 0;	}
	SentDataFormat.reserve(192);
	SensorSentence.reserve(124);
	AppDataSentence.reserve(124);
	StringBuffer0_0.reserve(21);
	StringBuffer0_1.reserve(21);
	StringBuffer0_2.reserve(21);
	StringBuffer1_0.reserve(96);
	StringBuffer1_1.reserve(96);
	StringBuffer1_2.reserve(96);
	StringBuffer2.reserve(192);
	StringBuffer3.reserve(296);
	StringBuffer4.reserve(512);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A1  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::InitSensors()
{	//A COMPLETER//
//Initialisation:
	uint8_t LoadMode = Uint8_Buffer[0];
	Uint8_Buffer[0] = 0;
	InitTYPE_0();	//BlueTooth
	CreateSensorList();	//Creation de la liste de capteur:
	CoolFlush(*BT);
	Application.sendConfigToExternal(SensorMemList);
	delay(500);
	Application.sendConfigToExternal(SensorMemList);
	InitTYPE_1();	//RTC
	InitTYPE_2();	//Luminosites
	InitTYPE_3();	//DHTs
	InitTYPE_4();	//Pompes
	InitTYPE_5();	//Ventilateurs
	InitTYPE_6();	//Hydratations
	InitTYPE_7();	//Leds
	InitTYPE_8();	//Display
	InitTYPE_9();	//WaterLevel
//Ecriture de la configuration:
	printConfig();
	if((LoadMode != 1)&&(LoadMode != 2))	{	WriteConfigEEPROM();	}
	if(LoadMode == 2) WriteConfigSD();
	CreateSentencesToSend();
	return;
}

void PetitCool::InitErrors()
{	//A COMPLETER//
//Initialisation:
	(Application.SensorsInfo).Error[0][0] = 0;
	InitERR_2();
	InitERR_3();
	InitERR_4();
	InitERR_5();
	InitERR_6();
	InitERR_7();
	InitERR_8();
	InitERR_9();
//Ecriture de la configuration:
	printErrors();
	CoolPrint(F(" Objets initialises\n"), *BT);
	CoolPrint(F("Biobot Initialise\n"), *BT);
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A2  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::FactoryInit()
{
	CoolPrint(F("Initialisation Usine : "), *BT);
	CoolPrint(F(__DATE__), *BT);
	CoolPrint(F(" @ "), *BT);
	CoolPrintln(F(__TIME__), *BT);
	CoolPrintln(F("Demarrage :"), *BT);
	return;
}

void PetitCool::printErrors()
{
//Initialisation:
	CommTest[0] = true;
	uint8_t j = 0;
	StringBuffer3 ="";
	StringBuffer4 ="\n";
//Creation du journal d'erreur:
	CoolPrintln(F("Ecriture Error.txt"), *BT);
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)
	{
		for(uint8_t k = 0; k < (Application.SensorsInfo).Type[i]; k++)	{	CommTest[0] = CommTest[0]&&((Application.SensorsInfo).Error[i][k] != 0);	}
		if(CommTest[0])
		{
			if(j == 0)	{	StringBuffer4 = "\tMessages d'erreur :\n";	}
			j++;
			StringBuffer4.concat("\t\tErreur ");
			StringBuffer4.concat(j);
			StringBuffer4.concat(" : Capteur ");
			StringBuffer4.concat(AssociateSensorName[i]);
			StringBuffer4.concat("\n");
		}
	}
	if(StringBuffer4.equals("\n")) {	StringBuffer4 = "\tAucune Erreur\n";	}
//Ecriture SD:	
	if((Application.SensorsInfo).MemHere[0])
	{
	//Effacement des fichiers existants:
		if(SD.exists("Error.txt")) SD.remove("Error.txt");
		//Ecriture des donnees:
		FtoW = SD.open("Error.txt", FILE_WRITE);
		if(FtoW)
		{
			FtoW.print(StringBuffer4);
			FtoW.flush();
			FtoW.close();
		}
		else CoolPrintln(F("\tFAILED -- Error writing Error"), *BT);
	}
//Decoupe pour envoi Serial:
	int Size = StringBuffer4.length();
	uint8_t NbTry = Size/SEND_MAX_LENGTH + 1;
//Ecriture Serial:
	for(uint8_t i = 0; i < NbTry; i++)
	{
		for(uint8_t j = 0; j < SEND_MAX_LENGTH; j++)
		{
			if(SEND_MAX_LENGTH*i+j > Size + 1) break;
			else CoolPrint(StringBuffer4.charAt(SEND_MAX_LENGTH*i+j), *BT);
		}
	}
	CoolPrintln(F("Ecriture Error.txt Terminee.\n"), *BT);
//Ecriture Journal SD:
	if((Application.SensorsInfo).MemHere[0])
	{
		StringBuffer3.concat((Application.AppData).ClockData[0] + 2000);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[1]);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[2]);
		StringBuffer3.concat(F(" @ "));
		StringBuffer3.concat((Application.AppData).ClockData[3]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[4]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[5]);
		StringBuffer3.concat("\t+ Error");
		FtoW = SD.open("PCLog.log", FILE_WRITE);
		if(FtoW)
		{
			FtoW.println(StringBuffer3);
			FtoW.flush();
			FtoW.close();
		}
		else CoolPrintln(F("\tFAILED -- Error writing Log"), *BT);
	}
//Nettoyage:
	CoolFlush(*BT);
	StringBuffer3 ="";
	StringBuffer4 = "";
	CommTest[0] = false;
	return;
}

void PetitCool::printConfig()
{
//Redemarrage SD:
	SD.end();
	delayMicroseconds(50);
	pinMode(SS, OUTPUT);
	SD.begin(CS_Pin, MOSI_Pin, MISO_Pin, CLK_Pin);
	delayMicroseconds(50);
//Creation de la liste de capteurs:
	CoolPrint(F("\nEcriture Config.txt\n\t"), *BT);
	StringBuffer3 = "";
	StringBuffer4 = "Liste du materiel Connecte :\n";
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)
	{
		if((Application.SensorsInfo).Type[i] != 0)
		{
			for(uint8_t j = 0; j < (Application.SensorsInfo).Type[i]; j++)
			{
				StringBuffer4.concat('\t');
				StringBuffer4.concat(AssociateSensorName[i]);
				StringBuffer4.concat(" ");
				StringBuffer4.concat((j+1));
				switch(i)
				{
					case 0:										//Bluetooth
						StringBuffer4.concat("  -> ");
						StringBuffer4.concat((Application.SensorsInfo).Pins[i][0]);
						StringBuffer4.concat(" - ");
						StringBuffer4.concat((Application.SensorsInfo).Pins[i][1]);
						StringBuffer4.concat("\n");
						break;
					case 1:										//RTC
						StringBuffer4.concat(" -> ");
						for(int k = 5; k>=0; k--)
						{
							StringBuffer4.concat((Application.AppData).ClockData[k]);
							if((k != 0)&&(k != 3)) StringBuffer4.concat("-");
							else if(k == 3) StringBuffer4.concat(" || ");
						}
						StringBuffer4.concat("\n");
						break;
					case 8:										//Display
						j = (Application.SensorsInfo).Type[i];					
						if((Application.SensorsInfo).Type[i] == 1)							//LedBar
						{
							StringBuffer4.concat("\n\t  Bar -> Clk : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][0]);
							StringBuffer4.concat(" || Dio : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][1]);
						}
						else if((Application.SensorsInfo).Type[i] == 2)						//4-Digit + Bouton
						{
							StringBuffer4.concat("\n\t  Dig -> Clk : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][0]);
							StringBuffer4.concat(" || Dio : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][1]);
							StringBuffer4.concat("\n\t  But -> Pin : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][2]);
						}
						else if((Application.SensorsInfo).Type[i] == 3)						//4-Digit + Bouton + LedBar
						{
							StringBuffer4.concat("\n\t  Dig -> Clk : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][0]);
							StringBuffer4.concat(" || Dio : ");							
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][1]);
							StringBuffer4.concat("\n\t  Bar -> Clk : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][2]);
							StringBuffer4.concat(" || Dio : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][3]);
							StringBuffer4.concat("\n\t  But -> Pin : ");
							StringBuffer4.concat((Application.SensorsInfo).Pins[i][4]);
						}
						else StringBuffer4.concat("Erreur -> Configuration Inconnue");		//Autre
						StringBuffer4.concat("\n");
						break;
					default:
						StringBuffer4.concat("  -> ");
						StringBuffer4.concat((Application.SensorsInfo).Pins[i][j]);
						StringBuffer4.concat("\n");
						break;
				}
			}
		}
	}
//Ecriture de la liste de capteurs:
	//Decoupe pour envoi:
	int Size = StringBuffer4.length();
	uint8_t NbTry = Size/SEND_MAX_LENGTH + 1;
	uint8_t NbTrySD = Size/SD_MAX_SEND + 1;
	//Effacement des fichiers existants:
	if((Application.SensorsInfo).MemHere[0])
	{
		if(SD.exists("Config.txt"))
		{
			SD.remove("Config.txt");
			StringBuffer3.concat((Application.AppData).ClockData[2] + 2000);
			StringBuffer3.concat(" | ");
			StringBuffer3.concat((Application.AppData).ClockData[1]);
			StringBuffer3.concat(" | ");
			StringBuffer3.concat((Application.AppData).ClockData[0]);
			StringBuffer3.concat(" @ ");
			StringBuffer3.concat((Application.AppData).ClockData[3]);
			StringBuffer3.concat(" : ");
			StringBuffer3.concat((Application.AppData).ClockData[4]);
			StringBuffer3.concat(" : ");
			StringBuffer3.concat((Application.AppData).ClockData[5]);
			StringBuffer3.concat("\t- Config.txt\n");
		}
		//Ecriture du nouveau fichier:
		FtoW = SD.open("Config.txt", FILE_WRITE);
		FtoW.flush();
		if(FtoW)
		{
			for(uint8_t i = 0; i < NbTrySD; i++)
			{
				for(int j = 0; j < SD_MAX_SEND; j++)
				{
					if(SD_MAX_SEND*i+j > Size + 1) break;
					else FtoW.print(StringBuffer4.charAt(SD_MAX_SEND*i+j));
				}
				FtoW.flush();
			}
			FtoW.close();
		}
		else CoolPrintln(F("\tFAILED -- Error writing Config"), *BT);
	}	
	//Ecriture Serial:
	for(uint8_t i = 0; i < NbTry; i++)
	{
		for(uint8_t j = 0; j < SEND_MAX_LENGTH; j++)
		{
			if(SEND_MAX_LENGTH*i+j > Size + 1) break;
			else CoolPrint(StringBuffer4.charAt(SEND_MAX_LENGTH*i+j), *BT);
		}
	}
	CoolPrintln(F("Ecriture Config.txt Terminee.\n"), *BT);
//Ecriture Journal SD:
	if((Application.SensorsInfo).MemHere[0])
	{
		StringBuffer3.concat((Application.AppData).ClockData[0] + 2000);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[1]);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[2]);
		StringBuffer3.concat(F(" @ "));
		StringBuffer3.concat((Application.AppData).ClockData[3]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[4]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[5]);
		StringBuffer3.concat(F("\t+ Config.txt"));
		FtoW = SD.open("PCLog.log", FILE_WRITE);
		if(FtoW)
		{
			FtoW.println(StringBuffer3);
			FtoW.flush();
			FtoW.close();
		}
		else CoolPrintln(F("\tFAILED -- Error writing Log"), *BT);
	}
//Nettoyage
	CoolFlush(*BT);
	StringBuffer3 = "";
	StringBuffer4 = "";
	return;
}

		void PetitCool::Fireworks()
{
	if(((Application.SensorsInfo).Type[6] > 0)&&((Application.SensorsInfo).Type[8] == 0))		//Demo Leds
	{
		
	}
	else if(((Application.SensorsInfo).Type[6] > 0)&&((Application.SensorsInfo).Type[8] > 0))	//Demo Led + Display
	{
		
	}
	else if(((Application.SensorsInfo).Type[6] == 0)&&((Application.SensorsInfo).Type[8] > 0))	//Demo Display
	{
		
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A3  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::InitTYPE_0()						//Initialisation BlueTooth
{	//	VERIFIER ATMode sur GROVE BT	-	Reecrire PrepareBTString	//
//Recuperation:
	if(((Application.SensorsInfo).Type[0] == 0)||((Application.SensorsInfo).Type[0] > 1)) BT = NULL;
	else if((Application.SensorsInfo).Type[0] == 1)
	{
	//Initialisation:
		BT = new SoftwareSerial((Application.SensorsInfo).Pins[0][0], (Application.SensorsInfo).Pins[0][1]);
		Application.accessBT(BT);
		pinMode((Application.SensorsInfo).Pins[0][0], INPUT);	//RX Pin
		pinMode((Application.SensorsInfo).Pins[0][1], OUTPUT);	//TX Pin
		(*BT).begin(BTBaudRate);
	/* ST MODE:
	//Parametres generaux:
		prepareBTString();
		(*BT).print(StringBuffer1_0);		
	//Mise en Fonctionnement:
		delay(2000);
		(*BT).println(StringBuffer1_1);
		delay(2000);
		(*BT).println(F("\tConnexion BlueTooth engagee."));
		(*BT).flush();
		delay(2000); */
	/* AT MODE: */
		StringBuffer1_0 = "AT";
		(*BT).print(StringBuffer1_0);
		delay(2000);
		//Set Baudrate:
		StringBuffer1_0.concat("+BAUD");
		StringBuffer1_0.concat(ATBaudRate);
		(*BT).print(StringBuffer1_0);
		StringBuffer1_0 = "";
		delay(2000);
		//Set Name:
		StringBuffer1_0 = "AT+NAME";
		StringBuffer1_0.concat((Application.BTInfo).Name);
		(*BT).print(StringBuffer1_0);
		StringBuffer1_0 = "";
		delay(2000);
		//Set Pin:
		StringBuffer1_0 = "AT+PIN";
		for(uint8_t i = 0; i<BTCodeLength; i++)	{	StringBuffer1_0.concat((Application.BTInfo).PinCode[i]);	}
		(*BT).print(StringBuffer1_0);
		delay(2000);
		//Sortie de mode AT:
		(*BT).print("AT+RESET");
		delay(2000);
		(*BT).println(F("\tConnexion BlueTooth engagee."));
		(*BT).flush();
	}
	StringBuffer1_0 = "";
	StringBuffer1_1 = "";
	return;
}

void PetitCool::InitTYPE_1()						//Initialisation RTC
{
	if(((Application.SensorsInfo).Type[1] == 0)||((Application.SensorsInfo).Type[1] > 1)) return;
	else if((Application.SensorsInfo).Type[1] == 1)
	{
		delay(1000);
		if(!Init_1_Serial())	Init_1_Compile();
	}
	return;
}

void PetitCool::InitTYPE_2()						//Initialisation Luminosites
{
	if((Application.SensorsInfo).Type[2] == 0)	return;
	else if(((Application.SensorsInfo).Type[2] > 0)||((Application.SensorsInfo).Type[2] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[2]; i++)	{	Lums[i].begin((Application.SensorsInfo).Pins[2][i]);	}
	}
	return;
}

void PetitCool::InitTYPE_3()						//Initialisation DHTs
{
	if((Application.SensorsInfo).Type[3] == 0) return;
	else if(((Application.SensorsInfo).Type[3] > 0)||((Application.SensorsInfo).Type[3] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[3]; i++)	{	DHT[i].begin((Application.SensorsInfo).Pins[3][i]);	}
	}
	return;
}

void PetitCool::InitTYPE_4()						//Initialisation Pompes
{
	if((Application.SensorsInfo).Type[4] == 0)	return;
	else if(((Application.SensorsInfo).Type[4] > 0)||((Application.SensorsInfo).Type[4] < 3))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[4]; i++)	{	Pompe[i].begin((Application.SensorsInfo).Pins[4][i]);	}
	}
	return;
}

void PetitCool::InitTYPE_5()						//Initialisation Ventilateurs
{
	if((Application.SensorsInfo).Type[5] == 0)	return;
	else if(((Application.SensorsInfo).Type[5] > 0)&&((Application.SensorsInfo).Type[5] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[5]; i++)
		{
			uint8_t TempPin = (Application.SensorsInfo).Pins[5][i];
			Fans[i].begin(TempPin);
		}
	}
	return;
}

void PetitCool::InitTYPE_6()						//Initialisation Hydratations
{
	if((Application.SensorsInfo).Type[6] == 0)	return;
	else if(((Application.SensorsInfo).Type[6] > 0)||((Application.SensorsInfo).Type[6] < 3))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[6]; i++)	{	Moist[i].begin((Application.SensorsInfo).Pins[7][i] + 54);	}
	}
	return;
}

void PetitCool::InitTYPE_7()						//Initialisation Leds
{
	if((Application.SensorsInfo).Type[7] == 0)	return;
	else if(((Application.SensorsInfo).Type[7] > 0)||((Application.SensorsInfo).Type[7] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)	{	Led[i].begin((Application.SensorsInfo).Pins[7][i]);	}
	}
	return;
}

void PetitCool::InitTYPE_8()						//Initialisation Display
{
	switch((Application.SensorsInfo).Type[8])
	{
		case 0:					//Neant
			break;
		case 1:					//LedBar
			Display.begin(0, 0, (Application.SensorsInfo).Pins[8][0], (Application.SensorsInfo).Pins[8][1], 0);
			break;
		case 2:					//Bouton + 4-Digit
			Display.begin((Application.SensorsInfo).Pins[8][0], (Application.SensorsInfo).Pins[8][1], 0, 0, (Application.SensorsInfo).Pins[8][2]);
			break;
		case 3:					//Bouton + 4-Digit + LedBar
			Display.begin((Application.SensorsInfo).Pins[8][0], (Application.SensorsInfo).Pins[8][1], (Application.SensorsInfo).Pins[8][2], (Application.SensorsInfo).Pins[8][3], (Application.SensorsInfo).Pins[8][4]);
			break;
		default:
			break;
	}
	return;
}

void PetitCool::InitTYPE_9()						//Initialisation WaterLevel
{
	if((Application.SensorsInfo).Type[9] == 0)	return;
	else if(((Application.SensorsInfo).Type[9] > 0)||((Application.SensorsInfo).Type[9] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[9]; i++)	{	WL[i].begin((Application.SensorsInfo).Pins[9][i]);	}
	}
	return;
}

void PetitCool::InitERR_2()						//Erreur Luminosites
{
	if((Application.SensorsInfo).Type[2] == 0) return;
	else if(((Application.SensorsInfo).Type[2] > 0)||((Application.SensorsInfo).Type[2] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[2]; i++)	{	(Application.SensorsInfo).Error[2][i] = Lums[i].errorlevel();	}
	}
	return;
}

void PetitCool::InitERR_3()						//Erreur DHTs
{
	if((Application.SensorsInfo).Type[3] == 0) return;
	else if(((Application.SensorsInfo).Type[3] > 0)||((Application.SensorsInfo).Type[3] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[3]; i++)	{	(Application.SensorsInfo).Error[3][i] = DHT[i].errorlevel();	}
	}
	return;
}

void PetitCool::InitERR_4()						//Erreur Pompes
{
	if((Application.SensorsInfo).Type[4] == 0) return;
	else if(((Application.SensorsInfo).Type[4] > 0)||((Application.SensorsInfo).Type[4] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[4]; i++)	{	(Application.SensorsInfo).Error[4][i] = DHT[i].errorlevel();	}
	}
	return;
}

void PetitCool::InitERR_5()						//Erreur Ventilateurs
{
	if((Application.SensorsInfo).Type[5] == 0) return;
	else if(((Application.SensorsInfo).Type[5] > 0)||((Application.SensorsInfo).Type[5] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[5]; i++)	{	(Application.SensorsInfo).Error[5][i] = Fans[i].errorlevel();	}
	}
	return;
}

void PetitCool::InitERR_6()						//Erreur Hydratations
{
	if((Application.SensorsInfo).Type[6] == 0) return;
	else if(((Application.SensorsInfo).Type[6] > 0)||((Application.SensorsInfo).Type[6] < 3))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[6]; i++)	{	(Application.SensorsInfo).Error[6][i] = Moist[i].errorlevel();	}
	}
	return;
}

void PetitCool::InitERR_7()						//Erreur Leds
{
	if((Application.SensorsInfo).Type[7] == 0) return;
	else if(((Application.SensorsInfo).Type[7] > 0)||((Application.SensorsInfo).Type[7] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)	{	(Application.SensorsInfo).Error[7][i] = Led[i].errorlevel();	}
	}
	return;
}

void PetitCool::InitERR_8()						//Erreur Display
{
	if((Application.SensorsInfo).Type[8] == 0) return;
	else if((Application.SensorsInfo).Type[8] > 0) (Application.SensorsInfo).Error[8][0] = Display.errorlevel();
	return;
}

void PetitCool::InitERR_9()						//Erreur WaterLevel
{
	if((Application.SensorsInfo).Type[9] == 0) return;
	else if(((Application.SensorsInfo).Type[9] > 0)||((Application.SensorsInfo).Type[9] < 7))
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[9]; i++)	{	(Application.SensorsInfo).Error[9][i] = WL[i].errorlevel();	}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  INITIALISATION--A4  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool PetitCool::Init_1_Serial()
{//OK//
	if((!Serial.available())&&((*BT).available() > 0))	return false; 		//Application Indisponible
	CoolPrintln(F(" Connexion Application Disponible"), *BT);
	CoolPrintln(F("  Mise a l'heure par Application"), *BT);
	CoolFlush(*BT);
	Application.setDateFromExternal();
	CoolFlush(*BT);
	if((Application.AppData).ClockData[0] == 0) return false;
	(Application.SensorsInfo).Error[1][0] = Clock.errorlevel();
	Clock.setDateTimeFromApp((Application.AppData).ClockData);
//Affichage:
	CoolPrint(F("	Box: ["), *BT);
	for(uint8_t i = 0; i < 6; i++)
	{
		CoolPrint((Application.AppData).ClockData[i], *BT);
		CoolPrint(F(" "), *BT);
	}	
	CoolPrintln(F("]"), *BT);
	CoolFlush(*BT);
	return true;
}

void PetitCool::Init_1_Compile()
{
	CoolPrintln(F(" Mise a l heure par Compilation"), *BT);
	(Application.SensorsInfo).Error[1][0] = Clock.errorlevel();
	Clock.setDateTimeFromCompile(__DATE__,__TIME__);
	for(uint8_t i = 0; i < 6; i++)	{	(Application.AppData).ClockData[i] = Clock.getNow(i);	}
	CoolPrint(F("\tDateTime : "), *BT);
	for(uint8_t i = 0; i<6; i++) {	CoolPrint((Application.AppData).ClockData[i], *BT); CoolPrint(F(" "), *BT);	}
	CoolPrint(F("\n"), *BT);
	CoolFlush(*BT);
	return;
}

void PetitCool::prepareBTString()
{	//	Ecrire AT Mode	-	Verifier Commande STNAME	//
/*ST MODE:*/
	StringBuffer1_0 = "\r\n+STWMOD=";
	StringBuffer1_0.concat((Application.BTInfo).Params[0]);
	StringBuffer1_0.concat("\r\n");
	StringBuffer1_0.concat("\r\n+STNAME=");
	StringBuffer1_0.concat((Application.BTInfo).Name);
	StringBuffer1_0.concat("\r\n");
	StringBuffer1_0.concat("\r\n+STPIN=");
	for(uint8_t i = 0; i<4; i++)	{	StringBuffer1_0.concat((Application.BTInfo).PinCode[i]);	}
	StringBuffer1_0.concat("\r\n");
	StringBuffer1_0.concat("\r\n+STOAUT=");
	StringBuffer1_0.concat((Application.BTInfo).Params[1]);
	StringBuffer1_0.concat("\r\n");
	StringBuffer1_0.concat("\r\n+STAUTO=");
	StringBuffer1_0.concat((Application.BTInfo).Params[2]);
	StringBuffer1_0.concat("\r\n");
	StringBuffer1_1 = "\r\n+INQ=";
	StringBuffer1_1.concat((Application.BTInfo).Params[3]);
	StringBuffer1_1.concat("\r\n");
/*AT MODE:*/
}

void PetitCool::WriteConfigEEPROM()
{
//Ecriture de la configuration:
	if(!(Application.SensorsInfo).MemHere[0])
	{
		CoolPrint(F(" Ecriture Configuration EEPROM...\n\t"), *BT);
		for(uint8_t i = 0; i < SensorMemList[0]; i++)
		{
			EEPROM.write(i, SensorMemList[i]);
			CoolPrint(SensorMemList[i], *BT);
		}
		CoolPrintln(F("\n Done\n"), *BT);
	}
	return;
}

void PetitCool::WriteConfigSD()
{
	CoolPrint(F("Ecriture SD MemConf.txt\n\t"), *BT);
//Ecriture SD:	
	if(SD.exists("MemConf.txt")) SD.remove("MemConf.txt");
	FtoW = SD.open("MemConf.txt", FILE_WRITE);
	FtoW.flush();
	if(FtoW)
	{
		for(uint8_t k = 0; k < SensorMemList[0]; k++)	{	FtoW.write(SensorMemList[k]);	}
		FtoW.close();		
	}
	else CoolPrintln(F("\tFAILED -- Error writing MemConf"), *BT);
//Ecriture Serial:
	for(uint8_t k = 0; k < SensorMemList[0]; k++)	{	CoolPrint(SensorMemList[k], *BT);	}
	CoolPrintln(F("\nEcriture SD MemConf.txt Terminee.\n"), *BT);
//Ecriture Journal SD:
	StringBuffer3.concat((Application.AppData).ClockData[0] + 2000);
	StringBuffer3.concat(" | ");
	StringBuffer3.concat((Application.AppData).ClockData[1]);
	StringBuffer3.concat(" | ");
	StringBuffer3.concat((Application.AppData).ClockData[2]);
	StringBuffer3.concat(" @ ");
	StringBuffer3.concat((Application.AppData).ClockData[3]);
	StringBuffer3.concat(":");
	StringBuffer3.concat((Application.AppData).ClockData[4]);
	StringBuffer3.concat(":");
	StringBuffer3.concat((Application.AppData).ClockData[5]);
	StringBuffer3.concat("\t+ MemConf");
	FtoW = SD.open("PCLog.log", FILE_WRITE);
	if(FtoW)
	{
		FtoW.println(StringBuffer3);
		FtoW.flush();
		FtoW.close();
	}
	else CoolPrintln(F("\tFAILED -- Error writing Log"), *BT);
//Nettoyage:
	CoolFlush(*BT);
	StringBuffer3 = "";
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
////////////////////////////  COMMUNICATION--A0  //////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::CreateSensorList()
{
//Initialisation:
	for(uint8_t i = 0; i < SensorMemList[0]; i++)	{	SensorMemList[i] = 0;	}
	Uint8_cmpt[0] = 0;
	Uint8_cmpt[1] = 0;
	SensorMemList[(Uint8_cmpt[0])++] = 0;
	SensorMemList[(Uint8_cmpt[0])++] = 0;
//Creation de la liste de capteurs:
	MakeSensorList();
	MakeTimersList();
	MakeConsignesList();
	MakeModeList();
	SensorMemList[0]++;
//Nettoyage:
	for(uint8_t i = 0; i < 5; i++)	{	Uint8_cmpt[i] = 0;	}
	return;
}

void PetitCool::CreateSentencesToSend()
{	//A COMPLETER//
	SensorSentence = "";
//Date et Heure:
	AddDateTimeToSentence();
//Donnees des capteurs:
	AddType_2_ToSentence();
	AddType_3_ToSentence();
	AddType_4_ToSentence();
	AddType_5_ToSentence();
	AddType_6_ToSentence();
	AddType_7_ToSentence();
	AddType_8_ToSentence();
	AddType_9_ToSentence();
//Consignes de l'Utilisateur:
	AppDataSentence = "";
	return;
}

void PetitCool::SendData()
{
	ReceiveOrder();
	display();
	CreateSentencesToSend();
	display();
	(Application.Timers).General[3] = millis();
	if((Application.Timers).General[3] - (Application.Timers).General[6] > (Application.Timers).General[0])
	{
		(Application.Timers).General[6] = (Application.Timers).General[3];
		CoolPrint(SensorSentence, *BT);
		CoolPrintln(AppDataSentence, *BT);
	}
//Ecriture SD:
	(Application.Timers).General[4] = millis();
	if((Application.Timers).General[4] - (Application.Timers).General[7] > (Application.Timers).General[1])
	{
		(Application.Timers).General[7] = (Application.Timers).General[4];
		//Ecriture des donnees:
		if((Application.SensorsInfo).MemHere[0])
		{
			FtoW = SD.open("Data.txt", FILE_WRITE);
			if(FtoW)
			{
				FtoW.println(SensorSentence);
				FtoW.flush();
				FtoW.close();
			}
			else CoolPrintln(F("\tFAILED -- Error writing Data"), *BT);
		}
		//Ecriture Journal SD:
		if((Application.SensorsInfo).MemHere[0])
		{
			StringBuffer3 = "";
			StringBuffer3.concat((Application.AppData).ClockData[0] + 2000);
			StringBuffer3.concat(" | ");
			StringBuffer3.concat((Application.AppData).ClockData[1]);
			StringBuffer3.concat(" | ");
			StringBuffer3.concat((Application.AppData).ClockData[2]);
			StringBuffer3.concat(" @ ");
			StringBuffer3.concat((Application.AppData).ClockData[3]);
			StringBuffer3.concat(":");
			StringBuffer3.concat((Application.AppData).ClockData[4]);
			StringBuffer3.concat(":");
			StringBuffer3.concat((Application.AppData).ClockData[5]);
			StringBuffer3.concat("\t+ Data");
			FtoW = SD.open("PCLog.log", FILE_WRITE);
			if(FtoW)
			{
				FtoW.println(StringBuffer3);
				FtoW.flush();
				FtoW.close();
			}
			else CoolPrintln(F("\tFAILED -- Error writing Log"), *BT);
			StringBuffer3 = "";
			return;
		}
	}
	else return;
}

void PetitCool::ReceiveOrder()
{
//Recuperation de l'ordre:
	if(!Application.getOrderFromExternal()) return;
	StringBuffer0_0 = (Application.AppData).Order;
//Traitement:
	OrderSensor();
	controlAll();
	StringBuffer0_0 = "";
//Envoi de donnees:
	CreateSentencesToSend();
	CoolPrint(SensorSentence, *BT);
	CoolPrintln(AppDataSentence, *BT);
//Nettoyage:
	CoolFlush(*BT);
	return;
}

	void PetitCool::OrderSensor()
{	//	A REVOIR (TIMERS)	-	A COMPLETER//
	uint8_t FirstSize = StringBuffer0_0.indexOf(':');
	StringBuffer0_1 = StringBuffer0_0.substring(0, FirstSize);
	StringBuffer0_2 = StringBuffer0_0.substring(FirstSize + 1);
	uint8_t Order;
		if(StringBuffer0_1.equals("CO"))	Order = 0;
		else if(StringBuffer0_1.equals("A"))	Order = 1;
		else if(StringBuffer0_1.equals("R"))	Order = 2;
		else if(StringBuffer0_1.equals("T"))	Order = 3;
		else if(StringBuffer0_1.equals("P"))	Order = 4;
		else if(StringBuffer0_1.equals("F"))	Order = 5;
		else if(StringBuffer0_1.equals("L"))	Order = 7;
		else if(StringBuffer0_1.equals("C"))	Order = 100;
		else Order = 255;
	switch(Order)
	{
		case 0:		//CONFIG
			Application.sendConfigToExternal(SensorMemList);
			break;
		case 100:	//CONSIGNES
			ModifyConsignesFromApp();
			CreateSensorList();
			if(!(Application.SensorsInfo).MemHere[0])	WriteConfigEEPROM();
			else WriteConfigSD();
			break;
		case 1:		//ADD
			OrderManageSensor('+');
			break;
		case 2:		//REMOVE
			OrderManageSensor('-');
			break;
		case 3:		//TIMERS
			/* FirstSize = StringBuffer0_2.indexOf(':');
			StringBuffer0_1 = StringBuffer0_2.substring(0, FirstSize);
			StringBuffer0_2 = StringBuffer0_2.substring(FirstSize + 1); */
			setTimers();
			/* if(!(Application.SensorsInfo).MemHere[0])	WriteConfigEEPROM();
			else WriteConfigSD(); */
			break;
		case 4:		//POMPE
			FirstSize = StringBuffer0_2.indexOf(':');
			StringBuffer0_1 = StringBuffer0_2.substring(0, FirstSize);
			StringBuffer0_2 = StringBuffer0_2.substring(FirstSize + 1);
			if(StringBuffer0_1.equals("A"))			{	ActOnPumpAll();	}
			else if(StringBuffer0_1.equals("P"))	{	ActOnPump();	}
			else if(StringBuffer0_1.equals("M"))		
			{
				parseOrder();
				Pompe[Uint8_Buffer[1]].setMode(Uint8_Buffer[2]);
				for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
			}
			break;
		case 5:		//VENTILATEURS
			FirstSize = StringBuffer0_2.indexOf(':');
			StringBuffer0_1 = StringBuffer0_2.substring(0, FirstSize);
			StringBuffer0_2 = StringBuffer0_2.substring(FirstSize + 1);
			if(StringBuffer0_1.equals("A"))			{	ActOnFanAll();	}
			else if(StringBuffer0_1.equals("P"))	{	ActOnFan();		}
			else if(StringBuffer0_1.equals("M"))		
			{
				parseOrder();
				Fans[Uint8_Buffer[1]].setMode(Uint8_Buffer[2]);
				for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
			}
			break;
		case 7:		//LED
			FirstSize = StringBuffer0_2.indexOf(':');
			StringBuffer0_1 = StringBuffer0_2.substring(0, FirstSize);
			StringBuffer0_2 = StringBuffer0_2.substring(FirstSize + 1);
			if(StringBuffer0_1.equals("A"))			{	ActOnLedAll();	}
			else if(StringBuffer0_1.equals("P"))	{	ActOnLed();		}
			else if(StringBuffer0_1.equals("S"))	//SAISON		
			{
			//Traitement:	
				parseOrder();
			//Mise à jour Mode et Saison:
				for(uint8_t i = 0; i < 14; i++)	{	(Application.SensorsInfo).Mode[0][i] = Uint8_Buffer[1];	}
				for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)	{	(Application.SensorsInfo).Mode[7][2*i] = Uint8_Buffer[1];	}
			//Sauvegarde:	
				CreateSensorList();
				if(!(Application.SensorsInfo).MemHere[0])	WriteConfigEEPROM();
				else WriteConfigSD();
			//Nettoyage:
				for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
			}
			else if(StringBuffer0_1.equals("SP"))	//SPECTRE PERSO
			{
			//Traitement:
				parseOrder();
			//Mise à jour Mode et Saison:
				if((Application.SensorsInfo).Mode[0][0] != 4)
				{
					for(uint8_t i = 0; i < 14; i++)	{	(Application.SensorsInfo).Mode[0][i] = 4;	}
					for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)	{	(Application.SensorsInfo).Mode[7][2*i] = 4;	}
				}
			//Mise a jour Puissance Leds:
				(Application.SensorsInfo).Mode[7][2*Uint8_Buffer[1] + 1] = Uint8_Buffer[2];
				CreateSensorList();
				//Nettoyage:
				for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
			}
			else if(StringBuffer0_1.equals("SPS"))	//SAUVEGARDE SPECTRE PERSO
			{
			//Sauvergarde:	
				if(!(Application.SensorsInfo).MemHere[0])	WriteConfigEEPROM();
				else WriteConfigSD();
			}
			else if(StringBuffer0_1.equals("M"))	//Mode	
			{
				parseOrder();
				Led[Uint8_Buffer[1]].setMode(Uint8_Buffer[2]);
				for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
			}
			break;
		default:
			break;
	}
	StringBuffer0_1 = "";
	StringBuffer0_2 = "";
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
////////////////////////////  COMMUNICATION--A1  //////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::MakeSensorList()
{
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)
	{
		SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = i;
		SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Type[i];
		if((Application.SensorsInfo).Type[i] != 0)
		{
			for(uint8_t j = 0; j < (Application.SensorsInfo).Type[i]; j++)
			{
				if(i == 0)		//Bluetooth
				{
					SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][0];
					SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][1];
					SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 4;
					for(uint8_t k = 0; k<4; k++)	{	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = Application.BTInfo.Params[k];	}
					Uint8_Buffer[0] = String((Application.BTInfo).Name).length();
					SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = Uint8_Buffer[0];
					for(uint8_t k = 0; k<Uint8_Buffer[0]; k++)	{	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = String(Application.BTInfo.Name).charAt(k); }
					SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = BTCodeLength;
					for(uint8_t k = 0; k<BTCodeLength; k++)	{	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = Application.BTInfo.PinCode[k];	}
				}
				else if(i == 8)	//Display
				{
					if((Application.SensorsInfo).Type[i] == 1)
					{
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][0];
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][1];
					}
					else if((Application.SensorsInfo).Type[i] == 2)
					{
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][0];
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][1];
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][2];
					}
					else if((Application.SensorsInfo).Type[i] == 3)
					{
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][0];
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][1];
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][2];
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][3];
						SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][4];
					}
					j = (Application.SensorsInfo).Type[i];
				}
				else	{	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Pins[i][j];	}
			}
		}
	}
	Uint8_cmpt[0] += Uint8_cmpt[1];
	SensorMemList[0] += Uint8_cmpt[0];			//MAJ Taille Totale
	SensorMemList[1] += ++Uint8_cmpt[1];		//MAJ Taille Capteurs
/* //Ecriture:
	for(uint8_t i = 0; i < SensorMemList[0]; i++) {	Serial.print(SensorMemList[i]);	}
	Serial.println(); */
//Nettoyage:
	Uint8_cmpt[1] = 0;
	Uint8_Buffer[0] = 0;
	return;
}

void PetitCool::MakeTimersList()
{
//Initialisation:
	unsigned long TimeSever[4];
	for(uint8_t i = 0; i < 4; i++) {	TimeSever[i] = 0;	}
	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 0;
//Timers Generaux:
	for(uint8_t i = 0; i < 3; i++)
	{
		if((Application.Timers).General[i] != 0)
		{
			TimeSever[0] = ((Application.Timers).General[i]/1000)%3600;
			TimeSever[1] = (((Application.Timers).General[i]/1000)-TimeSever[0])/3600;
			TimeSever[3] = TimeSever[0]%60;
			TimeSever[2] = (TimeSever[0] - TimeSever[3])/60;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 0;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = i;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[1];
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[2];
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[3];
		}
	}
//Timers capteurs:
	for(uint8_t i = 0; i < TIMERS_TYPE; i++)
	{
		switch(i)
		{
			case 0:	
				Uint8_Buffer[0] = 3;
				Uint8_Buffer[1] = 7;
				break;
			case 1:	
				Uint8_Buffer[0] = 4;
				Uint8_Buffer[1] = 3;
				break;
			case 2:	
				Uint8_Buffer[0] = 5;
				Uint8_Buffer[1] = 3;
				break;
			case 3:	
				Uint8_Buffer[0] = 6;
				Uint8_Buffer[1] = 3;
				break;
			case 4:	
				Uint8_Buffer[0] = 2;
				Uint8_Buffer[1] = 7;
				break;
			default:
				Uint8_Buffer[1] = 0;
				break;
		}
		for(uint8_t j = 0; j < Uint8_Buffer[1]; j++)
		{
			if((Application.Timers).Sensor[i][j] != 0)
			{
				TimeSever[0] = ((Application.Timers).Sensor[i][j]/1000)%3600;
				TimeSever[1] = (((Application.Timers).Sensor[i][j]/1000)-TimeSever[0])/3600;
				TimeSever[3] = TimeSever[0]%60;
				TimeSever[2] = (TimeSever[0] - TimeSever[3])/60;
				SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = Uint8_Buffer[0];
				SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = j;
				SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[1];
				SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[2];
				SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[3];
			}
		}
	}
//Timers Leds:
	for(uint8_t i = 0; i < 18; i++)
	{
		if((Application.Timers).Led[i][0] != 0)
		{
			TimeSever[0] = ((Application.Timers).Led[i][0]/1000)%3600;
			TimeSever[1] = (((Application.Timers).Led[i][0]/1000)-TimeSever[0])/3600;
			TimeSever[3] = TimeSever[0]%60;
			TimeSever[2] = (TimeSever[0] - TimeSever[3])/60;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 7;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = i;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 0;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[1];
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[2];
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[3];
		}
		if((Application.Timers).Led[i][1] != 0)
		{
			TimeSever[0] = ((Application.Timers).Led[i][1]/1000)%3600;
			TimeSever[1] = (((Application.Timers).Led[i][1]/1000)-TimeSever[0])/3600;
			TimeSever[3] = TimeSever[0]%60;
			TimeSever[2] = (TimeSever[0] - TimeSever[3])/60;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 7;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = i;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 1;
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[1];
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[2];
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = TimeSever[3];
		}
	}
//MAJ Tailles:
	SensorMemList[Uint8_cmpt[0]] += Uint8_cmpt[1];		//MAJ Taille Timers
	Uint8_cmpt[0] += Uint8_cmpt[1];
	SensorMemList[0] += Uint8_cmpt[1];					//MAJ Taille Totale
/* //Ecriture:
	for(uint8_t i = 0; i < SensorMemList[0]; i++) {	Serial.print(SensorMemList[i]);	}
	Serial.println(); */
//Nettoyage:
	Uint8_cmpt[1] = 0;
	Uint8_Buffer[0] = 0;
	Uint8_Buffer[1] = 0;
	return;
}

void PetitCool::MakeConsignesList()
{
//Initialisation:
	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 0;
//Consignes:
	for(uint8_t i = 0; i < 12; i++)	{	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.AppData).Consignes[i];	}
//MAJ Tailles:
	SensorMemList[Uint8_cmpt[0]] += Uint8_cmpt[1];		//MAJ Taille Consigne
	Uint8_cmpt[0] += Uint8_cmpt[1];
	SensorMemList[0] += Uint8_cmpt[1];					//MAJ Taille Totale
/* //Ecriture:
	for(uint8_t i = 0; i < SensorMemList[0]; i++) {	Serial.print(SensorMemList[i]);	}
	Serial.println(); */
//Nettoyage:
	Uint8_cmpt[1] = 0;
	return;
}

void PetitCool::MakeModeList()
{
//Initialisation:
	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = 0;
//Recuperation des Modes:
	for(uint8_t i = 0; i < MODES_TYPE; i++)
	{
		if((Application.SensorsInfo).Type[i] != 0)
		{
			SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = i;
			if((i == 2)||(i == 3)||(i == 5)||(i == 6))
			{
				for(uint8_t j = 0; j < ((Application.SensorsInfo).Type[i]); j++)
				{
					SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Mode[i][2*j];
					SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Mode[i][2*j+1];
				}
			}
			else if((i == 0)||(i == 1))	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Mode[i][0];
			else if(i == 4)
			{
				for(uint8_t j = 0; j < (Application.SensorsInfo).Type[4]; j++)
				{	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Mode[4][2*j+1];	}
				
			}
			else if(i == 7)
			{
				for(uint8_t j = 0; j < (Application.SensorsInfo).Type[7]; j++)
				{	SensorMemList[Uint8_cmpt[0] + (Uint8_cmpt[1]++)] = (Application.SensorsInfo).Mode[7][2*j+1];	}
			}
		}
	}
//MAJ Tailles:
	SensorMemList[Uint8_cmpt[0]] += Uint8_cmpt[1];		//MAJ Taille Mode
	Uint8_cmpt[0] += Uint8_cmpt[1];
	SensorMemList[0] += Uint8_cmpt[1];					//MAJ Taille Totale
/* //Ecriture:
	for(uint8_t i = 0; i < SensorMemList[0]; i++) {	Serial.print(SensorMemList[i]);	}
	Serial.println(); */
//Nettoyage:
	Uint8_cmpt[1] = 0;
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
////////////////////////////  COMMUNICATION--A1  //////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void PetitCool::setTimers()
{	//	EN COURS	//
//Tri des donnees recues:
	parseOrder();
	Serial.println();
//Modification:
	if(StringBuffer0_1.equals("GEN"))		(Application.Timers).General[Uint8_Buffer[1]] = (3600*Uint8_Buffer[2] + 60*Uint8_Buffer[3] + Uint8_Buffer[4])*1000;
	else if(StringBuffer0_1.equals("SEN"))	(Application.Timers).Sensor[Uint8_Buffer[1]][Uint8_Buffer[2]] = (3600*Uint8_Buffer[3] + 60*Uint8_Buffer[4] + Uint8_Buffer[5])*1000;
	else if(StringBuffer0_1.equals("LED"))	(Application.Timers).Led[Uint8_Buffer[1]][Uint8_Buffer[2]] = (3600*Uint8_Buffer[3] + 60*Uint8_Buffer[4] + Uint8_Buffer[5])*1000;
//Ecriture Journal SD:
	if((Application.SensorsInfo).MemHere[0])
	{
		StringBuffer3.concat((Application.AppData).ClockData[0] + 2000);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[1]);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[2]);
		StringBuffer3.concat(F(" @ "));
		StringBuffer3.concat((Application.AppData).ClockData[3]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[4]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[5]);
		StringBuffer3.concat(F("\t* Timer "));
		StringBuffer3.concat(Uint8_Buffer[1]);
		StringBuffer3.concat(F(" # "));
		StringBuffer3.concat(Uint8_Buffer[2]);
		StringBuffer3.concat(F(" -> "));
		StringBuffer3.concat(Uint8_Buffer[3]);
		FtoW = SD.open("PCLog.log", FILE_WRITE);
		if(FtoW)
		{
			FtoW.println(StringBuffer3);
			FtoW.flush();
			FtoW.close();
		}
		else CoolPrintln(F("\tFAILED -- Error writing Log"), *BT);
	}
//Nettoyage:
	StringBuffer3 = "";
	for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
	return;
}

void PetitCool::ModifyConsignesFromApp()
{
//Tri des donnees recues:
	parseOrder();
	if(Uint8_Buffer[1] > NB_CONS) return;
//Modification:
	(Application.AppData).Consignes[Uint8_Buffer[1]] = Uint8_Buffer[2];	
//Ecriture Journal SD:
	if((Application.SensorsInfo).MemHere[0])
	{
		StringBuffer3.concat((Application.AppData).ClockData[0] + 2000);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[1]);
		StringBuffer3.concat(F(" | "));
		StringBuffer3.concat((Application.AppData).ClockData[2]);
		StringBuffer3.concat(F(" @ "));
		StringBuffer3.concat((Application.AppData).ClockData[3]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[4]);
		StringBuffer3.concat(F(":"));
		StringBuffer3.concat((Application.AppData).ClockData[5]);
		StringBuffer3.concat(F("\t* Cons "));
		StringBuffer3.concat(Uint8_Buffer[1]);
		StringBuffer3.concat(F(" -> "));
		StringBuffer3.concat(Uint8_Buffer[2]);
		FtoW = SD.open("PCLog.log", FILE_WRITE);
		if(FtoW)
		{
			FtoW.println(StringBuffer3);
			FtoW.flush();
			FtoW.close();
		}
		else CoolPrintln(F("\tFAILED -- Error writing Log"), *BT);
	}
//Nettoyage:
	StringBuffer3 = "";
	CoolFlush(*BT);
	for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
	return;
}

	void PetitCool::ActOnPump()
{	//	Revoir Gestion modes	-	volatile dans BioPompe.h???	//
//Tri des donnees recues:
	parseOrder();
		for(uint8_t i = 0; i < Uint8_Buffer[0]; i++)	{	Serial.println(Uint8_Buffer[i]);	}
//Modification:
	/* Uint8_Buffer[49] = Pompe[Uint8_Buffer[1]].getMode();	//Recuperation du mode en cours */
	Pompe[Uint8_Buffer[1]].setMode(0);						//Overide
	CommTest[0] = Pompe[Uint8_Buffer[1]].getPhase();		//Recuperation de la phase (On/Off)
	Pompe[Uint8_Buffer[1]].setPhase(!CommTest[0]);			//Inversion de la phase
	/* Pompe[Uint8_Buffer[1]].setMode(Uint8_Buffer[49]); 	//Retour au mode normal */
//Ecriture Journal SD:

//Nettoyage:
	CommTest[0] = false;
	for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
	return;
}

	void PetitCool::ActOnFan()
{	//	Revoir Gestion modes	-	volatile dans BioFan.h???	//
//Tri des donnees recues:
	parseOrder();
		for(uint8_t i = 0; i < Uint8_Buffer[0]; i++)	{	Serial.println(Uint8_Buffer[i]);	}
//Modification:
	/* Uint8_Buffer[49] = Fans[Uint8_Buffer[1]].getMode();	//Recuperation du mode en cours */
	Fans[Uint8_Buffer[1]].setMode(0);						//Overide
	CommTest[0] = Fans[Uint8_Buffer[1]].getPhase();			//Recupertaion de la phase (On/Off)
	Fans[Uint8_Buffer[1]].setPhase(!CommTest[0]);			//Inversion de la phase
	/* Fans[Uint8_Buffer[1]].setMode(Uint8_Buffer[49]);		//Retour au mode normal */
//Ecriture Journal SD:

//Nettoyage:
	CommTest[0] = false;
	for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
	return;
}

	void PetitCool::ActOnLed()
{	//	A REVOIR	//
	parseOrder();
		for(uint8_t i = 0; i < Uint8_Buffer[0]; i++)	{	Serial.println(Uint8_Buffer[i]);	}
	if(Led[Uint8_Buffer[1]].getMode() == 1)	Led[Uint8_Buffer[1]].setMode(0);
	else	Led[Uint8_Buffer[1]].setMode(0);
	CommTest[0] = Led[Uint8_Buffer[1]].getPhase();
	Led[Uint8_Buffer[1]].setPhase(!CommTest[0]);
	CommTest[0] = false;	
	for(uint8_t i = 0; i < 50; i++)	{	Uint8_Buffer[i] = 0;	}
	return;
}

void PetitCool::ActOnPumpAll()
{	
	CommTest[0] = true;
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[4]; i++)	{	CommTest[0] = CommTest[0]&&Pompe[i].getPhase();	}
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[4]; i++)
	{
		Pompe[i].setMode(0);
		Pompe[i].setPhase(!CommTest[0]);
	}
	CommTest[0] = false;
	return;
}

void PetitCool::ActOnFanAll()
{
	CommTest[0] = true;
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[5]; i++)	{	CommTest[0] = CommTest[0]&&Fans[i].getPhase();	}
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[5]; i++)
	{
		Fans[i].setMode(0);
		Fans[i].setPhase(!CommTest[0]);
	}
	CommTest[0] = false;
	return;
}

void PetitCool::ActOnLedAll()
{
	CommTest[0] = true;
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)	{	CommTest[0] = CommTest[0]&&Led[i].getPhase();	}
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)
	{
		Led[i].setMode(0);
		Led[i].setPhase(!CommTest[0]);
	}
	CommTest[0] = false;
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
////////////////////////////  COMMUNICATION--A1  //////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::AddDateTimeToSentence()
{	//RTC
	for(uint8_t i = 0; i<6; i++)
	{
		if(i == 0) SensorSentence.concat(((Application.AppData).ClockData[i]+2000));
		else SensorSentence.concat((Application.AppData).ClockData[i]);		
		SensorSentence.concat(":");
	}
}

void PetitCool::AddType_2_ToSentence()
{	//Luminosites:
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[2]; i++)
	{
		SensorSentence.concat(String(Lums[i].getVal()));
		SensorSentence.concat(":");
	}
}

void PetitCool::AddType_3_ToSentence()
{	//DHTs:
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[3]; i++)
	{
		SensorSentence.concat(String(DHT[i].getTemperature()));
		SensorSentence.concat(":");
		SensorSentence.concat(String(DHT[i].getHumidity()));
		SensorSentence.concat(":");
	}
}

void PetitCool::AddType_4_ToSentence()
{	//Pompes:
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[4]; i++)
	{
		SensorSentence.concat(String(Pompe[i].getVal()));
		SensorSentence.concat(":");
	}
}

void PetitCool::AddType_5_ToSentence()
{	//Ventilateurs:
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[5]; i++)
	{
		SensorSentence.concat(String(Fans[i].getVal()));
		SensorSentence.concat(":");
	}
}

void PetitCool::AddType_6_ToSentence()
{	//Hydratations:
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[6]; i++)
	{
		SensorSentence.concat(String(Moist[i].getVal()));
		SensorSentence.concat(":");
	}
}

void PetitCool::AddType_7_ToSentence()
{	//Leds:
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)
	{
		SensorSentence.concat(String(Led[i].getVal()));
		SensorSentence.concat(":");
	}
}

void PetitCool::AddType_8_ToSentence()
{	//Display:
	return;
}

void PetitCool::AddType_9_ToSentence()
{	//Niveau d'eau:
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[9]; i++)
	{
		SensorSentence.concat(String(WL[i].getVal_cm()));
		SensorSentence.concat(":");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  GESTION CAPTEURS--A0  /////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::parseOrder()
{
	Uint8_cmpt[0] = StringBuffer0_2.indexOf(":");
	Uint8_cmpt[1] = 1;
	StringBuffer1_0 = StringBuffer0_2.substring(Uint8_cmpt[0]+1);
	StringBuffer1_1 = StringBuffer0_2.substring(0, Uint8_cmpt[0]);
	CommTest[0] = false;
	while(!CommTest[0])
	{		
		Uint8_Buffer[Uint8_cmpt[1]++] = StringBuffer1_1.toInt();
		CommTest[0] = (Uint8_cmpt[0] == uint8_t(-1));
		Uint8_cmpt[0] = StringBuffer1_0.indexOf(":");
		StringBuffer1_1 = StringBuffer1_0.substring(0, Uint8_cmpt[0]);
		StringBuffer1_0 = StringBuffer1_0.substring(Uint8_cmpt[0]+1);
	}
	Uint8_Buffer[0] = Uint8_cmpt[1];
	StringBuffer1_0 = "";
	StringBuffer1_1 = "";
	return;
}

void PetitCool::OrderManageSensor(const char Op)
{
//Initialisation:
	parseOrder();
	if(Uint8_Buffer[1] == 0 || Uint8_Buffer[1] == 1) return;
//Modification:
	if(Op == '+') addSensor();
	else if(Op == '-') removeSensor();
//Reconfiguration:
	CreateSensorList();
	Application.sendConfigToExternal(SensorMemList);
	InitTYPE_2();	//Luminosites
	InitTYPE_3();	//DHTs
	InitTYPE_4();	//Pompes
	InitTYPE_5();	//Ventilateurs
	InitTYPE_6();	//Hydratations
	InitTYPE_7();	//Leds
	InitTYPE_8();	//Display
	InitTYPE_9();	//WaterLevel
//Ecriture de la configuration:
	CoolFlush(*BT);
	printConfig();
	if((!(Application.SensorsInfo).MemHere[0])&&((Application.SensorsInfo).MemHere[1]))	{	WriteConfigEEPROM();	}
	if((Application.SensorsInfo).MemHere[0]) WriteConfigSD();
	CreateSentencesToSend();
//Nettoyage:
	Uint8_Buffer[0] = 0;
	Uint8_Buffer[1] = 0;
	return;
}

void PetitCool::addSensor()
{	//	Revoir Exception Display -> Augmentation	//
//Initialisation:
	for(uint8_t i = 0; i < Uint8_Buffer[0]-3; i++)	{	Uint8_Buffer1[i] = Uint8_Buffer[i+3];	}					//Recuperation des pins a ajouter
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)		{	for(uint8_t j = 0; j < 14; j++)	{	PinsCopy[i][j] = 0;	}}	//Reinitialisation Buffer Pins
//Ajout:
	for(uint8_t i = 0; i<SENSOR_TYPE; i++)
	{
	//Mise a jour Nb Capteurs:	
		if(i == Uint8_Buffer[1])	Uint8_Buffer0[i] = (Application.SensorsInfo).Type[i] + Uint8_Buffer[2];
		else if(i != Uint8_Buffer[1])	Uint8_Buffer0[i] = (Application.SensorsInfo).Type[i];
	//Mis a jour Pins:	
		//Pin Simple:	
		if((i != 0)&&(i != 8))
		{
			for(uint8_t j = 0; j<(Application.SensorsInfo).Type[i]; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
			if(i == Uint8_Buffer[1])
			{
				for(uint8_t j = (Application.SensorsInfo).Type[i]; j < Uint8_Buffer0[i]; j++)	{	PinsCopy[i][j] = Uint8_Buffer1[j-(Application.SensorsInfo).Type[i]];	}
			}
		}
		//Pin Double:
		else if((i == 0)||(i == 10))
		{
			for(uint8_t j = 0; j<2*(Application.SensorsInfo).Type[i]; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
			if(i == Uint8_Buffer[1])
			{
				for(uint8_t j = 2*(Application.SensorsInfo).Type[i]; i< 2*Uint8_Buffer0[i]; i++)	{	PinsCopy[i][j] = Uint8_Buffer1[j-2*(Application.SensorsInfo).Type[j]];	}
			}
		}
		//Exception Display:
		else if(i == 8)
		{
			switch((Application.SensorsInfo).Type[i])
			{
				case 1:
					for(uint8_t j = 0; j < 2; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
					break;
				case 2:
					for(uint8_t j = 0; j < 3; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
					break;
				case 3:
					for(uint8_t j = 0; j < 5; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
					break;
				default:
					break;
			}
		}
	}
//Sauvegarde:
	for(uint8_t i = 0; i<SENSOR_TYPE; i++)
	{
		(Application.SensorsInfo).Type[i] = Uint8_Buffer0[i];
		for(uint8_t j = 0; j < 14; j++)	{	(Application.SensorsInfo).Pins[i][j] = PinsCopy[i][j];	}
	}
//Nettoyage:
	for(uint8_t i = 0; i < 50; i++)
	{
		Uint8_Buffer0[i] = 0;
		Uint8_Buffer1[i] = 0;
	}
	return;
}

void PetitCool::removeSensor()
{	//	Revoir Exception Display -> Diminution	//
//Initialisation:
	for(uint8_t i = 0; i<Uint8_Buffer[0]-3; i++)	{	Uint8_Buffer1[i] = Uint8_Buffer[i+3];	}				//Recuperation des pins a retirer
	for(uint8_t i = 0; i < SENSOR_TYPE; i++)	{	for(uint8_t j = 0; j < 14; j++)	{	PinsCopy[i][j] = 0;	}}	//Reinitialisation Buffer Pin
//Retrait:
	for(uint8_t i = 0; i<SENSOR_TYPE; i++)
	{
	//Initialisation:
		uint8_t k = 0;
	//Mise a jour Nb Capteurs:
		if(i == Uint8_Buffer[1])	Uint8_Buffer0[i] = (Application.SensorsInfo).Type[i] - Uint8_Buffer[2];
		else if(i != Uint8_Buffer[1])	Uint8_Buffer0[i] = (Application.SensorsInfo).Type[i];
	//Mise a jour Pins:
		//Pin Simple:	
		if((i != 0)&&(i != 8))
		{
			for(uint8_t j = 0; j<Uint8_Buffer0[i]; j++)
			{
				for(uint8_t m = 0; m<Uint8_Buffer[0]-3; m++)	{	CommTest[0] = CommTest[0]||((Application.SensorsInfo).Pins[i][k] == Uint8_Buffer1[m]);	}
				if(CommTest[0])
				{
					k++;
					j--;
					CommTest[0] = false;
				}
				else
				{
					PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][k];
					CommTest[0] = false;
					k++;
				}
			}
		
		}
		//Pin Double:
		else if((i == 0)||(i == 10))
		{
			for(uint8_t j = 0; j<Uint8_Buffer0[i]; j = j + 2)
			{
				for(uint8_t m = 0; m<Uint8_Buffer[0]-3; m++)	{	CommTest[0] = CommTest[0]||((Application.SensorsInfo).Type[i] == Uint8_Buffer1[m]);	}
				if(CommTest[0])
				{
					k += 2;
					j -= 2;
					CommTest[0] = false;
				}
				else 
				{
					PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][k];
					PinsCopy[i][j+1] = (Application.SensorsInfo).Pins[i][k+1];
					k += 2;
				}
			}
		}
		//Exception Display:
		else if(i == 8)
		{
			switch((Application.SensorsInfo).Type[i])
			{
				case 1:
					for(uint8_t j = 0; j < 2; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
					break;
				case 2:
					for(uint8_t j = 0; j < 3; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
					break;
				case 3:
					for(uint8_t j = 0; j < 5; j++)	{	PinsCopy[i][j] = (Application.SensorsInfo).Pins[i][j];	}
					break;
				default:
					break;
			}
		}
	}
//Sauvegarde:
	for(uint8_t i = 0; i<SENSOR_TYPE; i++)
	{
		(Application.SensorsInfo).Type[i] = Uint8_Buffer0[i];
		for(uint8_t j = 0; j < 14; j++)	{	(Application.SensorsInfo).Pins[i][j] = PinsCopy[i][j];	}
	}
//Nettoyage:
	CommTest[0] = false;
	for(uint8_t i = 0; i < 50; i++)
	{
		Uint8_Buffer0[i] = 0;
		Uint8_Buffer1[i] = 0;
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////FONCTIONS BIOBOT/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PetitCool::getAllData()
{
		display();
	getDateTime();
		display();
	getLumValue();
		display();
	getDHTData();
		display();
	getMoistData();
		display();
	getWaterLevel();
		display();
	setDisplay();
		display();
}

void PetitCool::setDisplay()
{
//Donnees Capteurs:
	Display.setInfoFromSensor(0, (Application.AppData).ClockData[3]);	//Heures
	Display.setInfoFromSensor(1, (Application.AppData).ClockData[4]);	//Minutes
	Display.setInfoFromSensor(2, (Application.AppData).ClockData[5]);	//Secondes
	Display.setInfoFromSensor(3, Lums[0].getBVal());					//Luminosite Interieure
	Display.setInfoFromSensor(4, Lums[1].getBVal());					//Luminosite Exterieure
	Display.setInfoFromSensor(5, DHT[0].getTemperature());				//Temperature Interieure
	Display.setInfoFromSensor(6, DHT[1].getTemperature());				//Temperature Exterieure
	Display.setInfoFromSensor(7, DHT[0].getHumidity());					//Humidite Interieure
	Display.setInfoFromSensor(8, DHT[1].getHumidity());					//Humidite Exterieure
	Display.setInfoFromSensor(9, Moist[0].getVal());					//Moisture 1
	Display.setInfoFromSensor(10, Moist[0].getVal());					//Moisture 2
	Display.setInfoFromSensor(11, Moist[0].getVal());					//Moisture 3
	Display.setInfoFromSensor(12, WL[0].getVal_cm());					//WaterLevel
//Consignes:
	Display.setLevelFromMem(0, (Application.AppData).Consignes[6]);		//Temperature Desiree
	Display.setLevelFromMem(1, (Application.AppData).Consignes[7]);		//Tolerance Max Temperature Interieure
	Display.setLevelFromMem(2, (Application.AppData).Consignes[8]);		//Tolerance Min Temperature Interieure
	Display.setLevelFromMem(3, (Application.AppData).Consignes[9]);		//Humidite Desiree
	Display.setLevelFromMem(4, (Application.AppData).Consignes[10]);	//Tolerance Maximale Humidite Interieure
	Display.setLevelFromMem(5, (Application.AppData).Consignes[11]);	//Tolerance minimale Humidite Interieure
	Display.setLevelFromMem(6, (Application.AppData).Consignes[0]);		//Moisture 1
	Display.setLevelFromMem(7, (Application.AppData).Consignes[0]);		//Moisture 2
	Display.setLevelFromMem(8, (Application.AppData).Consignes[0]);		//Moisture 3
	
}

void PetitCool::controlAll()
{
		display();
	controlFan();
	controlPompe();
	controlLed();
		display();
}

void PetitCool::getDateTime()
{
	Clock.readNow();
	for(uint8_t i = 0; i < 6; i++)
	{
		(Application.AppData).ClockData[i] = Clock.getNow(i);
		display();
	}
}

void PetitCool::getLumValue()
{
	if((Application.SensorsInfo).Type[2] == 0) return;
	else
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[2]; i++)
		{
			Lums[i].readData();
			Lums[i].readRVal();
			Lums[i].readBrightness();
			display();
		}
	}
	return;
}

void PetitCool::getDHTData()
{
	
	if((Application.SensorsInfo).Type[3] == 0) return;
	else
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[3]; i++)
		{
			(Application.Timers).Sensor[0][i+7] = millis();
			if((Application.Timers).Sensor[0][i+7] - (Application.Timers).Sensor[0][i+14] > (Application.Timers).Sensor[0][i])
			{
				(Application.Timers).Sensor[0][i+14] = (Application.Timers).Sensor[0][i+7];
				DHT[i].readSensor();
			}
			display();
		}
	}
	return;
}

void PetitCool::getMoistData()
{
	if((Application.SensorsInfo).Type[6] == 0) return;
	else
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[6]; i++)
		{
			(Application.Timers).Sensor[2][i+3] = millis();
			if((Application.Timers).Sensor[2][i+3] - (Application.Timers).Sensor[0][i+6] > (Application.Timers).Sensor[2][i])
			{
				(Application.Timers).Sensor[2][i+6] = (Application.Timers).Sensor[2][i+3];
				Moist[i].readVal();
			}
			display();
		}
	}
	return;
}

	void PetitCool::getWaterLevel()
{	//	AJOUTER TIMERS	//
	if((Application.SensorsInfo).Type[9] == 0) return;
	else
	{
		for(uint8_t i = 0; i < (Application.SensorsInfo).Type[9]; i++)
		{
			//(Application.Timers).Sensor[3][i+7] = millis();
			//if((Application.Timers).Sensor[3][i+7] - (Application.Timers).Sensor[3][i+14] > (Application.Timers).Sensor[3][i])
			//{
				//(Application.Timers).Sensor[3][i+14] = (Application.Timers).Sensor[3][i+7];
				WL[i].ReadVal_cm();
				display();
			//}
		}
	}
	return;
}

	void PetitCool::controlPompe()
{	//	Inclure Mode Hydro	//
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[4]; i++)
	{
		if(Pompe[i].getMode() == 0)
		{
			Uint8_Buffer[0] = (Pompe[i].getPhase())?255:0;
			Pompe[i].setVal(Uint8_Buffer[0]);
		}
		else if(Pompe[i].getMode() == 1)
		{	
			Pompe[i].setVal(255);
		}
		display();
	}
//Nettoyage:
	Uint8_Buffer[0] = 0;
	return;
}

void PetitCool::controlFan()
{
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[5]; i++)
	{
		if(Fans[i].getMode() == 0)
		{
			Uint8_Buffer[0] = (Fans[i].getPhase())?255:0;
			Fans[i].setVal(Uint8_Buffer[0]);
		}
		else if(Fans[i].getMode() == 1)
		{	
			Fans[i].setVal(255);
		}
		display();
	}
//Nettoyage:
	Uint8_Buffer[0] = 0;
	return;
}

	void PetitCool::controlLed()
{	//	REVOIR OutOfHours	//
	for(uint8_t i = 0; i < (Application.SensorsInfo).Type[7]; i++)
	{
		if(Led[i].getMode() == 0)
		{
			Uint8_Buffer[0] = (Led[i].getPhase())?255:0;
			Led[i].setVal(Uint8_Buffer[0]);
		}
		else if(Led[i].getMode() == 1)
		{
			//Ecriture des conditions sur heures maximales:
			bool MaxHourSens = ((Application.AppData).ClockData[4] >= (Application.AppData).Consignes[5]);
			bool OutOfHours = (!MaxHourSens&&(((Application.AppData).ClockData[3] >= (Application.AppData).Consignes[5])||((Application.AppData).ClockData[3] <= (Application.AppData).Consignes[4])));
			OutOfHours = OutOfHours||(MaxHourSens&&(((Application.AppData).ClockData[3] >= (Application.AppData).Consignes[5])&&((Application.AppData).ClockData[3] <= (Application.AppData).Consignes[4])));
			if(OutOfHours) Uint8_Buffer[0] = 0;
			//Demonstrateur lie au capteur de luminosite et au mode saison:
			else
			{
				//Mode Personnalise:
				if((Application.SensorsInfo).Mode[0][0] == 4) Uint8_Buffer[0] = (Application.SensorsInfo).Mode[7][2*i+1];
				//Mode preenregistres:
				else Uint8_Buffer[0] = map(Lums[1].getBVal(), 0, LedSeasonPower[6*((Application.SensorsInfo).Mode[0][0]) + i] , 255, 0);
			}
			//Application des conditions:
			Led[i].setVal(Uint8_Buffer[0]);
		}
		display();
	}
//Nettoyage:
	Uint8_Buffer[0] = 0;
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////