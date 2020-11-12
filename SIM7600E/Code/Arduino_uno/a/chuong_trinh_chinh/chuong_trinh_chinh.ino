#include <Arduino.h>
#include <Wire.h>
#include <MCP3008.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include "sim7600e.h"
#include "RTClib.h"
#include "main.h"

RTC_DS1307 RTC;
File myFile;

LiquidCrystal lcd(33, 35, 37, 39, 41, 43);

#include <SimpleKalmanFilter.h>

SimpleKalmanFilter bo_loc1(1, 1, 0.01);
SimpleKalmanFilter bo_loc2(1, 1, 0.01);
SimpleKalmanFilter bo_loc3(1, 1, 0.01);
SimpleKalmanFilter bo_loc4(1, 1, 0.01);
SimpleKalmanFilter bo_loc5(1, 1, 0.01);
SimpleKalmanFilter bo_loc6(1, 1, 0.01);
SimpleKalmanFilter bo_loc7(1, 1, 0.01);
SimpleKalmanFilter bo_loc8(1, 1, 0.01);
SimpleKalmanFilter bo_loc9(1, 1, 0.01);
SimpleKalmanFilter bo_loc10(1, 1, 0.01);
SimpleKalmanFilter bo_loc11(1, 1, 0.01);
SimpleKalmanFilter bo_loc12(1, 1, 0.01);
SimpleKalmanFilter bo_loc13(1, 1, 0.01);
SimpleKalmanFilter bo_loc14(1, 1, 0.01);
SimpleKalmanFilter bo_loc15(1, 1, 0.01);
SimpleKalmanFilter bo_loc16(1, 1, 0.01);
SimpleKalmanFilter bo_loc17(1, 1, 0.01);
SimpleKalmanFilter bo_loc18(1, 1, 0.01);
SimpleKalmanFilter bo_loc19(1, 1, 0.01);
SimpleKalmanFilter bo_loc20(1, 1, 0.01);
SimpleKalmanFilter bo_loc21(1, 1, 0.01);
SimpleKalmanFilter bo_loc22(1, 1, 0.01);
SimpleKalmanFilter bo_loc23(1, 1, 0.01);
SimpleKalmanFilter bo_loc24(1, 1, 0.01);
SimpleKalmanFilter bo_loc25(2, 2, 0.01);
SimpleKalmanFilter bo_loc26(2, 2, 0.01);
SimpleKalmanFilter bo_loc27(2, 2, 0.01);

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);
MCP3008 adc1(CLOCK_PIN1, MOSI_PIN1, MISO_PIN1, CS_PIN1);


SIM7600E SIM;

unsigned long cur_time, old_time;
static const char *url = "http://admin.breedlife.com/api/v1/data";
static const char *data = "{\"data\":[\"TR001\",[[8,8,8],[9,9,9],[9,9,9],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}";

/*function user deftion*/

// LCD funtion

void LCD_Print_ACCU(float votagle, uint8_t lpos)
{
  char tempdata[30];
  sprintf(tempdata, "ACCU %d", lpos-2);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(tempdata);
  lcd.setCursor(3, 1);
  lcd.print("U = ");
  lcd.print(votagle, 3);
  lcd.print(" V");
}

void LCD_Print_UT1_ACCU(float votagle, uint8_t lpos)
{
  char tempdata[30];
  sprintf(tempdata,"UT1_ACCU %d",lpos-2);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(tempdata);
  lcd.setCursor(3, 1);
  lcd.print("U = ");
  lcd.print(votagle, 3);
  lcd.print(" V");
}

