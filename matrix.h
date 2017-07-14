*/

#ifndef Matrix_h
#define Matrix_h

#include <inttypes.h>

class Sprite;

class Matrix
{
 private:
   uint8_t _pinData;
   uint8_t _pinClock;
   uint8_t _pinLoad;

   uint8_t* _buffer;
   uint8_t _screens;
   uint8_t _maximumX;

   void putByte(uint8_t);
   
   void syncRow(uint8_t);

   void setScanLimit(uint8_t);

   void buffer(uint8_t, uint8_t, uint8_t);
 public:
   Matrix(uint8_t, uint8_t, uint8_t, uint8_t = 1);
   void setBrightness(uint8_t);
   void write(uint8_t, uint8_t, uint8_t);
   void write(uint8_t, uint8_t, Sprite);
   void write(uint8_t, uint8_t, uint8_t[8][8]);
   void clear(void);
   void setRegister(uint8_t, uint8_t);
};

#endif
