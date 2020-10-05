#include "sim7600e.h"
#include <SoftwareSerial.h>

#define SIM7600E_PWRKEY 8
#define SIM7600E_RST 9
#define SIM7600E_RX 10
#define SIM7600E_TX 11
#define serialMON Serial

SoftwareSerial SIM7600E_SS = SoftwareSerial(SIM7600E_RX, SIM7600E_TX);

#if defined(ARDUINO_ARCH_SAMD)
#define DebugStream SERIAL_PORT_USBVIRTUAL // Needed for SAMD21
#else
#define DebugStream Serial
#endif

// debug
#define DEBUG_PRINT(...) DebugStream.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) DebugStream.println(__VA_ARGS__)

/*
  buffer serial max la 64 byte
  __FlashStringHelper với tham số thì đối số truyền vào là F("")
*/

SIM7600E::SIM7600E(long baud)
{
  begin(baud);
}

void SIM7600E::begin(long baud)
{
  BAUD = baud;
  SIM7600E_SS.begin(BAUD);
}

String SIM7600E::getModemInfo()
{
  int idex = 0;
  String txt;
  serialMON.println(F("ATI\r\n"));
  SIM7600E_SS.println(F("ATE0\r\n"));
  while (!SIM7600E_SS.available())
  {
  }
  while (SIM7600E_SS.available())
  {
    txt = SIM7600E_SS.readString();
  }
  return txt;
}

boolean SIM7600E::setBaud(long baud)
{
  BAUD=baud;
  SIM7600E_SS.println(F("AT+IPR=9600\r\n")); 
  delay(100);
  SIM7600E_SS.begin(BAUD);
}

void SIM7600E::rstSIM()
{
  String answer;
  char buf[100];
  int len;
  serialMON.println(F("rst begin"));
  digitalWrite(SIM7600E_RST, LOW);
  delay(1000);
  digitalWrite(SIM7600E_RST, HIGH);
  delay(500);
  serialMON.println("rst ok");
  delay(500);
}

boolean SIM7600E::baudCheck()
{
  delay(1000);
  SIM7600E_SS.println("AT+IPREX=?");
  wRespon(2000);
  delay(1000);
  SIM7600E_SS.println("AT+IPREX?");
  wRespon(2000);
  SIM7600E_SS.println("AT+IPR?");
  wRespon(2000);
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
  return sendCheckReply("AT+HTTPINIT", ok_reply);
}

boolean SIM7600E::HTTP_term()
{
  return sendCheckReply("AT+HTTPTERM", ok_reply);
}

boolean SIM7600E::HTTP_para(const char* parameterTag, const char* parameterVaule)
{
  char *tempData;
  flushInput();
  
  sprintf(tempData,"AT+HTTPPARA=\"%s\",\"%s\"\r\n", parameterTag, parameterVaule);
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempData);

  SIM7600E_SS.println(tempData);
  
  return expectReply(ok_reply);
}

boolean SIM7600E::HTTP_data(const char* data, uint32_t maxTime)
{
  char *tempData;
  flushInput();

  sprintf(tempData,"AT+HTTPDATA=%s,%d\r\n", strlen(data), maxTime);
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempData);

  SIM7600E_SS.println(tempData);
  
  if(expectReply("DOWNLOAD")){ // wait for "DOWNLOAD"
    sprintf(tempData,"\"%s\"\r\n", data);
    DEBUG_PRINTLN(tempData);
    SIM7600E_SS.println(tempData);
    delay(100);
  }
  return expectReply(ok_reply);
}

boolean SIM7600E::HTTP_action(uint8_t method, uint16_t *status, uint16_t *datalen, int32_t timeout)
{
  char *tempData;
  flushInput();
  
  // Send request.
  sprintf(tempData,"AT+HTTPACTION=%d",method);
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempData);
  SIM7600E_SS.println(tempData);
  
//  if (!sendCheckReply("AT+HTTPACTION=", method, ok_reply))
//    return false;

  // Parse response status and size.
  readline(timeout);
  if (!parseReply("+HTTPACTION:", status, ',', 1))
    return false;
  if (!parseReply("+HTTPACTION:", datalen, ',', 2))
    return false;

  return true;
}

boolean SIM7600E::HTTP_readall(uint16_t *datalen)
{
  getReply("AT+HTTPREAD");
  if (!parseReply("+HTTPREAD:", datalen, ',', 0))
    return false;

  return true;
}

boolean SIM7600E::HTTP_ssl(boolean onoff)
{
  return sendCheckReply("AT+HTTPSSL=", onoff ? 1 : 0, ok_reply);
}

