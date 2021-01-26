#include <Binary.h> 

#include <EEPROM.h>
#include <Matrix.h> 
//#include <Flash.h>
#include <avr/pgmspace.h>

#define _BV(bit) (1 << (bit))
//letter sprites and matrix writing functions

#include ".\matrixwrappers.h"
//color defines
#define RED 0
#define GREEN 1
#define ORANGE 2
#define GREEN_RED 3
#define RED_GREEN 4
#define INV_RED 5
#define INV_GREEN 6
#define INV_ORANGE 7

#define BRIGHTNESS 10
#define MAX_SPRITES 50
//set up the Matrix objects
Matrix redMatrix = Matrix(8, 6, 7); 
Matrix grMatrix = Matrix(5, 3, 4); 
//all this for the timer
boolean redActive=true;
#define REG_SHUTDOWN    0x0C
#define ISR_FREQ 249
unsigned long ISRTime; 
///variables for the cellular animation
const uint8_t numparticles = 4; 
int velx[numparticles]; 
int vely[numparticles]; 
long posx[numparticles]; 
long posy[numparticles]; 
uint8_t res = 7; 
int maxnum = 8<<res;
uint8_t maxspeed = 30; 
uint8_t row; 
uint8_t col; 
////variables for the life game
//uint8_t world[8][8][2];
uint8_t density = 50; // The density of the population on the the screen
bufStruct life1;
bufStruct life2;
#define XSIZE 8
#define YSIZE 8
//put the strings in progmem
prog_uchar message1[] PROGMEM = " TEXT SCROLLING!! ";
prog_uchar message2[] PROGMEM = " GAME OF LIFE!! ";
prog_uchar message3[] PROGMEM = " CELLULAR ANIMATION!! "; 
prog_uchar message4[] PROGMEM = " SPRITE ANIMATION!! "; 
prog_uchar message5[] PROGMEM = " EEPROM STORED FONT AND SPRITES!! ";
prog_uchar message6[] PROGMEM = " TWO COLOR ANIMATION!! "; 
prog_uchar alienrun[] PROGMEM =" bcbcbcbcbc aaaaa efefefefef ddddd cbcbcbcbcb fefefefe";
prog_uchar moreani[] PROGMEM = " ijkl ijkl ijkl lkji lkji lkji opql opql opql lqpo lqpo lqpo rstl rstl rstl uvwl uvwl uvwl uvwl ";
prog_uchar testanigr[] PROGMEM =" ghghghghghghghghghgh mnmnmnmnmnmn ijkl ijkl ijkl";
prog_uchar testanir[] PROGMEM =" hghghghghghghghghghg nmnmnmnmnmnm  ijk  ijk  ijk";




//timer functions
ISR(TIMER2_COMPA_vect) {  //This ISR toggles shutdown between the 2MAX7221's
 if(redActive==true){
   grMatrix.setRegister(0x0C, 0x00);  // The order here is critical - Shutdown first!
   redMatrix.setRegister(0x0C, 0x01);   // . . . Then restart the other.
   redActive=false;
 }
 else {
   redMatrix.setRegister(0x0C, 0x00);
   grMatrix.setRegister(0x0C, 0x01);
   redActive=true;
 }
}  
void setISRtimer(){  // setup ISR timer controling toggleing
 TCCR2A = 0x02;                        // WGM22=0 + WGM21=1 + WGM20=0 = Mode2 (CTC)
 TCCR2B = 0x05;                // CS22=1 + CS21=0 + CS20=1 = /128 prescaler (125kHz)
 TCNT2 = 0;                            // clear counter
 OCR2A = ISR_FREQ;                     // set TOP (divisor) - see define
}
void startISR(){  // Starts the ISR
 TCNT2 = 0;                            // clear counter (needed here also)
 TIMSK2|=(1<<OCIE2A);                  // set interrupts=enabled (calls ISR(TIMER2_COMPA_vect)
}
void stopISR(){    // Stops the ISR
 TIMSK2&=~(1<<OCIE2A);       // disable interrupts
} 
///////////////////
///more functions for writing to matrix
///////////

//////////////////////

