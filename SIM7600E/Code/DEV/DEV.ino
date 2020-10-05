#include "sim7600e.h"


//SIM7600E SIM(115200);

unsigned long cur_time, old_time;
static const char *url = "http://admin.breedlife.com/api/v1/data";
static const char *data = "{\"data\":[\"TR001\",[[8,8,8],[9,9,9],[9,9,9],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}";

void setup()
{
  Serial.begin(9600);
  Serial.println("hello");
//  SIM.rstSIM();
//  delay(100);
//
//  SIM.setBaud(9600);
//  String txt = SIM.getModemInfo();
//  Serial.print("modem: ");
//  Serial.println(txt);
//  delay(1000);
}
/*
  BUG: thieu len \r\n o ham getReply
*/
void loop()
{
  //  cur_time = millis();
  //  if ( cur_time - old_time >= 5000){
  //    if(!SIM.pushData(url, data)){
  //      Serial.println("fail post");
  //    }
  //    old_time = cur_time;
  //  }
//  SIM.pushData(url, data);

//  SIM.HTTP_init();
//  delay(2000);
//  SIM.HTTP_para("URL", url);
//  delay(5000);
//  SIM.HTTP_para("CONTENT", "application/json");
//  delay(5000);
//  SIM.HTTP_data(strlen(data), 10000);
//  delay(1000);
//  SIM.HTTP_action(HTTP_POST, 1, strlen(data), strlen(data));
//  delay(1000);
//  SIM.HTTP_term();
//  delay(2000);
}
