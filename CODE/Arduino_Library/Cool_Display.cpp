/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : ACTUATOR	-	MODEL : Petit Cool Display System
	ROLE : 	Le Petit Cool Display Management
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

#include "Arduino.h"
#include "PetitCool.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////ALPHABET ASCII//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
const static int ascii[] = {
	0,      // 32 (space)
	0,    // 33 !
	0,      // 34 "
	0,      // 35 #
	0,      // 36 0x
	0,      // 37 %
	0,      // 38 &
	0,      // 39 '
	0,      // 40 (
	0,      // 41 )
	0,      // 42 *
	0,      // 43 +
	0,      // 44 ,
	0,      // 45 -
	0,      // 46 .
	0,      // 47 
	0b00111111,    // 48  0
	0b00000110,    // 49 1
	0b01011011,    // 50 2
	0b01001111,    // 51 3
	0b01100110,    // 52 4
	0b01101101,    // 53 5
	0b01111101,    // 54 6
	0b00000111,    // 55 7
	0b01111111,    // 56 8
	0b01101111,    // 57 9
	0b00001000,      // 58 :
	0,      // 59 ;
	0,      // 60 <
	0,      // 61 =
	0,      // 62 >
	0,      // 63 ?
	0,      // 64 @
	0b01110111,    // 65 A
	0b01111100,    // 66 B
	0b00111001,    // 67 C
	0b00111111,    // 68 D
	0b01111001,    // 69 E
	0b01110001,    // 70 F
	0b00111101,    // 71 G 
	0b01110110,    // 72 H
	0b00110000,    // 73 I
	0b00001110,    // 74 J
	0b01110110,    // 75 K
	0b00111000,    // 76 L
	0b00110111,    // 77 M
	0b01010100,    // 78 N
	0b01011100,    // 79 O
	0b01110011,    // 80 P
	0b01100111,    // 81 Q
	0b01010000,    // 82 R
	0b01101101,    // 83 S
	0b01111000,    // 84 T
	0b00111110,    // 85 U
	0b00111110,    // 86 V
	0b01111110,    // 87 W
	0b01110110,    // 88 X
	0b01101110,    // 89 Y
	0b01011011,    // 90 Z
	0,      // 91 
	0,      // 92 
	0,      // 93 
	0,      // 94 
	0,      // 95 
	0,      // 96 
	0b01110111,     // 97 a
	0b01111100,     // 98 b
	0b01011000,     // 99 c
	0b01011110,     // 100 d
	0b01111001,     // 101 e
	0b01110001,     // 102 f
	0b00111101,     // 103 g
	0b01110100,     // 104 h
	0b00010000,     // 105 i
	0b00001110,     // 106 j
	0b01110110,     // 107 k
	0b00110000,     // 108 l
	0b00110111,     // 109 m
	0b01010100,     // 110 n
	0b01011100,     // 111 o
	0b01110011,     // 112 p
	0b01100111,     // 113 q
	0b01010000,     // 114 r
	0b01101101,     // 115 s
	0b01111000,     // 116 t
	0b00011100,     // 117 u
	0b00111110,     // 118 v
	0b01111110,     // 119 w
	0b01110110,     // 120 x
	0b01101110,     // 121 y
	0b01011011,     // 122 z
	0,      // 123 
	0,      // 124 
	0,      // 125 
	0,      // 126 
	0 };    // 127
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////CONSTANTES////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
static uint8_t TempInt[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii['T'],
	ascii['E'],
	ascii['M'],
	ascii['P'],
	ascii['E'],
	ascii['R'],
	ascii['A'],
	ascii['T'],
	ascii['U'],
	ascii['R'],
	ascii['E'],
	ascii[0],
	ascii['I'],
	ascii['N'],
	ascii['T'],
	ascii['E'],
	ascii['R'],
	ascii['I'],
	ascii['O'],
	ascii['R'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};
	
static uint8_t HumInt[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii['H'],
	ascii['U'],
	ascii['N'],
	ascii['I'],
	ascii['D'],
	ascii['I'],
	ascii['T'],
	ascii['Y'],
	ascii[0],
	ascii['I'],
	ascii['N'],
	ascii['T'],
	ascii['E'],
	ascii['R'],
	ascii['I'],
	ascii['O'],
	ascii['R'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};

static uint8_t LumInt[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii['L'],
	ascii['U'],
	ascii['N'],
	ascii['I'],
	ascii['N'],
	ascii['O'],
	ascii['S'],
	ascii['I'],
	ascii['T'],
	ascii['Y'],
	ascii[0],
	ascii['I'],
	ascii['N'],
	ascii['T'],
	ascii['E'],
	ascii['R'],
	ascii['I'],
	ascii['O'],
	ascii['R'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};
	
static uint8_t Moisture[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii['M'],
	ascii['O'],
	ascii['I'],
	ascii['S'],
	ascii['T'],
	ascii['U'],
	ascii['R'],
	ascii['E'],	
	ascii[0],
	ascii['S'],
	ascii['O'],
	ascii['I'],
	ascii['L'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};
	
static uint8_t TempExt[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],	
	ascii['T'],
	ascii['E'],
	ascii['M'],
	ascii['P'],
	ascii['E'],
	ascii['R'],
	ascii['A'],
	ascii['T'],
	ascii['U'],
	ascii['R'],
	ascii['E'],
	ascii[0],	
	ascii['E'],
	ascii['X'],
	ascii['T'],
	ascii['E'],
	ascii['R'],
	ascii['I'],
	ascii['O'],
	ascii['R'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};

static uint8_t HumExt[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii['H'],
	ascii['U'],
	ascii['N'],
	ascii['I'],
	ascii['D'],
	ascii['I'],
	ascii['T'],
	ascii['Y'],
	ascii[0],	
	ascii['E'],
	ascii['X'],
	ascii['T'],
	ascii['E'],
	ascii['R'],
	ascii['I'],
	ascii['O'],
	ascii['R'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};

static uint8_t LumExt[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii['L'],
	ascii['U'],
	ascii['N'],
	ascii['I'],
	ascii['N'],
	ascii['O'],
	ascii['S'],
	ascii['I'],
	ascii['T'],
	ascii['Y'],
	ascii[0],
	ascii['E'],
	ascii['X'],
	ascii['T'],
	ascii['E'],
	ascii['R'],
	ascii['I'],
	ascii['O'],
	ascii['R'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};

static uint8_t WatLev[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],	
	ascii['W'],
	ascii['A'],
	ascii['T'],
	ascii['E'],
	ascii['R'],
	ascii[0],
	ascii['L'],
	ascii['E'],
	ascii['V'],
	ascii['E'],
	ascii['L'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};
	
static uint8_t Hello[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[72],
	ascii[69],
	ascii[76],
	ascii[76],
	ascii[79],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};
	
static uint8_t Photo[] = {
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii['T'],
	ascii['A'],
	ascii['K'],
	ascii['I'],
	ascii['N'],
	ascii['G'],
	ascii[0],
	ascii['P'],
	ascii['I'],
	ascii['C'],
	ascii[0],
	ascii[0],
	ascii[0],
	ascii[0]
};

static int MessageSize[10] = {
	sizeof(TempInt)/sizeof(TempInt[0]),
	sizeof(HumInt)/sizeof(HumInt[0]),
	sizeof(LumInt)/sizeof(LumInt[0]),
	sizeof(Moisture)/sizeof(Moisture[0]),
	sizeof(TempExt)/sizeof(TempExt[0]),
	sizeof(HumExt)/sizeof(HumExt[0]),
	sizeof(LumExt)/sizeof(LumExt[0]),
	sizeof(WatLev)/sizeof(WatLev[0]),
	sizeof(Hello)/sizeof(Hello[0]),
	sizeof(Photo)/sizeof(Photo[0]),
};

static uint8_t ErrorMessage[4] = {ascii[69-32],ascii[82-32],ascii[82-32],ascii[0]};
static uint8_t FailMessage[4] = {ascii[70-32],ascii[65-32],ascii[73-32],ascii[76-32]};
static uint8_t WaitMessage[4] = {ascii[87-32], ascii[65-32], ascii[73-32], ascii[84-32]};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////CONSTRUCTEURS///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
Cool_Display::Cool_Display()
{}

void Cool_Display::begin(int DigClk, int DigDio, int LedClk, int LedDio, int Button)
{//OK//
//Initialisation generale:
	DigitClk = DigClk;
	DigitDio = DigDio;
	LedBarClk = LedClk;
	LedBarDio = LedDio;
	ButtonPin = Button;
//4-Digits:
	if(DigitClk != 0)
	{
		pinMode(DigitClk,OUTPUT);
		pinMode(DigitDio,OUTPUT);
		DigitNb = 4;
		StartDigit = 0;
		DigitBright = 0x0f;
	}
//LedBar:
	if(LedBarClk != 0)
	{
		LedBarDisplay = 0x00;
		pinMode(LedBarClk,OUTPUT);
		pinMode(LedBarDio,OUTPUT);
		for(int i = 0; i < 10; i++)	{	Led[i] = 0x00;	}
		for(int i = 0; i < 5; i++)	{	Quality[i] = 0x00;	}
	}
//Bouton:
	if(ButtonPin != 0)
	{
		pinMode(ButtonPin, INPUT);
		PushCounter = 0;
		PushResetTime = 0;
	}
//Fin Initialisation:
	testError(1);
	DigitDisplay(MessageSize[8],Hello);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////ERRORLEVEL///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int Cool_Display::errorlevel()
{	//OK//
	testError(0);
	return BioDispErrorLevel;
}

void Cool_Display::setError(int error)
{	//OK//
	BioDispErrorLevel = error;
}

void Cool_Display::testError(int i)
{	//EN COURS//
//Initialisation:
	bool Test;
	if(0) Test = false;
	else Test = true;
//Niveau d erreur:
	BioDispErrorLevel = (!Test)?1:0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////FONCTIONS 4-DIGIT///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool Cool_Display::DigitWriteByte(uint8_t Data)
{	//OK//
	if(DigitClk == 0) return false;
	uint8_t i;
	uint8_t ReadData;
	for(i=0;i<8;i++)        //sent 8bit data
	{
		digitalWrite(DigitClk,LOW);      
		if(Data & 0x01) digitalWrite(DigitDio,HIGH);
		else digitalWrite(DigitDio,LOW);
		Data >>= 1;      
		digitalWrite(DigitClk,HIGH);
	}  
	digitalWrite(DigitClk,LOW); //wait for the ACK
	digitalWrite(DigitDio,HIGH);
	digitalWrite(DigitClk,HIGH);     
	pinMode(DigitDio,INPUT);
	while(digitalRead(DigitDio))    
	{ 
		ReadData +=1;
		if(ReadData == 200)//
		{
		pinMode(DigitDio,OUTPUT);
		digitalWrite(DigitDio,LOW);
		ReadData =0;
		}
		pinMode(DigitDio,INPUT);
	}
	pinMode(DigitDio,OUTPUT);
}

	void Cool_Display::DigitWriteData(uint8_t Data[])
{//OK//
	if(DigitClk == 0) return;
	int8_t SegData[4];
	uint8_t i;
	for(i = 0;i < 4;i ++)
	{
		SegData[i] = Data[i];
	}
//Send start signal to 4-Digit:
	digitalWrite(DigitClk,HIGH);
	digitalWrite(DigitDio,HIGH);
	digitalWrite(DigitDio,LOW);
	digitalWrite(DigitClk,LOW);
//Write to 4-Digit:
	DigitWriteByte(COMM1);//
//Send stop signal to 4-Digit:
	digitalWrite(DigitClk,LOW);
	digitalWrite(DigitDio,LOW);
	digitalWrite(DigitClk,HIGH);
	digitalWrite(DigitDio,HIGH);
//send start signal to 4-Digit:
	digitalWrite(DigitClk,HIGH);
	digitalWrite(DigitDio,HIGH);
	digitalWrite(DigitDio,LOW);
	digitalWrite(DigitClk,LOW);
//Write to 4-Digit:
	DigitWriteByte(START);
	for(i=0;i < 4;i ++)	{	DigitWriteByte(SegData[i]);	}
//Send stop signal to 4-Digit:
	digitalWrite(DigitClk,LOW);
	digitalWrite(DigitDio,LOW);
	digitalWrite(DigitClk,HIGH);
	digitalWrite(DigitDio,HIGH);
//send start signal to 4-Digit:
	digitalWrite(DigitClk,HIGH);
	digitalWrite(DigitDio,HIGH);
	digitalWrite(DigitDio,LOW);
	digitalWrite(DigitClk,LOW);
//Write to 4-Digit:
	DigitWriteByte(CMDDS);
//Send stop signal to 4-Digit:
	digitalWrite(DigitClk,LOW);
	digitalWrite(DigitDio,LOW);
	digitalWrite(DigitClk,HIGH);
	digitalWrite(DigitDio,HIGH);
}

void Cool_Display::DigitDisplay(int Size, uint8_t Data[])
{
	if(DigitClk == 0) return;
	int i = 0;
	unsigned long tic = millis();
	while(i<Size-3)
	{
		uint8_t ToDisplay[]= {Data[i],Data[i+1],Data[i+2],Data[i+3]};
		if(millis()-tic>DisplaySpeed)
		{
			tic = millis();
			DigitWriteData(ToDisplay);
			++i;
		}
	}
}

void Cool_Display::DigitInfo(uint16_t Data)
{
	if(DigitClk == 0) return;
	uint16_t k = 1000;
	uint8_t Segment[4];
	if(Data == (uint16_t)(-1))
	{
		for(int i = 0; i < 4; i++)
		{
			Segment[i] = ErrorMessage[i];
		}	
	}
	
	else
	{
		for(int i = 0; i < 4 ; i++)
		{
			int Temp = (int)(Data/k);
			Segment[i] = ascii[(48-32)+Temp];
			Data -= Temp*k;
			k = k/10;
		}
	}
	//Affichage:
	DigitWriteData(Segment);
	//unsigned long tic = millis();
	//while(millis() - tic < DataDisplayTime)	{	DigitWriteData(Segment);	}
}

void Cool_Display::setDigitBright(uint8_t bright)
{
	if(DigitClk == 0) return;
	DigitBright = bright;
}

void Cool_Display::DispInfo_4Digit(int Size, uint8_t Message[], uint16_t Data)
{
	setDigitBright(0x00);
	setDigitBright(0x0f);
	DigitDisplay(Size, Message);
	DigitInfo(SensorInfo[Data]);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////FONCTIONS LED BAR///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Cool_Display::SendBarData(unsigned int Data)
{	//OK//
	for(int i=0; i<16; i++)
    {
        uint16_t Status = Data&0x8000 ? HIGH : LOW;
        digitalWrite(LedBarDio,Status);
        Status = digitalRead(LedBarClk) ? LOW : HIGH;
        digitalWrite(LedBarClk,Status);
        Data <<= 1;
    }
}

void Cool_Display::LedBarLightSet(uint8_t Set, bool RightToLeft)
{
	float SetBright = Set/10;
	SetBright *= 8;
	for (uint8_t i = 0; i < 10; i++)
	{
		if(RightToLeft)
		{
			Led[i] = (SetBright > 8) ? ~0 :
			(SetBright > 0) ? ~(~0 << byte(SetBright)) : 0;
			SetBright -= 8;
		}
		else
		{
			Led[10-i-1] = (SetBright > 8) ? ~0 :
			(SetBright > 0) ? ~(~0 << byte(SetBright)) : 0;
			SetBright -= 8;
		}
	}
	SendBarData(CMDMODE);
	for (uint8_t i = 0; i < 10; i++)	{	SendBarData(Led[i]);	}
	SendBarData(0x00);
	SendBarData(0x00);
	digitalWrite(LedBarDio, LOW);
	delayMicroseconds(10);
	for (unsigned char i = 0; i < 4; i++)
	{
		digitalWrite(LedBarDio, HIGH);
		digitalWrite(LedBarDio, LOW);
	}
}

void Cool_Display::LedBarLightLed(uint8_t LedIndex, bool RightToLeft)
{
 	Led[LedIndex] = ~(~0 << (unsigned char) (8));
	SendBarData(CMDMODE);
	for (unsigned char i = 0; i < 10; i++)
	{
		if(RightToLeft) SendBarData(Led[i]);
		else SendBarData(Led[10-i-1]);
    }
	SendBarData(0x00);
	SendBarData(0x00);
	digitalWrite(LedBarDio, LOW);
	delayMicroseconds(10);
	for (unsigned char i = 0; i < 4; i++)
	{
		digitalWrite(LedBarDio, HIGH);
		digitalWrite(LedBarDio, LOW);
	}
}

void Cool_Display::LedBarBlinkLevel(uint8_t LedIndex, bool Warning)
{
	unsigned long tic = millis();
	for(uint8_t i = 0; i < 5; i++)
	{
		while(millis() - tic < uint8_t(DataDisplayTime/10))
		{
			if(!Warning)
			{	
				LedBarLightLed(0);
				for(uint8_t j = 2; j < 10; j++)	{	LedBarLightLed(j);	}
			}
			else
			{
				for(uint8_t j = 2; j < 10; j++)	{	LedBarLightLed(j);	}
			}
			
		}
		tic = millis();
		LedBarLightSet(0);
		while(millis() - tic < uint8_t(DataDisplayTime/10))
		{
			for(uint8_t i = 2; i < 10; i++)	{	if(i != LedIndex) LedBarLightLed(i);	}
		}
		tic = millis();
	}
	LedBarLightSet(0);
}

void Cool_Display::BarQualityControl(uint8_t Data)
{//OK//
//Chaque valeur affiche par le 4-Digit est compare a sa consigne
	if(LedBarClk == 0) return;
	uint8_t LedLevel = 0;
	bool Mod = false;
	switch(Data)
	{
		case 5:					//Temperature Interieure
			//Mappage de la valeur entre les tolerances minimale et maximale
			if(SensorInfo[Data] == uint8_t(-1))
			{
				LedLevel = -1;
				break;
			}
			LedLevel = map(SensorInfo[Data], Quality[2], Quality[1], 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break;
		/* case 6:					//Temperature Exterieure
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], Quality[2], Quality[1], 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break; */
		case 7:					//Humidite Inteurieure
			//Mappage de la valeur entre les tolerances minimale et maximale	
			if(SensorInfo[Data] == -1)
			{
				LedBarBlinkLevel(1, true);
				return;
			}
			LedLevel = map(SensorInfo[Data], Quality[11], Quality[10], 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break;
		/* case 8:					//Humidite Exterieure
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], Quality[11], Quality[10], 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break; */
		case 3:					//Luminosite Interieure
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], 0, 100, 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break;
		/* case 4:					//Luminosite Exterieure
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], 0, 1023, 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break; */
		case 9:					//Moisture 1
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], 0, 100, 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break;
		/* case 10:					//Moisture 2
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], 0, 100, 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break;
		case 11:				//Moisture 3
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], 0, 100, 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break; */
		/* case 12:				//WaterLevel
			//Mappage de la valeur entre les tolerances minimale et maximale	
			LedLevel = map(SensorInfo[Data], 0, 5, 0 , 8);
			//Depassement de la tolerance Maximale
			if(LedLevel > 8)
			{
				LedLevel = 8;
				Mod = true;
			}
			//Depassement de la tolerance minimale
			else if(LedLevel < 0)
			{
				LedLevel = 0;
				Mod = true;
			}
			break; */
		default:
			LedLevel = 0;
			break;
	}
	for(uint8_t i = 2; i < 10; i++)	{	LedBarLightLed(i);	}
	LedLevel += 2;
	LedBarBlinkLevel(LedLevel, Mod);
}

void Cool_Display::BarTakePhoto()
{
	uint8_t i = 0;
	unsigned long tic = millis();
	while(i < 10)
	{
		while(millis() - tic < uint8_t(DataDisplayTime/10))	{	LedBarLightSet(100-10*i);	}
		tic = millis();
		i++;
	}
	tic = millis();
	for(uint8_t i = 0; i < 2*5; i++)
	{
		while(millis() - tic < uint8_t(DataDisplayTime/5)) {	LedBarLightSet(100*((i+1)%2));	}
		tic = millis();	
	}
	LedBarLightSet(0);
}

void Cool_Display::BarSecondDisplay()
{
	LedBarLightSet(0);
	LedBarLightLed(SensorInfo[2]/6, false);
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////FONCTIONS DISPLAY///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void Cool_Display::ButtonControl()
{//??	A Revoir en Interrupt	??//
	
	if(ButtonPin == 0) return;	
	PushCounter = 0;
	if(digitalRead(ButtonPin) == HIGH)
	{
		LedBarLightSet(0);
		PushCounter++;
		PushResetTime = millis();
		while(millis() - PushResetTime < PushPicture)
		{
			if(digitalRead(ButtonPin) == HIGH) PushCounter++;
			if((PushCounter >= PushDispInfo)&&(PushCounter <= 2*PushDispInfo)) LedBarLightSet(20);
			else if((PushCounter > 2*PushDispInfo)&&(PushCounter <= 3*PushDispInfo)) LedBarLightSet(40);
			else if((PushCounter > 3*PushDispInfo)&&(PushCounter <= 4*PushDispInfo)) LedBarLightSet(60);
			else if((PushCounter > 4*PushDispInfo)&&(PushCounter <= 5*PushDispInfo)) LedBarLightSet(80);
			else if(PushCounter > PushDispPic) LedBarLightSet(100);
		}
		ButtonAction(PushCounter);
		return;
	}
	else if(millis()-PushResetTime>DisplayTimerOff)
	{
		PushCounter  = 0;
		PushResetTime = millis();
		ButtonAction(PushCounter);
		return;
	}
}

void Cool_Display::ButtonAction(int PushCounter)
{//OK//
//Si le bouton est actionne par une pression courte, le defilement
//	des differentes informations est active.
//Si le bouton est actionne par une pression prolongee, la camera
//	est activee et declanche la prise d'une photo ainsi que son 
//	enregistrement sur la carte micro-SD
//Si le bouton n'est pas active, l'heure est affichee (4-Digit)
//	ainsi que le niveau d'eau restant (LedBar).

	if(PushCounter == 0)
	{//Affichage de l'heure et du niveau d'eau:
		BarSecondDisplay();
		setDigitBright(0x0f);
		if((SensorInfo[0] >= 25)) DigitWriteData(FailMessage);
		else DigitInfo(100*SensorInfo[0]+SensorInfo[1]);
	}
	else if(PushCounter <= PushDispPic)
	{//Affichage du defilement des donnees:
		PushCounter = 0;
		LedBarLightSet(0);
		//Temperature Interieure:
		DispInfo_4Digit(MessageSize[0], TempInt, 5);
		BarQualityControl(4);
		//Humidite Inteurieure:
		DispInfo_4Digit(MessageSize[1], HumInt, 7);
		BarQualityControl(7);
		//Luminosite Interieure:
		DispInfo_4Digit(MessageSize[2], LumInt, 3);
		BarQualityControl(3);
		//Moisture:
		DispInfo_4Digit(MessageSize[3], Moisture, 9);
		BarQualityControl(9);
		/* DigitInfo(SensorInfo[10]);
		BarQualityControl(10);
		DigitInfo(SensorInfo[11]);
		BarQualityControl(11);
		setDigitBright(0x00); */
		//Temperature Exterieure:
		/* DispInfo_4Digit(MessageSize[4], TempExt, 6);
		BarQualityControl(6); */
		//Humidite Exterieure:
		/* DispInfo_4Digit(MessageSize[5], HumExt, 8);
		BarQualityControl(8); */
		//Luminosite Exterieure:
		/* DispInfo_4Digit(MessageSize[6], LumExt, 4);
		BarQualityControl(4); */
		//Niveau d'eau:
		/* DispInfo_4Digit(MessageSize[7], WatLev, 12);
		BarQualityControl(12); */
	}
	else if(PushCounter >= PushDispPic)
	{//Prise de Photo:
		setDigitBright(0x0f);
		DigitDisplay(MessageSize[9],Photo);
		DigitWriteData(WaitMessage);
//	->	Fonction de prise de photo.
		BarTakePhoto();
		PushCounter = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////