bufStruct loadBuffer(const uint8_t (sprarray)[8]){
 static bufStruct result;
 for (uint8_t x = 0;x<8;x++)
 {
   for (uint8_t y = 0;y<8;y++)
   {
     result.buffer[x][y]=(((sprarray)[y] >>(7-x)) & B00000001);
     //buffer[x][y]=(*alphabet[currentChar])[x]<<8-y) & B00000001;
   }
   
 }
   return result;
}

spriStruct fetchSprite(uint8_t indexno){
 
 static spriStruct test;
 int start = indexno*8;
 for (int y=start, j = start+8 ; y < j  ; y++){
   
   test.row[y%8]=(uint8_t)EEPROM.read(y);
   
 }
return test;
}
void animateRun(prog_uchar spriteset[],int del, int color){
 int aniLength = strlen_P((prog_char*) spriteset);
 for (int index = 0;index<aniLength;index++)
 {
   int currentChar = getChar(pgm_read_byte_near(spriteset+index));
    spriStruct charSprite = fetchSprite(currentChar);
    bufStruct screen = loadBuffer(charSprite.row);
    runBuffer(screen.buffer, color);
   
     delay(del);
     clearMatrix();
 }
}

void animate2colorRun(prog_uchar spritesetg[],prog_uchar spritesetr[],int del){
 int aniLength = strlen_P((prog_char*)spritesetg);
 for (int index = 0;index<aniLength;index++)
 {
   uint8_t greenChar = getChar(pgm_read_byte_near(spritesetg+index));
   uint8_t redChar = getChar(pgm_read_byte_near(spritesetr+index));
    spriStruct charSpriteg = fetchSprite(greenChar);
    spriStruct charSpriter = fetchSprite(redChar);     
    bufStruct screeng = loadBuffer(charSpriteg.row);
    bufStruct screenr = loadBuffer(charSpriter.row);
    
    //runBuffer(screeng.buffer, GREEN);
    //runBuffer(screenr.buffer, RED);
   writeBoth(0,0,screeng.buffer, screenr.buffer);
     delay(del);
     clearMatrix();
 }
}

void slideText(prog_uchar message[], int del, int color){
   int phraseLength = strlen_P((prog_char*)message);
 //uint8_t buffer[8][8];
 
 
 for (int index = 0;index<phraseLength-1;index++)
 {
uint8_t currentChar = getChar(pgm_read_byte_near(message+index));
uint8_t nextChar = getChar(pgm_read_byte_near(message+index+1));
 
 spriStruct charSprite = fetchSprite(currentChar);
 spriStruct nextcharSprite = fetchSprite(nextChar);
 
 bufStruct screen = loadBuffer(charSprite.row);
 
 
// &buffer = loadBuffer(buffer,(alphabet[currentChar]));
 for (int l = 0;l<9;l++)
 {
   runBuffer(screen.buffer, color);
   int valuer=0;
   int value2r=0;
   delay(del);
   for (int y=0;y<8;y++)
   {
         for (int x=0;x<7;x++)
     {
       screen.buffer[x][y] = screen.buffer[x+1][y];
        }
   }
      for (int y=0;y<8;y++)
   {
     if (l==0){value2r=0;}
     else {value2r = (nextcharSprite.row[y]>>(8-(l-1)) & B00000001) ;}
      screen.buffer[7][y]=value2r;
   }
  }
 clearMatrix();
 }
 //free(screen);
}
/////and the writer for the scroll
void runBuffer(uint8_t buffer[8][8], int color){
  if (color==RED) 
     {
       writeRed(0,0,buffer);
     }
     else if (color == GREEN)
     {
       writeGreen(0,0,buffer);
     }
     else if (color == ORANGE)
     {
       writeYellow(0,0,buffer);
     }
     else if (color == GREEN_RED)
     {
       writeGreenonRed(0,0,buffer);
     }
     else if (color == RED_GREEN)
     {
       writeRedonGreen(0,0,buffer);
     }
     else if (color == INV_RED)
     {
       writeInvRed(0,0,buffer);
     }
     else if (color == INV_GREEN)
     {
       writeInvGreen(0,0,buffer);
     }
     else if (color == INV_ORANGE)
     {
       writeInvYellow(0,0,buffer);
     }
}

