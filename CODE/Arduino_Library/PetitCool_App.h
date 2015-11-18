/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : INTELLIGENCE	-	MODEL : Le Petit Cool
	ROLE : Le Petit Cool Application Intelligence
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#ifndef PetitCool_App_h
#define PetitCool_App_h

//Inclusion des librairies
	#include "Arduino.h"
	#include <SoftwareSerial.h>
	#include <EEPROM.h>
	#include <SD.h>
	
	#include "PetitCool_Def.h"
	#include "PetitCool.h"
	
/*/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////	 
Data_Sensor->MemHere
	MemHere[0]		SD here - Use SD
	MemHere[1]		SD not here - Use EEPROM
Data_Sensor -> Type
	Type[0]				BlueTooth
	Type[1]				RTC
	Type[2]				Lumiere
	Type[3]				DHT
	Type[4]				Pompe
	Type[5]				Ventilateurs
	Type[6]				Hydratation
	Type[7]				Leds
	Type[8]				Display - {4-Digits (1), Button(2), LedBar(3)}
	Type[9]				Proximetre
	//Type[10]			Camera
Data_Sensor->Mode
	Mode[0]				XXX	
BTParameters->Params
	Params[0]			Mode de Connection
	Params[1]			Connexion Materiel Appaire Seulement
	Params[2]			Appairage automatique
	Params[3]			Mode de Reception
Data_Consignes->ClockData	
	ClockData[0]		Jour
	ClockData[1]		Mois
	ClockData[2]		Annee
	ClockData[3]		Heure
	ClockData[4]		Minute
	ClockData[5]		Seconde
Data_Consignes->Consignes	
	Consignes[0]		Hydratation du substrat desiree
	Consignes[1]		Puissance des Leds en %
	Consignes[2]		Parametre Led -> Width
	Consignes[3]		Parametre Led -> Offset
	Consignes[4]		Heure de debut de l eclairage
	Consignes[5]		Heure d arret de l eclairege
	Consignes[6]		Temperature desiree
	Consignes[7]		Temperature Maximale
	Consignes[8]		Temperature Minimale
	Consignes[9]		Humidite desiree
	Consignes[10]		Humidite Maximale
	Consignes[11]		Humidite Minimale
TicToc->General
	GeneralTimers[0]		Intervalle d'envoi de message
	GeneralTimers[1]		Intervalle d'ecriture des donnees sur SD
	GeneralTimers[2]		Intervalle de prise de vue
	GeneralTimers[3]		Courant Envoi message		
	GeneralTimers[4]		Courant Ecriture SD
	GeneralTimers[5]		Courant Prise de vue
	GeneralTimers[6]		Precedent Envoi Message
	GeneralTimers[7]		Precedent Ecriture SD
	GeneralTimers[8]		Precedent Prise de Vue
TicToc->Sensor[0]
	SensorTimers[0][0+i]	Mesure DHT[i]
	SensorTimers[0][7+i]	Courant DHT[i]
	SensorTimers[0][14+i]	Precedent DHT[i]
TicToc->Sensor[1]	
	SensorTimers[1][0+i]	ON Pompe[i]
	SensorTimers[1][3+i]	OFF Pompe[i]
	SensorTimers[1][6+i]	Mode Pompe[i]
	SensorTimers[1][9+i]	Courant Pompe[i]
	SensorTimers[1][12+i]	Precedent Pompe[i]
TicToc->Sensor[2]	
	SensorTimers[2][0+i]	Mesure Moist
	SensorTimers[2][3+i]	Courant Moist
	SensorTimers[2][6+i]	Precedent Moist
TicToc->Sensor[3]	
	SensorTimers[3][0+i]	Mesure WL[i]
	SensorTimers[3][7+i]	Courant WL[i]
	SensorTimers[3][14+i]	Precedent WL[i]
TicToc->Sensor[4]	
	SensorTimers[4][0+i]	Mesure Lums[i]
	SensorTimers[4][7+i]	Courant Lums[i]
	SensorTimers[4][14+i]	Precedent Lums[i]
/////////////////////////////////////////////////////////*/
/////////////////////////////////////////////////////////*/

	class PetitCool_App
	{
		public:						//Structures de donnees
			struct Data_Sensors
			{
				bool		MemHere[2];
				uint8_t		Type[SENSOR_TYPE];
				uint8_t		Pins[SENSOR_TYPE][14];
				uint8_t		Error[SENSOR_TYPE][7];
				uint8_t		Mode[SENSOR_TYPE][14];
			};
			struct BTParameters
			{
				char		Name[7];
				uint8_t		PinCode[4];
				uint8_t		Params[4];
			};
			struct Data_Consignes
			{
				uint8_t		ClockData[6];
				uint8_t		Consignes[NB_CONS];
				String		Order;
			};
			struct TicToc
			{
				unsigned long	General[9];
				unsigned long	Sensor[TIMERS_TYPE][21];
				unsigned long	Led[18][3];
			};
		public:						//Initialisation--A0
			PetitCool_App();
			uint8_t begin(bool SDMem, bool EEPPROMMem);
			inline void setMemHere(uint8_t MemType, bool here)	{	SensorsInfo.MemHere[MemType] = here;	}
			void InitiateBioApp();
			void CleanBuffer();
			public:					//Initialisation--A1
				void LoadFromEEPROM();
				void LoadFromSD();
				void defaultLoading();
				public:				//Initialisation--A2
					void AssignSensor();
					void AssignTimers();
					void AssignConsignes();
					void AssignModes();
					void getBTParam();
					void accessBT(SoftwareSerial* BlueTooth);
					public:			//Initialisation--A3
						void assignSinglePin(const uint8_t n);
						void assignDoublePin(const uint8_t n);
						void assignDisplay(const uint8_t n);
						void AssignTimersGeneral();
						void AssignTimersLed();
						void AssignTimersSensors();
						void readBTname();
						void readBTpin();
						void readBTparams();
						void readBTPinCode();
		public:						//Communication--A0
			void setDateFromExternal();
			void sendConfigToExternal(uint8_t* Config);
			bool getOrderFromExternal();
			public:					//Communication--A1
				void BeginComm(const String Requested, const String Accept);
				void EndComm();
				void CommBodyReceive(const uint8_t nb_do, const String SpecialChar, const String Requested, const String Accept);
				void CommBodySend(const String Requested, const String Accept);
				public:				//Communication--A2
					void CommReceiveData(const String SpecialChar, const uint8_t i);
					void CommReceiveNext(const uint8_t i, const String SpecialChar);
					void CommSendData();
					void CommEndLoop();
					public:			//Communication--A3
						void CommPrintDateTime();
						void CommSaveDateTime();
		private:
			SoftwareSerial*	BT;
		public:
			Data_Sensors	SensorsInfo;
			Data_Consignes	AppData;
			BTParameters	BTInfo;
			TicToc			Timers;
		private:
			bool			CommTest[4];
			uint8_t			Uint8_cmpt[2];
			uint8_t			Uint8_Buffer0_0[156];
			uint8_t			Uint8_Buffer0_1[156];
			String			StringBuffer0_0;
			String			StringBuffer0_1;
			String			StringBuffer0_2;
			String			StringBuffer0_3;
			String			StringBuffer0_4;
			String			StringBuffer1_0[6];
			String			StringBuffer1_1[6];
			String			StringBuffer2_0;
			String			StringBuffer2_1;
	};
#endif