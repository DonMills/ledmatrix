//////////////////
//wrappers
//////////////////////
void startISR(void);
void stopISR(void);
extern Matrix redMatrix;
extern Matrix grMatrix;

typedef struct spriStruct {
 uint8_t row[8];
};

typedef struct bufStruct {
 uint8_t buffer[8][8];
};
/*
void writeGreen(uint8_t x, uint8_t y, Sprite sprite)
{
 stopISR();
 grMatrix.write(x,y,sprite);
 startISR();
}
*/
void writeGreen(uint8_t x, uint8_t y, uint8_t value)
{
 stopISR();
 grMatrix.write(x,y,value);
 startISR();
}
void writeGreen(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
 stopISR();
 grMatrix.write(x,y,sprarray);
 startISR();
}
void writeRed(uint8_t x, uint8_t y, uint8_t value)
{
 stopISR();
 redMatrix.write(x,y,value);
 startISR();
}
/*
void writeRed(uint8_t x, uint8_t y, Sprite sprite)
{
 stopISR();
 redMatrix.write(x,y,sprite);
 startISR();
}
*/
void writeRed(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
 stopISR();
 redMatrix.write(x,y,sprarray);
 startISR();
}
void writeYellow(uint8_t x, uint8_t y, uint8_t value)
{
 stopISR();
 redMatrix.write(x,y,value);
 grMatrix.write(x,y,value);
 startISR();
}
void writeYellow(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
 stopISR();
 redMatrix.write(x,y,sprarray);
 grMatrix.write(x,y,sprarray);
 startISR();
}
/*
void writeYellow(uint8_t x, uint8_t y, Sprite sprite)
{
 stopISR();
 redMatrix.write(x,y,sprite);
 grMatrix.write(x,y,sprite);
 startISR();
}
*/
void writeInvYellow(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
    bufStruct inverseBuffer;
 for (uint8_t x=0;x<8;x++)
 {
     for (uint8_t y=0;y<8;y++)
     {
     inverseBuffer.buffer[x][y]=1-(sprarray[x][y]);
     }
 }  
 stopISR();
 grMatrix.write(x,y,inverseBuffer.buffer);
 redMatrix.write(x,y,inverseBuffer.buffer);
 startISR();
}
void writeInvRed(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
    bufStruct inverseBuffer;
 for (uint8_t x=0;x<8;x++)
 {
     for (uint8_t y=0;y<8;y++)
     {
     inverseBuffer.buffer[x][y]=1-(sprarray[x][y]);
     }
 }  
 stopISR();
 redMatrix.write(x,y,inverseBuffer.buffer);
 startISR();
}
void writeInvGreen(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
    bufStruct inverseBuffer;
 for (uint8_t x=0;x<8;x++)
 {
     for (uint8_t y=0;y<8;y++)
     {
     inverseBuffer.buffer[x][y]=1-(sprarray[x][y]);
     }
 }  
 stopISR();
 grMatrix.write(x,y,inverseBuffer.buffer);
 startISR();
}
void writeGreenonRed(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
bufStruct inverseBuffer;
 for (uint8_t x=0;x<8;x++)
 {
     for (uint8_t y=0;y<8;y++)
     {
     inverseBuffer.buffer[x][y]=1-(sprarray[x][y]);
     }
 }  
 stopISR();
 grMatrix.write(x,y,sprarray);
 redMatrix.write(x,y,inverseBuffer.buffer);
 startISR();
}
void writeRedonGreen(uint8_t x, uint8_t y, uint8_t sprarray[8][8])
{
bufStruct inverseBuffer;
 for (uint8_t x=0;x<8;x++)
 {
     for (uint8_t y=0;y<8;y++)
     {
     inverseBuffer.buffer[x][y]=1-(sprarray[x][y]);
     }
 }  
 stopISR();
 grMatrix.write(x,y,inverseBuffer.buffer);
 redMatrix.write(x,y,sprarray);
 startISR();
}
/*
void writeBoth(uint8_t x, uint8_t y, Sprite green, Sprite red)
{
 stopISR();
 redMatrix.write(x,y,red);
 grMatrix.write(x,y,green);
 startISR();
}
*/
void writeBoth(uint8_t x, uint8_t y, uint8_t sprarrayg[8][8], uint8_t sprarrayr[8][8])
{
 stopISR();
 grMatrix.write(x,y,sprarrayg);
 redMatrix.write(x,y,sprarrayr);
 startISR();
}

void clearGreen()
{
 stopISR();
 grMatrix.clear();
 startISR();
}
void clearRed()
{
 stopISR();
 redMatrix.clear();
 startISR();
}
void clearMatrix()
{
 stopISR();
 grMatrix.clear();
 redMatrix.clear();
 startISR();
}
