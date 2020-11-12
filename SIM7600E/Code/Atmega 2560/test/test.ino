#include <SoftwareSerial.h>

const byte rxPin = 10;
const byte txPin = 11;

SoftwareSerial SIM5320E(rxPin, txPin); // RX, TX

void setup()
{
  // Open serial communications to computer
  Serial.begin(9600);

  SIM5320E.begin(9600); // Default for the board
  delay(100); // Let the command run
}
void loop()
{
  // nhap lenh -> SIM
  if (Serial.available())
  {
    SIM5320E.write(Serial.read());
    //SIM5320E.println("AT\r\n");
  }
  // SIM -> Serial 
  if (SIM5320E.available())
  {
    Serial.write(SIM5320E.read());
  }
}
