#ifndef __SIM7600E_H__
#define __SIM7600E_H__

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <NewSoftSerial.h>
#endif

static enum SIM7600E_RESULT{
    S_ERROR,
    S_OK,
    S_NORESPONE 
}SIM7600E_RESULT;

#define TIMEOUT_MS 500
#define ok_reply  "OK"

#define HTTP_GET   0
#define HTTP_POST  1
#define HTTP_HEAD  2

typedef enum SIM7600E_RESULT SIM7600E_FRESULT;

class SIM7600E{
  public:
    char replybuffer[255];
    char *useragent;
    boolean httpsredirect;
  public:
    SIM7600E(long baud);  
    void begin(long baud);
    void rstSIM();
    String getModemInfo();
    bool pushData(const char* url, const char* data);
    
    boolean baudCheck();
    boolean setBaud(long baud);

    // HTTP low level interface (maps directly to SIM800 commands).
    boolean HTTP_init();
    boolean HTTP_term();
    boolean HTTP_para(const char* parameterTag, const char* parameterVaule);
    boolean HTTP_data(const char* data, uint32_t maxTime=10000);
    boolean HTTP_action(uint8_t method, uint16_t *status, uint16_t *datalen, int32_t timeout = 10000);
    boolean HTTP_readall(uint16_t *datalen);
    boolean HTTP_ssl(boolean onoff);
    
    // HTTP high level interface (easier to use, less flexible).
    boolean HTTP_GET_start(char *url, uint16_t *status, uint16_t *datalen);
    void HTTP_GET_end(void);
    boolean HTTP_POST_start(char *url, const char* contenttype, const uint8_t *postdata, uint16_t postdatalen,  uint16_t *status, uint16_t *datalen);
    void HTTP_POST_end(void);

    // HTTP helpers to verify responses
    void setHTTPSRedirect(boolean onoff);
    void setUserAgent(const char *useragent);
       
    boolean HTTP_setup(char *url);
    
    void flushInput();
    uint16_t readRaw(uint16_t b);
    uint8_t readline(uint16_t timeout = TIMEOUT_MS, boolean multiline = false);
    uint8_t getReply(const char *send, uint16_t timeout = TIMEOUT_MS);
    
    boolean expectReply(const char* reply, uint16_t timeout = 10000);
    boolean sendCheckReply(const char* send, const char *reply, uint16_t timeout = TIMEOUT_MS);

    boolean parseReply(const char* toreply, uint16_t *v, char divider  = ',', uint8_t index=0);
    boolean parseReply(const char* toreply, char *v, char divider  = ',', uint8_t index=0);
  
  private:
    unsigned long nMillis, oMillis;
    long BAUD;
    void wRespon(unsigned long wkt);
    void waiting(unsigned long wkt);
};


#endif
