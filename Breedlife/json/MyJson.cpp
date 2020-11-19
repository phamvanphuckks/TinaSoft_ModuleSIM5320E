/**
 * Author: Tinasoft.vn
 */
#include "MyJson.h"


/**
 * constructor
 */
MyJsonClass::MyJsonClass()
{

}

/**
 * destructor
 */
MyJsonClass::~MyJsonClass()
{
}

//{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
JsonObject& MyJsonClass::encode(const char* key, long value){
    root[key] = value;
    return root;
}

JsonObject& MyJsonClass::encode(const char* key, const char* value){
    root[key] = value;
    return root;
}

JsonObject& MyJsonClass::encodeArray(char* key, int value[], int count){
    JsonArray& data = root.createNestedArray(key);
    for(int i=0; i<count; i++){
        data.add(value[i]);
    }
    return root;
}

JsonObject& MyJsonClass::decode(char json[]){
//    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    JsonObject& root = jsonBuffer.parseObject(json);
    return root;
}

String MyJsonClass::getStringEncode(){
    String stringRoot;
    root.printTo(stringRoot); 
    return stringRoot;
}
