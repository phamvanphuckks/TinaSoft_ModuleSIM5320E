#ifndef __MAIN_H__
#define __MAIN_H__

//khai báo senser điện áp;
#define analog0 A0
#define analog2 A2
#define analog4 A4
#define analog6 A6
#define analog8 A8
#define analog10 A10
#define analog12 A12
#define analog14 A14

#define CS_PIN 32
#define CLOCK_PIN 38
#define MOSI_PIN 34
#define MISO_PIN 36
#define CS_PIN1 40
#define CLOCK_PIN1 46
#define MOSI_PIN1 42
#define MISO_PIN1 44

float UT = 0.0, UT1 = 0.0, UT2 = 0.0, UT3 = 0.0, UT4 = 0.0;

float U1 = 0.0;
int kalman1;
float U2 = 0.0;
int kalman2;
float U3 = 0.0;
int kalman3;
float U4 = 0.0;
int kalman4;
float U5 = 0.0;
int kalman5;
float U6 = 0.0;
int kalman6;
float U7 = 0.0;
int kalman7;
float U8 = 0.0;
int kalman8;
float U9 = 0.0;
int kalman9;
float U10 = 0.0;
int kalman10;
float U11 = 0.0;
int kalman11;
float U12 = 0.0;
int kalman12;
float U13 = 0.0;
int kalman13;
float U14 = 0.0;
int kalman14;
float U15 = 0.0;
int kalman15;
float U16 = 0.0;
int kalman16;
float U17 = 0.0;
int kalman17;
float U18 = 0.0;
int kalman18;
float U19 = 0.0;
int kalman19;
float U20 = 0.0;
int kalman20;
float U21 = 0.0;
int kalman21;
float U22 = 0.0;
int kalman22;
float U23 = 0.0;
int kalman23;
float U24 = 0.0;
int kalman24;

const float R1 = 100000.0;
const float R2 = 10000.0;

//khai báo dòng;
int analog5 = A5;
const float vcc = 5.00;
const float factor = 0.02;
float current = 0;

//khai báo đo nhiệt độ;
int analog1 = A1;
int analog3 = A3;
float T0 = 0;
float T1 = 0;

//nut nhan
int nutnhan;
int pos = 0;
int tt = 15;
int button_up = 14;
int button_down = 16;
int button_back = 18;
int button_ligt = 22;
//điều khienr màn lcd;
int led = 24;

//led đơn
#define led1  26
#define led2  28
#define led3  30
#define led4  37
#define led5  39
#define led6  43
#define led7  3
#define led8  5
#define led9  7
#define led10  9
#define led11  11
#define led12  13
#define led13  15
#define led14  17
#define led15  19
#define led16  23
#define led17  25
#define led18  27
#define led19  29
#define led20  31
#define led21  33
#define led22  35
#define led23  45
#define led24  41

int LED_Array[25] = {
    led, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10,
    led11, led12, led13, led14, led15, led16, led17, led18, led19, led20,
    led21, led22, led23, led24};
    
unsigned long time;
unsigned long time1;
unsigned long time2;
unsigned long time_nnlingt;
//time canh bao led
unsigned long time3, time4, time5, time6, time7, time8, time9;
unsigned long time10, time11, time12, time13, time14, time15, time16, time17, time18, time19;
unsigned long time20, time21, time22, time23, time24, time25, time26;
//time Ucr-Usta
unsigned long t1;

#endif
