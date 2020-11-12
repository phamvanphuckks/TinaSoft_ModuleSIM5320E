#include <SoftwareSerial.h>

const byte rxPin = 10;
const byte txPin = 11;

SoftwareSerial SIM5320E(rxPin, txPin); // RX, TX

void setup()
{
  // Open serial communications to computer
  Serial.begin(9600);

  SIM5320E.begin(115200); // Default for the board
  SIM5320E.println("AT+IPR=115200\r\n");  // Set baud to 57600
  delay(100); // Let the command run
  
  SIM5320E.begin(115200); // Reconnect at lower baud, 115200 had issues with SoftwareSerial
  SIM5320E.println("AT\r\n");
}
void loop()
{
  // nhap lenh -> SIM
//  if (Serial.available())
//  {
//    SIM5320E.write(Serial.read());
//    //SIM5320E.println("AT\r\n");
//  }
//  // SIM -> Serial 
//  if (SIM5320E.available())
//  {
//    Serial.write(SIM5320E.read());
//  }
  SIM5320E.println("AT\r\n");
  delay(3000);
}
