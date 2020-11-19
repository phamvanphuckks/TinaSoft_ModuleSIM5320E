/**
 * Author: Tinasoft.vn
 */
#ifndef _MY_JSON_H_
#define _MY_JSON_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#define JSON_BUFFER_LENGTH 1460

class MyJsonClass
{
    private:
        StaticJsonBuffer<JSON_BUFFER_LENGTH> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
    public:
        MyJsonClass();
        ~MyJsonClass();
        JsonObject& encode(const char* key, long value);
        JsonObject& encode(const char* key, const char* value);
        JsonObject& encodeArray(char* key, int value[], int count);
        JsonObject& decode(char[]);
        String getStringEncode();
};

extern MyJsonClass MyJson;

#endif /* _MY_JSON_H_ */
