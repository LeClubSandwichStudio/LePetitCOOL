#include <PetitCool.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

//Declaration des objets presents
  //Pour les capteurs presents en double:
  //  - Le 1er capteur declare est considere interieur
  //  - Le 2nd capteur decalre est considere exterieur
PetitCool myPetitCool;

//Initialisation
void setup()
{
  Serial.begin(9600);
  myPetitCool.begin();
}

//Boucle de Controle
void loop()
{
   myPetitCool.getAllData();
   myPetitCool.controlAll();
   myPetitCool.SendData();
}