void LCD_Display(void)
{
  if (U1 < 5)
  {
    if (pos != tt)
    {
      time = millis();
      tt = pos;
      switch (pos)
      {
      case 1:
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("status");
        lcd.setCursor(0, 1);
        lcd.print("Ut=");
        lcd.print(U1 + U2 + U3 + U4 + U5 + U6 + U7 + U8 + U9 + U10 + U11 + U12 + U13 + U14 + U15 + U16 + U17 + U18 + U19 + U20 + U21 + U22 + U23 + U24, 1);
        lcd.setCursor(8, 1);
        lcd.print("It=");
        lcd.print(current, 1);
        break;
      case 2:
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("Temp");
        lcd.setCursor(0, 1);
        lcd.print("T0=");
        lcd.print(T0, 1);
        lcd.setCursor(9, 1);
        lcd.print("T1=");
        lcd.print(T1, 1);
        break;
      case 3:
        LCD_Print_ACCU(U1, pos);
        break;
      case 4:
        LCD_Print_ACCU(U2, pos);
        break;
      case 5:
        LCD_Print_ACCU(U3, pos);
        break;
      case 6:
        LCD_Print_ACCU(U4, pos);
        break;
      case 7:
        LCD_Print_ACCU(U5, pos);
        break;
      case 8:
        LCD_Print_ACCU(U6, pos);
        break;
      case 9:
        LCD_Print_ACCU(U7, pos);
        break;
      case 10:
        LCD_Print_ACCU(U8, pos);
        break;
      case 11:
        LCD_Print_ACCU(U9, pos);
        break;
      case 12:
        LCD_Print_ACCU(U10, pos);
        break;
      case 13:
        LCD_Print_ACCU(U11, pos);
        break;
      case 14:
        LCD_Print_ACCU(U12, pos);
        break;
      case 15:
        LCD_Print_ACCU(U13, pos);
        break;
      case 16:
        LCD_Print_ACCU(U14, pos);
        break;
      case 17:
        LCD_Print_ACCU(U15, pos);
        break;
      case 18:
        LCD_Print_ACCU(U16, pos);
        break;
      case 19:
        LCD_Print_ACCU(U17, pos);
        break;
      case 20:
        LCD_Print_ACCU(U18, pos);
        break;
      case 21:
        LCD_Print_ACCU(U19, pos);
        break;
      case 22:
        LCD_Print_ACCU(U20, pos);
        break;
      case 23:
        LCD_Print_ACCU(U21, pos);
        break;
      case 24:
        LCD_Print_ACCU(U22, pos);
        break;
      case 25:
        LCD_Print_ACCU(U23, pos);
        break;
      case 26:
        LCD_Print_ACCU(U24, pos);
        break;
      }
    }
  }

  if (U1 > 9)
  {
    if (pos != tt)
    {
      time = millis();
      tt = pos;
      switch (pos)
      {
      case 1:
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("status");
        lcd.setCursor(0, 1);
        lcd.print("Ut=");
        lcd.print(U1 + U2 + U3 + U4, 1);
        lcd.setCursor(8, 1);
        lcd.print("It=");
        lcd.print(current, 1);
        break;
      case 2:
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Temp");
        lcd.setCursor(0, 1);
        lcd.print("T0=");
        lcd.print(T0, 1);
        lcd.setCursor(9, 1);
        lcd.print("T1=");
        lcd.print(T1, 1);
        break;
      case 3:
        LCD_Print_UT1_ACCU(U1, pos);
        break;
      case 4:
        LCD_Print_UT1_ACCU(U2, pos);
        break;
      case 5:
        LCD_Print_UT1_ACCU(U3, pos);
        break;
      case 6:
        LCD_Print_UT1_ACCU(U4, pos);
        break;
      case 7:
        LCD_Print_UT1_ACCU(U5, pos);
        break;
      case 8:
        LCD_Print_UT1_ACCU(U6, pos);
        break;
      case 9:
        LCD_Print_UT1_ACCU(U7, pos);
        break;
      case 10:
        LCD_Print_UT1_ACCU(U8, pos);
        break;
      case 11:
        LCD_Print_UT1_ACCU(U9, pos);
        break;
      case 12:
        LCD_Print_UT1_ACCU(U10, pos);
        break;
      case 13:
        LCD_Print_UT1_ACCU(U11, pos);
        break;
      case 14:
        LCD_Print_UT1_ACCU(U12, pos);
        break;
      case 15:
        LCD_Print_UT1_ACCU(U13, pos);
        break;
      case 16:
        LCD_Print_UT1_ACCU(U14, pos);
        break;
      case 17:
        LCD_Print_UT1_ACCU(U15, pos);
        break;
      case 18:
        LCD_Print_UT1_ACCU(U16, pos);
        break;
      }
    }
  }

  if ((unsigned long)(millis() - time) > 5000)
  {
    time = millis();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("BREEDLIFE");
    lcd.setCursor(11, 1);
    DateTime now = RTC.now();
    lcd.print(now.hour(), DEC);
    lcd.print(":");

    if (now.minute() < 10)
    {
      lcd.print("0");
      lcd.print(now.minute(), DEC);
    }
    else
    {
      lcd.print(now.minute(), DEC);
    }
    lcd.setCursor(0, 1);
    lcd.print(now.day(), DEC);
    lcd.print("/");
    if (now.month() < 10)
    {
      lcd.print("0");
      lcd.print(now.month(), DEC);
    }
    else
    {
      lcd.print(now.month(), DEC);
    }
    lcd.print("/");
    lcd.print(now.year(), DEC);
    lcd.print(" ");
  }
}

