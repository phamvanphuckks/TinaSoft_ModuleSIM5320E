#ifndef __SDCARD_H__
#define __SDCARD_H__

#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include "AppDebug.h"

// 53 la tren board - khi dung chip atmega2560 la 19
#define SD_ChipSelectPin 53 

class SDCard
{
  public:
    void floatParse(float x);
    
    void SD_Init(void);
    void SD_openFile(char *nameFile, byte method);
    void SD_closeFile(char *nameFile);
    void SD_writeFile(char *nameFile, char *bufw);
    void SD_readFile(char *bufr, int len);
    void SD_removeFile(char *nameFile);
    void SD_removeDir(char *nameDir);
    void SD_makeDir(char *nameDir);
    int  SD_position(void);
    void SD_print(); // myFile.print(5.68664,4) ghi duoc so float
    void SD_println();
    void SD_seek(int pos);
    long SD_available();
    void SD_getNameFile(char *nameFile);
  private:
    File myFile;
};

#endif
