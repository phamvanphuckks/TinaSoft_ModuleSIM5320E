#include "sim7600e.h"
#include <SoftwareSerial.h>

#define SIM7600E_PWRKEY 8
#define SIM7600E_RST 9
#define SIM7600E_RX 10
#define SIM7600E_TX 11
#define serialMON Serial

SoftwareSerial SIM7600E_SS = SoftwareSerial(SIM7600E_RX, SIM7600E_TX);

#if defined(ARDUINO_ARCH_SAMD)
#define DebugStream SERIAL_PORT_USBVIRTUAL 
#else
#define DebugStream Serial
#endif

#define DEBUG 

#ifdef DEBUG 
#define DEBUG_PRINT(...)    DebugStream.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...)  DebugStream.println(__VA_ARGS__)
#endif
/*
  buffer serial max la 64 byte
  __FlashStringHelper với tham số thì đối số truyền vào là F("")
*/

SIM7600E::SIM7600E()
{
  setUserAgent("Module SIM7600E");
  setHTTPSRedirect(0);
}

boolean SIM7600E::setupSIM(long baud)
{
  if(!setBaud(baud)) 
    return false; 
  if(!baudCheck())  
    return false;
  if(!setEcho(false))  
    return false;
  if(!getModemInfo()) 
    return false;
    
  return true;
}

void SIM7600E::rstSIM()
{
  serialMON.println(F("rst begin"));
  digitalWrite(SIM7600E_RST, LOW);
  delay(500);
  digitalWrite(SIM7600E_RST, HIGH);
  delay(500);
  serialMON.println("rst ok");
  delay(500);
}

boolean SIM7600E::getModemInfo()
{
  flushInput();
  uint16_t t=0;
  String txt;
#ifdef DEBUG 
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(F("ATI"));
#endif
  SIM7600E_SS.println(F("ATI\r\n"));
  readline(1000, true);
  if(strstr(replybuffer, ok_reply))
  {
    
#ifdef DEBUG  
  DEBUG_PRINTLN(replybuffer);
  DEBUG_PRINT(F("\tAVR<---SIM:\t"));
  DEBUG_PRINTLN(ok_reply);
  DEBUG_PRINTLN(F("\t----------------------------------"));
#endif
    return true;
  }
  else
    return false;
}

boolean SIM7600E::setEcho(boolean status)
{
  flushInput();
  String txt;
#ifdef DEBUG  
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINT(F("ATE"));
  DEBUG_PRINTLN(status);
#endif
  if(status==true) SIM7600E_SS.println(F("ATE1\r\n"));
  else  SIM7600E_SS.println(F("ATE0\r\n"));

  readline(1000, true);
  if(strstr(replybuffer, ok_reply))
  {
    strcpy(replybuffer, ok_reply);
#ifdef DEBUG  
  DEBUG_PRINT(F("\tAVR<---SIM:\t"));
  DEBUG_PRINTLN(replybuffer);
  DEBUG_PRINTLN(F("\t----------------------------------"));
#endif
    return true;
  }
  else
    return false;
}

boolean SIM7600E::setBaud(long baud)
{
  flushInput();
  for(int i=0; i<8; i++)
  {
    SIM7600E_SS.begin(baudrate_array[i]);
    SIM7600E_SS.println("AT\r\n");
    readline(1000);
    if(strcmp(replybuffer, ok_reply) == 0) break;;
    delay(100);
  }
  sprintf(tempdata,"AT+IPR=%d\r\n", baud);
#ifdef DEBUG  
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINT(F("AT+IPR="));
  DEBUG_PRINTLN(baud);
#endif
  SIM7600E_SS.println(tempdata); 
  readline(1000, true);
  if(strstr(replybuffer, ok_reply))
  {
    strcpy(replybuffer, ok_reply);
#ifdef DEBUG
  DEBUG_PRINT(F("\tAVR<---SIM:\t")); 
  DEBUG_PRINTLN(replybuffer);
  DEBUG_PRINTLN(F("\t----------------------------------"));
#endif   
    SIM7600E_SS.begin(baud);
    return true;
  }
  else
    return false;
}

boolean SIM7600E::baudCheck()
{
  flushInput();

#ifdef DEBUG  
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(F("AT+IPR?"));
#endif
  SIM7600E_SS.println("AT+IPR?\r\n");
  readline(1000, true);
  if(strstr(replybuffer, ok_reply))
  {
    strcpy(replybuffer, ok_reply);
#ifdef DEBUG  
  DEBUG_PRINT(F("\tAVR<---SIM:\t"));
  DEBUG_PRINTLN(replybuffer);
  DEBUG_PRINTLN(F("\t----------------------------------"));
#endif
    return true;
  }
  return false;
}