// SD function

void SD_Write2File_2V(const char *namefile, const char *stt)
{
  myFile = SD.open(namefile, FILE_WRITE);
  if (myFile)
  {
    myFile.println("             ");
    DateTime now = RTC.now();
    myFile.print(now.day());
    myFile.print("/");
    myFile.print(now.month());
    myFile.print("/");
    myFile.print(now.year());
    myFile.print(" ");
    myFile.print(now.hour());
    myFile.print(":");
    myFile.print(now.minute());
    myFile.print("         ");
    myFile.print(stt);
    myFile.print("         ");
    myFile.print(current, 1);
    myFile.print("         ");
    myFile.print(T0, 1);
    myFile.print("         ");
    myFile.print(T1, 1);
    myFile.print("         ");
    myFile.print(U1 + U2 + U3 + U4, 1);
    myFile.print("         ");
    myFile.print(U1, 4);
    myFile.print("         ");
    myFile.print(U2, 4);
    myFile.print("         ");
    myFile.print(U3, 4);
    myFile.print("         ");
    myFile.print(U4, 4);
    myFile.print("         ");
    myFile.print(U5 + U6 + U7 + U8, 1);
    myFile.print("         ");
    myFile.print(U5, 4);
    myFile.print("         ");
    myFile.print(U6, 4);
    myFile.print("         ");
    myFile.print(U7, 4);
    myFile.print("         ");
    myFile.print(U8, 4);
    myFile.print("         ");
    myFile.print(U9 + U10 + U11 + U12, 1);
    myFile.print("         ");
    myFile.print(U9, 4);
    myFile.print("         ");
    myFile.print(U10, 4);
    myFile.print("         ");
    myFile.print(U11, 4);
    myFile.print("         ");
    myFile.print(U12, 4);
    myFile.print("         ");
    myFile.print(U13 + U14 + U15 + U16, 1);
    myFile.print("         ");
    myFile.print(U13, 4);
    myFile.print("         ");
    myFile.print(U14, 4);
    myFile.print("         ");
    myFile.print(U15, 4);
    myFile.print("         ");
    myFile.println(U16, 4);
    myFile.close();
    Serial.println("done.");
  }
  time1 = millis();
}

