#include "MyJson.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  MyJsonClass MyJson;
  MyJson.encode("temperture", 5);
  MyJson.encode("humidity", 7);
  MyJson.encode("light", 8);

  String message = MyJson.getStringEncode();
  Serial.println(message);
  char res[200];
  message.toCharArray(res, 200);
  Serial.println(message);

  delay(2000);
}
