#ifndef __SIM7600E_H__
#define __SIM7600E_H__

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <NewSoftSerial.h>
#endif

#define TIMEOUT_MS 1000
#define ok_reply  "OK"

#define HTTP_GET   0
#define HTTP_POST  1
#define HTTP_HEAD  2

class SIM7600E{
  public:
    SIM7600E();  
    void rstSIM();
    boolean getModemInfo();
    boolean setupSIM(long baud);
    boolean pushData(const char* url, const char* data);
    boolean setEcho(boolean status);
    boolean baudCheck();
    boolean setBaud(long baud);

    // HTTP low level interface
    boolean HTTP_init();
    boolean HTTP_term();
    boolean HTTP_para(const char* parameterTag, const char* parameterVaule);
    boolean HTTP_data(const char* data, uint32_t maxTime=1000);
    boolean HTTP_action(uint8_t method, int32_t timeout=1000);
    boolean HTTP_readall(uint16_t datalen);
    
    // HTTP high level interface
    boolean HTTP_GET_start(const char* url, const char* postData);
    void    HTTP_GET_end(void);
    boolean HTTP_POST_start(const char* url, const char* postData);
    void    HTTP_POST_end(void);
    boolean HTTP_setup(char *url);
    
    // HTTP verify responses message
    void setHTTPSRedirect(boolean onoff);
    void setUserAgent(const char *useragent);
    void     flushInput();
    uint16_t readRaw(uint16_t b);
    uint8_t  readline(uint16_t timeout = TIMEOUT_MS, boolean multiline = false);
    uint8_t  getReply(const char *send, uint16_t timeout = TIMEOUT_MS);
    boolean  expectReply(const char* reply, uint16_t timeout = 1000);
    boolean  sendCheckReply(const char* send, const char *reply, uint16_t timeout = TIMEOUT_MS);
    boolean  parseReply(const char* toreply, char divider);
    
  public:
    uint16_t stt, datalen; 
    uint32_t baudrate_array[8]={2400, 4800, 9600, 19200, 38400, 57600, 74880, 115200};
  private:
    char replybuffer[255], tempdata[255];
    char *useragent;
    boolean httpsredirect;
    long BAUD;
};


#endif