void SD_Write2File_12V(const char *namefile, const char *stt)
{
  myFile = SD.open(namefile, FILE_WRITE);
  if (myFile)
  {
    myFile.println("             ");
    DateTime now = RTC.now();
    myFile.print(now.day());
    myFile.print("/");
    myFile.print(now.month());
    myFile.print("/");
    myFile.print(now.year());
    myFile.print("   ");
    myFile.print(now.hour());
    myFile.print(":");
    myFile.print(now.minute());
    myFile.print("         ");
    myFile.print(stt);
    myFile.print("         ");
    myFile.print(current, 1);
    myFile.print("         ");
    myFile.print(T0, 1);
    myFile.print("         ");
    myFile.print(T1, 1);
    myFile.print("         ");
    myFile.print(U1 + U2 + U3 + U4, 1);
    myFile.print("         ");
    myFile.print(U1, 4);
    myFile.print("         ");
    myFile.print(U2, 4);
    myFile.print("         ");
    myFile.print(U3, 4);
    myFile.print("         ");
    myFile.print(U4, 4);
    myFile.print("         ");
    myFile.print(U5 + U6 + U7 + U8, 1);
    myFile.print("         ");
    myFile.print(U5, 4);
    myFile.print("         ");
    myFile.print(U6, 4);
    myFile.print("         ");
    myFile.print(U7, 4);
    myFile.print("         ");
    myFile.print(U8, 4);
    myFile.print("         ");
    myFile.print(U9 + U10 + U11 + U12, 1);
    myFile.print("         ");
    myFile.print(U9, 4);
    myFile.print("         ");
    myFile.print(U10, 4);
    myFile.print("         ");
    myFile.print(U11, 4);
    myFile.print("         ");
    myFile.print(U12, 4);
    myFile.print("         ");
    myFile.print(U13 + U14 + U15 + U16, 1);
    myFile.print("         ");
    myFile.print(U13, 4);
    myFile.print("         ");
    myFile.print(U14, 4);
    myFile.print("         ");
    myFile.print(U15, 4);
    myFile.print("         ");
    myFile.println(U16, 4);
    myFile.close();
    Serial.println("done.");
  }
  time1 = millis();
}

void SD_Save2file(void)
{
  if ((U1 > 9) && (current > 2) && ((unsigned long)(millis() - time1) > 300000))
  {
    SD_Write2File_12V("1nap_12v.txt", "ON");
  }
  if ((U1 > 9) && (current < -2) && ((unsigned long)(millis() - time1) > 600000))
  {
    SD_Write2File_12V("12v_xa.txt", "OFF");
  }
  if ((U1 < 4) && (current > 2) && ((unsigned long)(millis() - time1) > 300000))
  {
    SD_Write2File_2V("nap_2v_nap.txt", "ON");
  }
  if ((U1 < 4) && (current < -2) && ((unsigned long)(millis() - time1) > 600000))
  {
    SD_Write2File_2V("xa_2v_xa.txt", "OFF");
  }
  if ((U1 > 10) && ((unsigned long)(millis() - time2) > 5000))
  {
    Serial.println("CLEARDATA");
    Serial.println("LABEL, computer Time, current, nhiet do 1, nhiet do 2, Ut1,Ut1_accu1,Ut1_accu2,Ut1_accu3,Ut1_accu4,Ut2,Ut2_accu1,Ut2_accu2,Ut2_accu3,Ut2_accu4,Ut3,Ut3_accu1,Ut3_accu2,Ut3_accu3,Ut3_accu4");
    Serial.print("DATA, TIME, ");
    Serial.print(current, 1);
    Serial.print(",");
    Serial.print(T0, 1);
    Serial.print(", ");
    Serial.print(T1, 1);
    Serial.print(", ");
    Serial.print(U1 + U2 + U3 + U4, 1);
    Serial.print(", ");
    Serial.print(U1, 4);
    Serial.print(", ");
    Serial.print(U2, 4);
    Serial.print(", ");
    Serial.print(U3, 4);
    Serial.print(", ");
    Serial.print(U4, 4);
    Serial.print(" ,");
    Serial.print(U5 + U6 + U7 + U8, 1);
    Serial.print(", ");
    Serial.print(U5, 4);
    Serial.print(", ");
    Serial.print(U6, 4);
    Serial.print(", ");
    Serial.print(U7, 4);
    Serial.print(", ");
    Serial.print(U8, 4);
    Serial.print(", ");
    Serial.print(U9 + U10 + U11 + U12, 1);
    Serial.print(", ");
    Serial.print(U9, 4);
    Serial.print(", ");
    Serial.print(U10, 4);
    Serial.print(", ");
    Serial.print(U11, 4);
    Serial.print(", ");
    Serial.print(U12, 4);
    Serial.print(", ");
    Serial.print(U13 + U14 + U15 + U16, 1);
    Serial.print(", ");
    Serial.print(U13, 4);
    Serial.print(", ");
    Serial.print(U14, 4);
    Serial.print(",");
    Serial.print(U15, 1);
    Serial.print(",");
    Serial.println(U16, 1);
    time2 = millis();
  }
  if ((U1 < 3) && ((unsigned long)(millis() - time2) > 200000))
  {
    Serial.println("CLEARDATA");
    Serial.println("LABEL, computer Time,UT,current,nhiet do 1,nhiet do 2,U1,U2,U3,U4,U5,U6,U7,U8,U9,U10,U11,U12,U13,U14,U15,U16,U17,U18,U19,U20,U21,U22,U23,U24");
    Serial.print("DATA, TIME, ");
    Serial.print(U1 + U2 + U3 + U4 + U5 + U6 + U7 + U8 + U9 + U10 + U11 + U12 + U13 + U14 + U15 + U16 + U17 + U18 + U19 + U20 + U21 + U22 + U23 + U24, 1);
    Serial.print(", ");
    Serial.print(current, 1);
    Serial.print(",");
    Serial.print(T0, 1);
    Serial.print(", ");
    Serial.print(T1, 1);
    Serial.print(", ");
    Serial.print(U1, 4);
    Serial.print(", ");
    Serial.print(U2, 4);
    Serial.print(", ");
    Serial.print(U3, 4);
    Serial.print(", ");
    Serial.print(U4, 4);
    Serial.print(" ,");
    Serial.print(U5, 4);
    Serial.print(", ");
    Serial.print(U6, 4);
    Serial.print(", ");
    Serial.print(U7, 4);
    Serial.print(", ");
    Serial.print(U8, 4);
    Serial.print(", ");
    Serial.print(U9, 4);
    Serial.print(", ");
    Serial.print(U10, 4);
    Serial.print(", ");
    Serial.print(U11, 4);
    Serial.print(", ");
    Serial.print(U12, 4);
    Serial.print(", ");
    Serial.print(U13, 4);
    Serial.print(", ");
    Serial.print(U14, 4);
    Serial.print(",");
    Serial.print(U15, 4);
    Serial.print(", ");
    Serial.print(U16, 4);
    Serial.print(", ");
    Serial.print(U17, 4);
    Serial.print(", ");
    Serial.print(U18, 4);
    Serial.print(", ");
    Serial.print(U19, 4);
    Serial.print(", ");
    Serial.print(U20, 4);
    Serial.print(", ");
    Serial.print(U21, 4);
    Serial.print(", ");
    Serial.print(U22, 4);
    Serial.print(",");
    Serial.print(U23, 4);
    Serial.print(",");
    Serial.println(U24, 4);
    time2 = millis();
  }
}