void writeValue(uint8_t x, uint8_t y, uint8_t value, int color){
  if (color==RED) 
     {
       writeRed(x,y,value);
     }
     else if (color == GREEN)
     {
       writeGreen(x,y,value);
     }
     else if (color == ORANGE)
     {
       writeYellow(x,y,value);
     }
     /*
     else if (color == GREEN_RED)
     {
       writeGreenonRed(x,y,value);
     }
     else if (color == RED_GREEN)
     {
       writeRedonGreen(x,y,value);
     }
     else if (color == INV_RED)
     {
       writeInvRed(x,y,value);
     }
     else if (color == INV_GREEN)
     {
       writeInvGreen(x,y,value);
     }
     else if (color == INV_ORANGE)
     {
       writeInvYellow(x,y,value);
     }*/
}
void cycleSprites(uint8_t start,uint8_t finish, int del, int color){
 if (finish > MAX_SPRITES) //if pointer size = int size
 {
   finish = MAX_SPRITES;
 }
 for (uint8_t x=start;x<finish+1;x++)
 {
   spriStruct sprite = fetchSprite(x);
   bufStruct screen = loadBuffer(sprite.row);
   runBuffer(screen.buffer, color);
   delay(del);
   clearMatrix();
 }
}

//////////////////////////////////////////////
//some screensaver type stuff
///////////////////////////////////////////////
void cell(int color, int reps)
{
for (int runs=0;runs<reps+1;runs++)
 { 
 clearMatrix(); // clear display 

 for( uint8_t i = 0; i<numparticles; i++)
 {

   row = posx[i]>>res;
   col = posy[i]>>res;
   writeValue(row, col, HIGH, color); 


   posx[i]+=velx[i]; 
   posy[i]+=vely[i]; 

   if(posx[i]<0) 
   {
     posx[i]=1;
     velx[i]=-velx[i]; 
   }
   else if(posx[i]>=maxnum)
   {
     posx[i] = maxnum+(maxnum-posx[i]); 
     velx[i]= -velx[i];
     vely[i] = vely[i]+random(-1,1); 

   if(vely[i]>maxspeed) vely[i] = maxspeed; 
     else if(vely[i]<-maxspeed) vely[i] = -maxspeed; 


   }

   if(posy[i]<0) 
   {
     posy[i]=1;
     vely[i]=-vely[i]; 
   }
   else if(posy[i]>=maxnum)
   {
     posy[i] = maxnum+(maxnum-posy[i]);  
     vely[i]=-vely[i];  
     velx[i] = velx[i]+random(-1,1); 
     if(velx[i]>maxspeed) velx[i] = maxspeed; 
     else if(velx[i]<-maxspeed) velx[i] = -maxspeed; 


   }

 }
 delay(1);
 }
clearMatrix(); 
}
/////////////////////
//Life
void lifeinit(){
 for (uint8_t i = 0; i < 8; i++) {
   for (uint8_t j = 0; j < 8; j++) {
     if (random(100) < density) {
       
       life1.buffer[i][j] = 1;
       life2.buffer[i][j]= life1.buffer[i][j];
     }
     else {
       life1.buffer[i][j] = 0;
     }
     life2.buffer[i][j] = 0;
   }
 }


}
uint8_t neighbours(uint8_t x, uint8_t y, uint8_t buffer [8][8]) {
return buffer[(x + 1) % XSIZE][y] +
        buffer[x][(y + 1) % YSIZE] +
        buffer[(x + XSIZE - 1) % XSIZE][y] +
        buffer[x][(y + YSIZE - 1) % YSIZE] +
        buffer[(x + 1) % XSIZE][(y + 1) % YSIZE] +
        buffer[(x + XSIZE - 1) % XSIZE][(y + 1) % YSIZE] +
        buffer[(x + XSIZE - 1) % XSIZE][(y + YSIZE - 1) % YSIZE] +
        buffer[(x + 1) % XSIZE][(y + YSIZE - 1) % YSIZE];
}