bool SIM7600E::pushData(const char *url, const char *data)
{
  SIM7600E_SS.println("AT+HTTPINIT\r\n");
  delay(1000);
  SIM7600E_SS.println("AT+HTTPPARA=\"URL\",\"http://admin.breedlife.com/api/v1/data\"\r\n");
  delay(1000);
  SIM7600E_SS.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
  delay(1000);
  SIM7600E_SS.println("AT+HTTPDATA=110,10000\r\n");
  delay(1000);
  SIM7600E_SS.println("{\"data\":[\"TR001\",[[5,5,5],[5,5,5],[5,5,5],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}\r\n");
  delay(1000);
  SIM7600E_SS.println("AT+HTTPACTION=1\r\n");
  delay(1000);
  SIM7600E_SS.println("AT+HTTPTERM");
  delay(1000);
  return true;
}

/********* HTTP LOW LEVEL FUNCTIONS  ************************************/

boolean SIM7600E::HTTP_init()
{
  flushInput();
#ifdef DEBUG
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN("AT+HTTPINIT");
#endif
  SIM7600E_SS.println("AT+HTTPINIT\r\n");
  
  return expectReply(ok_reply);
}

boolean SIM7600E::HTTP_term()
{
  flushInput();
#ifdef DEBUG
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN("AT+HTTPTERM");
#endif
  SIM7600E_SS.println("AT+HTTPTERM\r\n");
  
  return expectReply(ok_reply);
}

boolean SIM7600E::HTTP_para(const char* parameterTag, const char* parameterVaule)
{
  flushInput();
  
  sprintf(tempdata,"AT+HTTPPARA=\"%s\",\"%s\"\r\n", parameterTag, parameterVaule);
#ifdef DEBUG
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempdata);
#endif
  SIM7600E_SS.println(tempdata);
  
  return expectReply(ok_reply);
}

boolean SIM7600E::HTTP_data(const char* data, uint32_t maxTime)
{
  flushInput();

  sprintf(tempdata,"AT+HTTPDATA=%d,%d\r\n", strlen(data), maxTime);
#ifdef DEBUG
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempdata);
#endif
  SIM7600E_SS.println(tempdata);
   
  if(expectReply("DOWNLOAD")){ // wait for "DOWNLOAD"
    sprintf(tempdata,"\"%s\"\r\n", data);
#ifdef DEBUG
    DEBUG_PRINT("\tdata:");
    DEBUG_PRINTLN(tempdata);
#endif
    SIM7600E_SS.println(tempdata);
  }
  else
    return false;
  return expectReply(ok_reply);
}

boolean SIM7600E::HTTP_action(uint8_t method, int32_t timeout)
{
  flushInput();
  
  // Send request.
  sprintf(tempdata,"AT+HTTPACTION=%d",method);
#ifdef DEBUG
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempdata);
#endif
  SIM7600E_SS.println(tempdata);

  if(!expectReply(ok_reply)) return false;

  // Parse response status and size.
  readline(timeout);
   
  if (!parseReply("+HTTPACTION:", ','))
    return false;

  return true;
}

boolean SIM7600E::HTTP_readall(uint16_t datalen)
{
  sprintf(tempdata,"AT+HTTPREAD=0,%d",datalen);
  getReply(tempdata);
  return true;
}

/********* HTTP HIGH LEVEL FUNCTIONS ***************************/

boolean SIM7600E::HTTP_GET_start(const char* url, const char* postData)
{
  if (!HTTP_setup(url))
    return false;

  // HTTP GET
  if (!HTTP_action(HTTP_GET))
    return false;
#ifdef DEBUG
  DEBUG_PRINT(F("Status: "));
  DEBUG_PRINTLN(stt);
  DEBUG_PRINT(F("Len: "));
  DEBUG_PRINTLN(datalen);
#endif
  // HTTP response data
  if (!HTTP_readall(datalen))
    return false;

  return true;
}

void SIM7600E::HTTP_GET_end(void)
{
  HTTP_term();
}

boolean SIM7600E::HTTP_POST_start(const char* url, const char* postData)
{
  if (!HTTP_setup(url))
    return false;

  // HTTP POST data
  if(!HTTP_data(postData))
    return false;

  // HTTP POST
  if (!HTTP_action(HTTP_POST))
    return false;
#ifdef DEBUG
  DEBUG_PRINT(F("\tStatus: "));
  DEBUG_PRINTLN(stt);
  DEBUG_PRINT(F("\tLen: "));
  DEBUG_PRINTLN(datalen);
  DEBUG_PRINTLN("\t----------------------------------");
#endif  
  // HTTP response data
  if (!HTTP_readall(datalen))
    return false;

  return true;
}