//button function
void BUTTON_light(void)
{
  int nnligt = digitalRead(button_ligt);
  if (nnligt == HIGH)
  {
    delay(10);
    if (nnligt == HIGH)
    {
      time_nnlingt == millis();
      digitalWrite(led, HIGH);
      while (nnligt == HIGH)
      {
        nnligt = digitalRead(button_ligt);
      }
    }
  }

  if ((unsigned long)(millis() - time_nnlingt) > 180000)
  {
    digitalWrite(led, LOW);
    time_nnlingt = millis();
  }
}

void BUTTON_back(void)
{
  int nnback = digitalRead(button_back);
  if (nnback == HIGH)
  {
    delay(10);
    if (nnback == HIGH)
    {
      pos = 1;
      while (nnback == HIGH)
      {
        nnback = digitalRead(button_back);
      }
    }
  }
}

void BUTTON_up(void)
{
  int nnup = digitalRead(button_up);
  if (nnup == HIGH)
  {
    delay(10);
    if (nnup == HIGH)
    {
      pos = pos + 1;
      if (pos > 26)
        pos = 1;
      while (nnup == HIGH)
      {
        nnup = digitalRead(button_up);
      }
    }
  }
}

void BUTTON_down(void)
{
  int nndw = digitalRead(button_down);
  if (nndw == HIGH)
  {
    delay(10);
    if (nndw == HIGH)
    {
      pos = pos - 1;
      if (pos < 1)
        pos = 26;
      while (nndw == HIGH)
      {
        nndw = digitalRead(button_down);
      }
    }
  }
}