void Life(int color,int reps,int del){
 uint8_t repeats = 0;
 bufStruct last;
 for (int goes = 0;goes<reps+1;goes++)
 {
   
   
   
   //bufStruct screen = loadBuffer(life1.row);
   runBuffer(life1.buffer, color);
   delay(del);
   for (int x = 0; x < 8; x++) {
   for (int y = 0; y < 8; y++) {
     // Default is for cell to stay the same
     life2.buffer[x][y] = life1.buffer[x][y];
     int count = neighbours(x, y, life1.buffer);
     if (count == 3 && life1.buffer[x][y] == 0) {
       // A new cell is born
       life2.buffer[x][y] = 1;
       
       
     }
     if ((count < 2 || count > 3) && life1.buffer[x][y] == 1) {
       // Cell dies
       
       life2.buffer[x][y] = 0;
     }       
   }
 }

 // Copy next generation into place
 if (life1.buffer == life2.buffer) {
   repeats++;
 }
 
 for (int x = 0; x < 8; x++) {
   for (int y = 0; y < 8; y++) {
     life1.buffer[x][y] = life2.buffer[x][y];
     
 }
}

   if (repeats>10) { 
     lifeinit(); 
   repeats = 0;
 }

 }
}
////////////////////////////////////////////

uint8_t getChar(char character){
uint8_t returnValue = 0;
 if ((character >64) && (character < 91)) {
   returnValue = character-65;
 }
 else
 {
   switch(character){
   case '!': returnValue = 27;break; 
   case 'a': returnValue = 28; break; //heart
   case 'b': returnValue = 29; break; //alien1
   case 'c': returnValue = 31; break; //alien1b
   case 'd': returnValue = 30; break; //alien2
   case 'e': returnValue = 32; break; //alien3
   case 'f': returnValue = 33; break; //alien3b
   case 'g': returnValue = 34; break; //check4a
   case 'h': returnValue = 35; break; //check4b
   //fillbox1,fillbox2,fillbox3,allfull,check2a,check2b,horzin1,horzin2,horzin3,fana1,fana2,fana3,fanb1,fanb2,fanb3
   case 'i': returnValue = 36; break; //fillbox1
   case 'j': returnValue = 37; break; //fillbox2
   case 'k': returnValue = 38; break; //fillbox3
   case 'l': returnValue = 39; break; //allfull
   case 'm': returnValue = 40; break; //check2a
   case 'n': returnValue = 41; break; //check2b
   case 'o': returnValue = 42; break; //horzin1
   case 'p': returnValue = 43; break; //horzin2
   case 'q': returnValue = 44; break; //horzin2
   case 'r': returnValue = 45; break; //fana1
   case 's': returnValue = 46; break; //fana2
   case 't': returnValue = 47; break; //fana3
   case 'u': returnValue = 48; break; //fanb1
   case 'v': returnValue = 49; break; //fanb2
   case 'w': returnValue = 50; break; //fanb3
   case ' ': returnValue = 26; break; //
  }
   return returnValue;
} 
}
///////////////Setup
void setup() 
{ 
 setISRtimer();
 startISR();
 grMatrix.setBrightness(BRIGHTNESS);
 redMatrix.setBrightness(BRIGHTNESS);
 randomSeed(analogRead(5));// a random seed for the random start pattern

 //setup the cell ani
 for(int i = 0; i<numparticles; i++)
 {
   velx[i] = random(11)+5;
   vely[i] = random(11)+5;
   posx[i] = (i%2)<<res;
   posy[i] = (i/2)<<res;

 }
//setup the life
lifeinit();

 }
/////////////
void loop()
{
 slideText(message1,75,GREEN);
 delay(1000);
 slideText(message2,75,RED);
 delay(1000);
 Life(GREEN,100,750);
 delay(1000);
 slideText(message3,75,ORANGE);
 delay(1000);
 cell(RED,5000);
 delay(1000);
 /*slideText(message2,50,INV_GREEN);
 delay(1000);
 cell(ORANGE,2000);
 delay(1000);
 */
 slideText(message4,75,ORANGE);
 delay(1000);
 animateRun(alienrun,500,RED);
 delay(1000);
 
 slideText(message5,75,INV_RED);
 delay(1000);
 animateRun(moreani,500,GREEN_RED);
 delay(1000);
 
 animate2colorRun(testanigr,testanir,500);
 delay(1000);

 slideText(message6,75,RED_GREEN);
delay(1000); 
 cycleSprites(0,25,500,ORANGE);
 delay(1000);
 cycleSprites(26,51,500,RED);
 delay(1000);
}

 
