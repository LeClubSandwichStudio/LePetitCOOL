/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : INTELLIGENCE	-	MODEL : Le Petit Cool
	ROLE : Le Petit Cool Machine Intelligence
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef PetitCool_h
#define PetitCool_h

//Inclusion des librairies
	#include "Arduino.h"		//Librairie d'optimisation pour Arduino
	#include <Wire.h>			//Librairie pour communication multiple
	#include <SoftwareSerial.h>	//Librairie pour communication BlueTooth et Camera
	#include <SD.h>				//Librairie pour carte SD
	
	#include <string.h>

	#include "PetitCool_Def.h"	//Librairie de definition generale
	#include "PetitCool_App.h"	//Librairie lien avec application PetitCool
	#include "Cool_Clock.h"		//Librairie pour module temp reel
	#include "Cool_Lum.h"		//Librairie pour capteurs de luminosite
	#include "Cool_DHT.h"		//Librairie pour DHTs 11 ou 22
	#include "Cool_Pump.h"		//Librairie pour Pompes
	#include "Cool_Fan.h"		//Librairie pour ventilateurs
	#include "Cool_Moist.h"		//Librairie pour capteurs d hydratation
	#include "Cool_Led.h"		//Librairie pour leds
	#include "Cool_Display.h"	//Librairie pour : ledbar, 4-digit, bouton
	#include "Cool_Proxi.h"		//Librairie pour le niveau d'eau
	//#include "CoolCam.h"		//Librairie pour camera UART
	
	class PetitCool
	{
		public:						//Initialisation--A0
			PetitCool();
			void begin();
			void InitiatePetitCool();
			void WriteCompileSD();
			public:					//Initialisation--A1
				void InitSensors();
				void InitErrors();
				public:				//Initialisation--A2
					void FactoryInit();
					void printErrors();
					void printConfig();
					void Fireworks();
					public:			//Initialisation--A3
						void InitTYPE_0();
						void InitTYPE_1();
						void InitTYPE_2();
						void InitTYPE_3();
						void InitTYPE_4();
						void InitTYPE_5();
						void InitTYPE_6();
						void InitTYPE_7();
						void InitTYPE_8();
						void InitTYPE_9();
					public:	
						void InitERR_2();
						void InitERR_3();
						void InitERR_4();
						void InitERR_5();
						void InitERR_6();
						void InitERR_7();
						void InitERR_8();
						void InitERR_9();
						public:		//Initialisation--A4
							bool Init_1_Serial();
							void Init_1_Compile();
							void prepareBTString();
							void WriteConfigEEPROM();
							void WriteConfigSD();
		public:						//Communication--A0
			void CreateSensorList();
			void CreateSentencesToSend();
			void SendData();
			void ReceiveOrder();
			void MakeSensorList();
			void MakeTimersList();
			void MakeConsignesList();
			void MakeModeList();
			public:					//Communication--A1
				void OrderSensor();
				void parseOrder();
				void AddDateTimeToSentence();
				void AddType_2_ToSentence();
				void AddType_3_ToSentence();
				void AddType_4_ToSentence();
				void AddType_5_ToSentence();
				void AddType_6_ToSentence();
				void AddType_7_ToSentence();
				void AddType_8_ToSentence();
				void AddType_9_ToSentence();
		public:						//Gestion de capteurs
			void OrderManageSensor(const char Op);
			void addSensor();
			void removeSensor();
		public:						//Recuperation et Controle --A0
			void getAllData();
			void controlAll();
			void getDateTime();
			void setDisplay();
			inline void display()	{Display.ButtonControl();}
		public:
			void setTimers();
			void ModifyConsignesFromApp();
			void ActOnPump();
			void ActOnPumpAll();
			void ActOnFan();
			void ActOnFanAll();
			void ActOnLed();
			void ActOnLedAll();
			public:
				void getLumValue();
				void getDHTData();
				void getMoistData();
				void getWaterLevel();
			public:
				void controlFan();
				void controlPompe();
				void controlLed();
		private:					//Declaration des objets potentiels:
			SoftwareSerial*		BT;
			//SoftwareSerial*		Cam;
			PetitCool_App		Application;
			Cool_Clock			Clock;
			Cool_Lum 			Lums[7];
			Cool_DHT 			DHT[7];
			Cool_Pump			Pompe[3];
			Cool_Fan			Fans[7];
			Cool_Moist			Moist[3];
			Cool_Led			Led[18];
			Cool_Display		Display;
			Cool_Proxi			WL[7];
			File				FtoW;
		private:					//Declaration de la configuration PetitCool:
			bool			CommTest[4];
			uint8_t			SensorMemList[255];
			uint8_t			Uint8_cmpt[5];
			uint8_t			Uint8_Buffer[50];
			uint8_t			Uint8_Buffer0[50];
			uint8_t			Uint8_Buffer1[50];
			uint8_t			Uint8_Buffer2[50];
			uint8_t			PinsCopy[SENSOR_TYPE][14];
			String 			SentDataFormat;
			String 			SensorSentence;
			String			AppDataSentence;
			String			StringBuffer0_0;
			String			StringBuffer0_1;
			String			StringBuffer0_2;
			String			StringBuffer1_0;
			String			StringBuffer1_1;
			String			StringBuffer1_2;
			String			StringBuffer2;
			String			StringBuffer3;
			String			StringBuffer4;
	};
#endif