/********* HTTP HIGH LEVEL FUNCTIONS ***************************/

boolean SIM7600E::HTTP_GET_start(char *url, uint16_t *status, uint16_t *datalen)
{
  if (!HTTP_setup(url))
    return false;

  // HTTP GET
  if (!HTTP_action(HTTP_GET, status, datalen, 30000))
    return false;

  DEBUG_PRINT(F("Status: "));
  DEBUG_PRINTLN(*status);
  DEBUG_PRINT(F("Len: "));
  DEBUG_PRINTLN(*datalen);

  // HTTP response data
  if (!HTTP_readall(datalen))
    return false;

  return true;
}

void SIM7600E::HTTP_GET_end(void)
{
  HTTP_term();
}

boolean SIM7600E::HTTP_POST_start(char *url,
                                  const char *contenttype,
                                  const uint8_t *postdata, uint16_t postdatalen,
                                  uint16_t *status, uint16_t *datalen)
{
  if (!HTTP_setup(url))
    return false;

  if (!HTTP_para("CONTENT", contenttype))
  {
    return false;
  }

  // HTTP POST data
  if (!HTTP_data(postdatalen, 10000))
    return false;
  SIM7600E_SS.write(postdata, postdatalen);
  if (!expectReply(ok_reply))
    return false;

  // HTTP POST
  if (!HTTP_action(HTTP_POST, status, datalen))
    return false;

  DEBUG_PRINT(F("Status: "));
  DEBUG_PRINTLN(*status);
  DEBUG_PRINT(F("Len: "));
  DEBUG_PRINTLN(*datalen);

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

/********* HTTP HELPERS ****************************************/
boolean SIM7600E::HTTP_setup(char *url)
{
  // Handle any pending
  HTTP_term();

  // Initialize and set parameters
  if (!HTTP_init())
    return false;
  if (!HTTP_para("CID", 1))
    return false;
  if (!HTTP_para("UA", useragent))
    return false;
  if (!HTTP_para("URL", url))
    return false;

  // HTTPS redirect
  if (httpsredirect)
  {
    if (!HTTP_para("REDIR", 1))
      return false;

    if (!HTTP_ssl(true))
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
      //DEBUG_PRINTLN(F("SPACE"));
      break;
    }
    while (SIM7600E_SS.available())
    {
      char c = SIM7600E_SS.read();
      if (c == '\r' || c=='\n')
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
      DEBUG_PRINT(c, HEX); DEBUG_PRINT("#"); DEBUG_PRINTLN(c);
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
  char *tempData;
  flushInput();

  sprintf(tempData,"%s\r\n", send);
  DEBUG_PRINT(F("\tAVR--->SIM:\t"));
  DEBUG_PRINTLN(tempData);
  SIM7600E_SS.println(tempData);

  uint8_t l = readline(timeout);

  DEBUG_PRINT(F("\tAVR<---SIM:\t"));
  DEBUG_PRINTLN(replybuffer);
  DEBUG_PRINTLN(F("\t----------------------------------"));
  return l;
}
boolean SIM7600E::sendCheckReply(const char* send, const char *reply, uint16_t timeout)
{
  if (!getReply(send, timeout))
    return false;
  return (strcmp(replybuffer, reply) == 0);
}

boolean SIM7600E::parseReply(const char* toreply, uint16_t *v, char divider, uint8_t index) {
  char *p = strstr(replybuffer, toreply);  // get the pointer to the voltage
  if (p == 0) return false;
  p+=strlen(toreply);
  //DEBUG_PRINTLN(p);
  for (uint8_t i=0; i<index; i++) {
    // increment dividers
    p = strchr(p, divider);
    if (!p) return false;
    p++;
    //DEBUG_PRINTLN(p);

  }
  *v = atoi(p);

  return true;
}

boolean SIM7600E::parseReply(const char* toreply, char *v, char divider, uint8_t index) {
  uint8_t i=0;
  char *p = strstr(replybuffer, toreply);
  if (p == 0) return false;
  p+=strlen(toreply);

  for (i=0; i<index;i++) {
    // increment dividers
    p = strchr(p, divider);
    if (!p) return false;
    p++;
  }

  for(i=0; i<strlen(p);i++) {
    if(p[i] == divider)
      break;
    v[i] = p[i];
  }

  v[i] = '\0';

  return true;
}

boolean SIM7600E::expectReply(const char* reply, uint16_t timeout){

  readline(timeout);
  DEBUG_PRINT(F("\tAVR<---SIM:\t")); 
  DEBUG_PRINTLN(replybuffer);

  return (strcmp(replybuffer, reply) == 0);
}
