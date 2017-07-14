#include <EEPROM.h>
#include <Binary.h>
#include "C:\Documents and Settings\D\My Documents\Arduino\scrollkickass\letters.h"

const uint8_t * alphabet[] =  {_A,_B,_C,_D,_E,_F,_G,_H,_I,_J,_K,_L,_M,_N,_O,_P,_Q,_R,_S,_T,_U,_V,_w,_X,_Y,_Z,__,_exclaim,_heart,_Alien1,_Alien2,_Alien1b,_Alien3,_Alien3b,check4a, check4b, fillbox1,fillbox2,fillbox3,allfull,check2a,check2b,horzin1,horzin2,horzin3,fana1,fana2,fana3,fanb1,fanb2,fanb3};

int alphabetsize = sizeof(alphabet)/sizeof(int);
int promcounter = 0;
void setup()
{
 Serial.begin(9600);
 }

void loop()
{
 for (int i = 0;i < alphabetsize;i++){
   for (uint8_t y = 0;y<8;y++)
   {
     
     Serial.print("Sprite");
     Serial.print("\t");
     Serial.print(i,DEC);
     Serial.print("\t");
     Serial.print("Line \t");
     Serial.println(y,DEC);
     uint8_t lineval = alphabet[i][y];
     Serial.print("value = ");
     Serial.println(lineval,DEC);
     Serial.print("Writing to EEPRom Location: ");
     Serial.println(promcounter,DEC);
     EEPROM.write(promcounter,lineval);
     Serial.print("Written...");
     uint8_t promval = EEPROM.read(promcounter);
     Serial.println(promval,DEC);
     promcounter++;
     
   }
 }
Serial.println("ALL DONE!");
while (1){};


}
