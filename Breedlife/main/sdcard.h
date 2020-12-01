#ifndef __SDCARD_H__
#define __SDCARD_H__

#include <SPI.h>
#include <SD.h>

#include <Arduino.h>
#include "AppDebug.h"

#define SD_ChipSelectPin 53 

#define FILE_WRITEF (O_WRITE | O_READ | O_CREAT) // enable using fseek()
#define FILE_APPEND ((FILE_WRITE) | O_APPEND)


class SDCard
{
  public:
    void floatParse(float x);
    
    void SD_Init(void);
    void SD_openFile(char *nameFile, byte method);
    void SD_closeFile(char *nameFile);
    void SD_writeFile(char *nameFile, char *bufw); // ghi vao so float => myFile.print();
    void SD_readFile(char *bufr, int len);
    void SD_readFile(float *x);
    void SD_readFile(char c, char *bufr, int len);
    char SD_readFile();
    void SD_removeFile(char *nameFile);
    void SD_removeDir(char *nameDir);
    void SD_makeDir(char *nameDir);
    int  SD_position(void);
    void SD_print(float x); // myFile.print(5.68664,4) ghi được số float vào thẻ nhớ.
    void SD_println(float x);
    void SD_seek(int pos);
    byte SD_peek(void);
    int SD_available();
    void SD_getNameFile(char *nameFile);
    
    byte readByte();
    short readShort();
    int readInt();
    long readLong();
    float readFloat();
  private:
    File myFile;
};

#endif