void SIM7600E::HTTP_POST_end(void)
{
  HTTP_term();
}

void SIM7600E::setUserAgent(const char *useragent)
{
  this->useragent = useragent;
}

void SIM7600E::setHTTPSRedirect(boolean onoff)
{
  httpsredirect = onoff;
}

/********* HTTP verify responses message ****************************************/

boolean SIM7600E::HTTP_setup(char *url)
{
  // Handle any pending
  HTTP_term();
  delay(1000);
  // Initialize and set parameters
  if (!HTTP_init())
    return false;
  if (!HTTP_para("URL", url))
    return false;
  if (!HTTP_para("UA", useragent))
    return false;
  if (!HTTP_para("CONTENT", "application/json"))
    return false;

  // HTTPS redirect
  if (httpsredirect)
  {
    if (!HTTP_para("REDIR", 1))
      return false;
  }
  
  return true;
}

void SIM7600E::flushInput()
{
  // Read all available serial input to flush pending data.
  uint16_t timeoutloop = 0;
  while (timeoutloop++ < 40)
  {
    while (SIM7600E_SS.available())
    {
      SIM7600E_SS.read();
      timeoutloop = 0; // If char was received reset the timer
    }
    delay(1);
  }
}

uint16_t SIM7600E::readRaw(uint16_t b)
{
  uint16_t idx = 0;

  while (b && (idx < sizeof(replybuffer) - 1))
  {
    if (SIM7600E_SS.available())
    {
      replybuffer[idx] = SIM7600E_SS.read();
      idx++;
      b--;
    }
  }
  replybuffer[idx] = 0;

  return idx;
}

uint8_t SIM7600E::readline(uint16_t timeout, boolean multiline)
{
  uint16_t replyidx = 0;
  while (timeout--)
  {
    if (replyidx >= 254)
    {
#ifdef DEBUG
      DEBUG_PRINTLN(F("Over stack"));
#endif
      break;
    }
    while (SIM7600E_SS.available())
    {
      char c = SIM7600E_SS.read();
      if (c == '\r')
        continue;
      if (c == 0xA)
      {
        if (replyidx == 0) // the first 0x0A is ignored
          continue;
        if (!multiline)
        {
          timeout = 0; // the second 0x0A is the end of the line
          break;
        }
      }
      replybuffer[replyidx] = c;
#ifdef DEBUG
      //DEBUG_PRINT(c, HEX); DEBUG_PRINT("#"); DEBUG_PRINTLN(c);
#endif
      replyidx++;
    }

    if (timeout == 0)
    {
      //DEBUG_PRINTLN(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  replybuffer[replyidx] = 0; // null term
  return replyidx;
}

uint8_t SIM7600E::getReply(const char *send, uint16_t timeout)
{
  flushInput();
  
  sprintf(tempdata,"%s\r\n", send);
#ifdef DEBUG
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempdata);
#endif
  SIM7600E_SS.println(tempdata);
  uint8_t l = readline(timeout);
#ifdef DEBUG  
  DEBUG_PRINT(F("\tAVR<---SIM:\t"));
  DEBUG_PRINTLN(replybuffer);
  DEBUG_PRINTLN(F("\t----------------------------------"));
#endif
  return l;
}
boolean SIM7600E::sendCheckReply(const char* send, const char *reply, uint16_t timeout)
{
  if (!getReply(send, timeout))
    return false;
  return (strcmp(replybuffer, reply) == 0);
}

boolean SIM7600E::parseReply(const char* toreply, char divider) {
  char *pstatus, *pdatalen;
  char *p_rx_buffer = strchr(replybuffer, divider); 
  if (p_rx_buffer == 0)
      return 0;
  p_rx_buffer += sizeof(divider);

  pstatus = p_rx_buffer;
  p_rx_buffer = strchr(pstatus, ',');
 
  if (p_rx_buffer != NULL)
  {
    *p_rx_buffer = 0;
    stt  = atoi(pstatus);
  }
  
  pdatalen = p_rx_buffer + 1;
  if (p_rx_buffer != NULL)
  {
    *p_rx_buffer = 0;
    datalen = atoi(pdatalen);
  }
  return true;
}

boolean SIM7600E::expectReply(const char* reply, uint16_t timeout){
  readline(timeout);
#ifdef DEBUG 
  DEBUG_PRINT(F("\tAVR<---SIM:\t")); 
  DEBUG_PRINTLN(replybuffer);
  DEBUG_PRINTLN(F("\t----------------------------------"));
#endif
  return (strcmp(replybuffer, reply) == 0);
}