// voltagle function
void getVoltage(void)
{
  //lấy giá trị votalge;
  kalman1 = bo_loc1.updateEstimate(analogRead(analog0));
  U1 = (kalman1 * (4.88 / 1023.0)) / 0.0801;

  //lay gia tri accu2v
  if (U1 < 3.5)
  {
    kalman2 = bo_loc2.updateEstimate(analogRead(analog2) - analogRead(analog0)) - 0.2;
    kalman3 = bo_loc3.updateEstimate(analogRead(analog4) - analogRead(analog2)) + 0.5;
    kalman4 = bo_loc4.updateEstimate(analogRead(analog6) - analogRead(analog4)) + 0.4;
    kalman5 = bo_loc5.updateEstimate(analogRead(analog8) - analogRead(analog6)) + 1.7;
    kalman6 = bo_loc6.updateEstimate(analogRead(analog10) - analogRead(analog8)) - 7.3;
    kalman7 = bo_loc7.updateEstimate(analogRead(analog12) - analogRead(analog10)) - 1.8;
    kalman8 = bo_loc8.updateEstimate(analogRead(analog14) - analogRead(analog12)) + 3.7;
    kalman9 = bo_loc9.updateEstimate(adc.readADC(0) - analogRead(analog14) - 2);
    kalman10 = bo_loc10.updateEstimate(adc.readADC(1) - adc.readADC(0)) - 2.5;
    kalman11 = bo_loc11.updateEstimate(adc.readADC(2) - adc.readADC(1)) + 3;
    kalman12 = bo_loc12.updateEstimate(adc.readADC(3) - adc.readADC(2)) + 0.2;
    kalman13 = bo_loc13.updateEstimate(adc.readADC(4) - adc.readADC(3)) + 5.2;
    kalman14 = bo_loc14.updateEstimate(adc.readADC(5) - adc.readADC(4)) - 0.5;
    kalman15 = bo_loc15.updateEstimate(adc.readADC(6) - adc.readADC(5)) - 4.5;
    kalman16 = bo_loc16.updateEstimate(adc.readADC(7) - adc.readADC(6)) + 10.6;
    kalman17 = bo_loc17.updateEstimate(adc1.readADC(0) - adc.readADC(7)) - 20;
    kalman18 = bo_loc18.updateEstimate(adc1.readADC(1) - adc1.readADC(0)) + 6.2;
    kalman19 = bo_loc19.updateEstimate(adc1.readADC(2) - adc1.readADC(1) - 0.2);
    kalman20 = bo_loc20.updateEstimate(adc1.readADC(3) - adc1.readADC(2)) + 3.8;
    kalman21 = bo_loc21.updateEstimate(adc1.readADC(4) - adc1.readADC(3)) + 7.5;
    kalman22 = bo_loc22.updateEstimate(adc1.readADC(5) - adc1.readADC(4)) - 1.7;
    kalman23 = bo_loc23.updateEstimate(adc1.readADC(6) - adc1.readADC(5)) + 13.5;
    kalman24 = bo_loc24.updateEstimate(adc1.readADC(7) - adc1.readADC(6)) - 9;

    U2 = ((kalman2 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U3 = ((kalman3 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U4 = ((kalman4 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U5 = ((kalman5 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U6 = ((kalman6 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U7 = ((kalman7 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U8 = ((kalman8 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U9 = ((kalman9 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U10 = ((kalman10 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U11 = ((kalman11 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U12 = ((kalman12 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U13 = ((kalman13 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U14 = ((kalman14 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U15 = ((kalman15 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U16 = ((kalman16 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U17 = ((kalman17 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U18 = ((kalman18 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U19 = ((kalman19 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U20 = ((kalman20 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U21 = ((kalman21 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U22 = ((kalman22 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U23 = ((kalman23 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U24 = ((kalman24 * 5.0) / 1023.0) / (R2 / (R1 + R2));
  }
  //lay gia tri accu12v
  if (U1 > 9)
  {
    kalman2 = bo_loc2.updateEstimate(analogRead(analog2) - analogRead(analog0));
    kalman3 = bo_loc3.updateEstimate(analogRead(analog4) - analogRead(analog2));
    kalman4 = bo_loc4.updateEstimate(analogRead(analog6) - analogRead(analog4));
    kalman5 = bo_loc5.updateEstimate(analogRead(analog8));
    kalman6 = bo_loc6.updateEstimate(analogRead(analog10) - analogRead(analog8));
    kalman7 = bo_loc7.updateEstimate(analogRead(analog12) - analogRead(analog10));
    kalman8 = bo_loc8.updateEstimate(analogRead(analog14) - analogRead(analog12));
    kalman9 = bo_loc9.updateEstimate(adc.readADC(0));
    kalman10 = bo_loc10.updateEstimate(adc.readADC(1) - adc.readADC(0));
    kalman11 = bo_loc11.updateEstimate(adc.readADC(2) - adc.readADC(1));
    kalman12 = bo_loc12.updateEstimate(adc.readADC(3) - adc.readADC(2));
    kalman13 = bo_loc13.updateEstimate(adc.readADC(4));
    kalman14 = bo_loc14.updateEstimate(adc.readADC(5) - adc.readADC(4));
    kalman15 = bo_loc15.updateEstimate(adc.readADC(6) - adc.readADC(5));
    kalman16 = bo_loc16.updateEstimate(adc.readADC(7) - adc.readADC(6));

    U2 = ((kalman2 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U3 = ((kalman3 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U4 = ((kalman4 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U5 = ((kalman5 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U6 = ((kalman6 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U7 = ((kalman7 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U8 = ((kalman8 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U9 = ((kalman9 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U10 = ((kalman10 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U11 = ((kalman11 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U12 = ((kalman12 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U13 = ((kalman13 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U14 = ((kalman14 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U15 = ((kalman15 * 5.0) / 1023.0) / (R2 / (R1 + R2));
    U16 = ((kalman16 * 5.0) / 1023.0) / (R2 / (R1 + R2));
  }

  //dòng điện
  float kalman25 = bo_loc25.updateEstimate(analogRead(analog5));
  current = (((5.0 / 1023.0) * kalman25) - (vcc * 0.5) + 0.007) / factor;
  //nhiet độ
  float kalman26 = bo_loc26.updateEstimate(analogRead(analog1));
  float kalman27 = bo_loc27.updateEstimate(analogRead(analog3));
  float T0 = (kalman26 * 5.0 / 1023) * 100;
  float T1 = (kalman27 * 5.0 / 1023) * 100;
}

// led function

void LED_Warning_24accu2v(void)
{
  for (uint8_t idex = 1; idex < 25; idex++)
  {
    if (idex == 7)
      digitalWrite(LED_Array[idex], LOW);
    else
    {
      digitalWrite(LED_Array[idex], HIGH);
    }
  }

  if ((-10 < current) && (current < -1))
  {
    if (U1 < 1.7 && (unsigned long)(millis() - time3) < 60000)
    {
      digitalWrite(led1, LOW);
      time3 = millis();
    }
    if (U2 < 1.7 && (unsigned long)(millis() - time4) < 60000)
    {
      digitalWrite(led2, LOW);
      time4 = millis();
    }
    if (U3 < 1.7 && (unsigned long)(millis() - time5) < 60000)
    {
      digitalWrite(led3, LOW);
      time5 = millis();
    }
    if (U4 < 1.7 && (unsigned long)(millis() - time6) < 60000)
    {
      digitalWrite(led4, LOW);
      time6 = millis();
    }
    if (U5 < 1.7 && (unsigned long)(millis() - time7) < 60000)
    {
      digitalWrite(led5, LOW);
      time7 = millis();
    }
    if (U6 < 1.7 && (unsigned long)(millis() - time8) < 60000)
    {
      digitalWrite(led6, LOW);
      time8 = millis();
    }
    if (U7 < 1.7 && (unsigned long)(millis() - time9) < 60000)
    {
      digitalWrite(led7, LOW);
      time9 = millis();
    }
    if (U8 < 1.7 && (unsigned long)(millis() - time10) < 60000)
    {
      digitalWrite(led8, LOW);
      time10 = millis();
    }
    if (U9 < 1.7 && (unsigned long)(millis() - time11) < 60000)
    {
      digitalWrite(led9, LOW);
      time11 = millis();
    }
    if (U10 < 1.7 && (unsigned long)(millis() - time12) < 60000)
    {
      digitalWrite(led10, LOW);
      time12 = millis();
    }
    if (U11 < 1.7 && (unsigned long)(millis() - time13) < 60000)
    {
      digitalWrite(led11, LOW);
      time13 = millis();
    }
    if (U12 < 1.7 && (unsigned long)(millis() - time14) < 60000)
    {
      digitalWrite(led12, LOW);
      time14 = millis();
    }
    if (U13 < 1.7 && (unsigned long)(millis() - time15) < 60000)
    {
      digitalWrite(led13, LOW);
      time15 = millis();
    }
    if (U14 < 1.7 && (unsigned long)(millis() - time16) < 60000)
    {
      digitalWrite(led14, LOW);
      time16 = millis();
    }
    if (U15 < 1.7 && (unsigned long)(millis() - time17) < 60000)
    {
      digitalWrite(led15, LOW);
      time17 = millis();
    }
    if (U16 < 1.7 && (unsigned long)(millis() - time18) < 60000)
    {
      digitalWrite(led16, LOW);
      time18 = millis();
    }
    if (U17 < 1.7 && (unsigned long)(millis() - time19) < 60000)
    {
      digitalWrite(led17, LOW);
      time19 = millis();
    }
    if (U18 < 1.7 && (unsigned long)(millis() - time20) < 60000)
    {
      digitalWrite(led18, LOW);
      time20 = millis();
    }
    if (U19 < 1.7 && (unsigned long)(millis() - time21) < 60000)
    {
      digitalWrite(led21, LOW);
      time21 = millis();
    }
    if (U20 < 1.7 && (unsigned long)(millis() - time22) < 60000)
    {
      digitalWrite(led20, LOW);
      time22 = millis();
    }
    if (U21 < 1.7 && (unsigned long)(millis() - time23) < 60000)
    {
      digitalWrite(led21, LOW);
      time23 = millis();
    }
    if (U22 < 1.7 && (unsigned long)(millis() - time24) < 60000)
    {
      digitalWrite(led2, LOW);
      time4 = millis();
    }
    if (U23 < 1.7 && (unsigned long)(millis() - time25) < 60000)
    {
      digitalWrite(led23, LOW);
      time25 = millis();
    }
    if (U24 < 1.7 && (unsigned long)(millis() - time26) < 60000)
    {
      digitalWrite(led24, LOW);
      time26 = millis();
    }
  }
}

void setup()
{
  Serial.begin(9600);

  lcd.begin(16, 2);
  Wire.begin();
  RTC.begin();
  SD.begin(53);

  pinMode(button_up, INPUT);
  pinMode(button_down, INPUT);

  for (uint8_t idex = 0; idex < 25; idex++)
    pinMode(LED_Array[idex], OUTPUT);

   delay(5000);
  //  SIM.rstSIM();
  //  delay(100);
  Serial.begin(9600);
  delay(100);
  if(SIM.setupSIM(9600))
  { 
    Serial.println(F("\t----------------------------------"));
    Serial.println(F("\tSettup done"));
    Serial.println(F("\t----------------------------------"));
  }
  else
  {
    Serial.println(F("\t----------------------------------"));
    Serial.println("\tSettup fail");
    Serial.println(F("\t----------------------------------"));
  }  
  delay(100);
}

void loop()
{
  getVoltage();
  //tinh gia tri Ucr-Usta:

  //điều khiển lcd
  BUTTON_light();

  //khiển tra nút nhấn
  BUTTON_back();
  BUTTON_up();
  BUTTON_down();

  //hiển thị lcd
  LCD_Display();

  //luu SD.
  SD_Save2file();

  //led cảnh báo  24accu2v
  LED_Warning_24accu2v();

    cur_time = millis();
  if ( cur_time - old_time >= 5000){
    SIM.HTTP_POST_start(url, data);
    old_time = cur_time;
  }
}
