#include <SoftwareSerial.h>

const byte rxPin = 10;
const byte txPin = 11;

SoftwareSerial SIM5320E(rxPin, txPin); // RX, TX

void setup()
{
  // Open serial communications to computer
  Serial.begin(115200);

  SIM5320E.begin(115200); // Default for the board
  SIM5320E.println("AT+IPR=115200\r\n");  // Set baud to 57600
  delay(100); // Let the command run
  
  SIM5320E.begin(115200); // Reconnect at lower baud, 115200 had issues with SoftwareSerial
  
  //Clear out any waiting serial data
//  while (SIM5320E.available())
//  {
//    SIM5320E.read();
//  }
//  delay(3000);
//  SIM5320E.println("AT\r\n");
//  delay(100);
  
  SIM5320E.println("AT+CSOCKSETPN=1\r\n");
  SIM5320E.println("AT+CIPMODE=0\r\n");
  SIM5320E.println("AT+NETOPEN=,,1\r\n");
  SIM5320E.println("AT+IPADDR\r\n");
  SIM5320E.println("AT+CHTTPSSTART\r\n");
  SIM5320E.println("AT+CHTTPSOPSE=\"httpbin.org\", 80,1\r\n");
  SIM5320E.println("AT+CHTTPSSEND=41\r\n");
  SIM5320E.println("GET http://httpbin.org/get HTTP/1.1\r\n");
  //SIM5320E.print("GET http://httpbin.org/get HTTP/1.1\r\n");
  //SIM5320E.println(26, HEX);
  SIM5320E.println("AT+CHTTPSSEND\r\n");
  SIM5320E.println("AT+CHTTPSRECV=1000\r\n");
}

void loop()
{
  // nhap lenh -> SIM
//  if (Serial.available())
//  {
//    //SIM5320E.write(Serial.read());
//    if(Serial.read()=='a')
//      SIM5320E.write("AT\r\n");
//  }
//  // SIM -> Serial 
//  if (SIM5320E.available())
//  {
//    Serial.write(SIM5320E.read());
//  }

}
