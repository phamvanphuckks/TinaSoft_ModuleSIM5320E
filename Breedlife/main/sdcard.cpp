#include "sdcard.h"

/*
 * Initializes the SD library and card.
*/
void SDCard::SD_Init(void)
{
  if (!SD.begin(SD_ChipSelectPin)) {
   ECHOLN("initialization failed! - kiem tra lai chan SD_ChipSelectPin - 53 on board, 19-on chip");
  }
  else
  {
    ECHOLN("initialization SD Card done");
  }
}

/*
 * Opens a file on the SD card. If the file is opened for writing, it will be created if it doesn't already exist 
 * (but the directory containing it must already exist).
 * Method: FILE_READ/FILE_WRITE
*/
void SDCard::SD_openFile(char *nameFile, byte method)
{
  this->myFile = SD.open(nameFile, method); 
  if (this->myFile) {
    ECHOLN("open file done.");
  } else {  // if the file didn’t open, print an error:
    ECHOLN("error opening file.");
  } 
}

/*
  Close the file, and ensure that any data written to it is physically saved to the SD card.
*/
void SDCard::SD_closeFile(char *nameFile)
{
  this->myFile.close();
  ECHOLN("close the file.");
}

/* Write data to the file.
 * bufw: the byte, char, or string (char *) to write
*/
void SDCard::SD_writeFile(char *nameFile, char *bufw)
{
  if(myFile.write(bufw, strlen(bufw))==0)
  {
    ECHOLN("Write to SD failed!");
  }
  myFile.flush();
}

/*
 * Read from the file.
 * Returns
 * The next byte (or character), or -1 if none is available.
*/
void SDCard::SD_readFile(char *bufr, int len)
{
 
  if(myFile.read(bufr, len)==NULL)
  {
    ECHOLN("read file error!"); 
  }
  bufr[len]='\0';
}

/*
 * Remove a file from the SD card.
*/
void SDCard::SD_removeFile(char *nameFile)
{
  if(!SD.remove(nameFile))
  {
    ECHOLN("remove File Failed!!");    
  }
}
/*
 * Remove a directory from the SD card. The directory must be empty.
*/
void SDCard::SD_removeDir(char *nameDir)
{
  if(!SD.rmdir(nameDir))
  {
    ECHOLN("remove Dirctory Failed!");    
  }
}

void SDCard::SD_makeDir(char *nameDir)
{
  if(!SD.mkdir(nameDir))
  {
    ECHOLN("make Dirctory Failed!");    
  }  
}
/*
 * get the current position within the file (i.e. the location to which the next byte will be read from or written to).
*/
int SDCard::SD_position(void)
{
  return myFile.position();
}

/*
 * Seek to a new position in the file, which must be between 0 and the size of the file (inclusive).
*/
void SDCard::SD_seek(int pos)
{
  if(!this->myFile.seek(pos))
  {
    ECHOLN("seek failed!");
  }
}

/*
 * Check if there are any bytes available for reading from the file.
 * Returns the number of bytes available (int)
*/
long SDCard::SD_available()
{
  return this->myFile.available();
}

/*
 * Returns the file name
*/
void SDCard::SD_getNameFile(char *nameFile)
{
  strcpy(nameFile, this->myFile.name());
}
