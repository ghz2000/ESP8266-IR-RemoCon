#include <Arduino.h>

#ifndef __CirRemote__
#define __CirRemote__



class CirRemote
{

  struct irPalse{
/*    unsigned int career = 13;
    unsigned int data0H = 510;
    unsigned int data0L = 510;
    unsigned int data1H = 510;
    unsigned int data1L = 1560;
    unsigned int readerH = 7500;
    unsigned int readerL = 4300;
*/
    unsigned int career = 12;
    unsigned int data0H = 260;
    unsigned int data0L = 900;
    unsigned int data1H = 260;
    unsigned int data1L = 2100;
    unsigned int readerH = 0;
    unsigned int readerL = 0;

  
  };

private:

  irPalse irPalse;
  int irPort;

  void irOn(long us);
  void irOff(long us);
  void char2bin(uint8_t code[], char data[], int len);
  void sendIR(uint8_t code[], int len);

public:

  CirRemote(int irPort);
  void irCode(byte data);
  void irOut(char data[], int len);

  void irOut();

};


#endif __CiniParser__

