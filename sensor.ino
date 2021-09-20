
#include <Robojax_AllegroACS_Current_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

// voltage sensor A0,A1,A2,A3
const int voltageSensor1 = A0;
float vOUT1 = 0.0;
float vIN1 = 0.0;
int value1 = 0;
float R1 = 30000.0;
float R2 = 7500.0;

const int VIN = A5; // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5.04;// supply voltage
const int MODEL = 2;   // enter the model (see above list)
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};

RTC_DS3231 rtc;
Robojax_AllegroACS_Current_Sensor robojax(MODEL, VIN);
File myFile;

void setup()
{
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("RTC tidak terbaca");
    while (1);
  }
  if (rtc.lostPower()) {
    //atur waktu sesuai waktu pada komputer
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //atur waktu secara manual
    // January 21, 2019 jam 10:30:00
    // rtc.adjust(DateTime(2019, 1, 25, 10, 30, 0));
  }
  pinMode(53, OUTPUT);
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("Initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void loop() {
  DateTime now = rtc.now();
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);//hari
  Serial.print(" ");
  Serial.print(now.day(), DEC); //tanggal
  Serial.print('/');
  Serial.print(now.month(), DEC); //bulan
  Serial.print('/');
  Serial.print(now.year(), DEC); //tahun
  Serial.print(' ');
  Serial.print(now.hour(), DEC); //jam
  Serial.print(':');
  Serial.print(now.minute(), DEC); //tanggal
  Serial.print(':');
  Serial.print(now.second(), DEC); //detik
  // PLTS 1
  value1 = analogRead(voltageSensor1);
  vOUT1 = (value1 * 16.0) / 1024.0;
  vIN1 = vOUT1 / (R2 / (R1 + R2));
  Serial.print(",");
  Serial.print(robojax.getCurrentAverage(300), 3); //
  Serial.print(",");
  Serial.print(vIN1);
  Serial.print(" ");
  Serial.print(",");
  float P1 = vIN1 * (robojax.getCurrentAverage(300));
  Serial.print(P1);
  Serial.print(" ");
  Serial.println(",");
  myFile = SD.open("dataPLTS.csv", FILE_WRITE);
  if (myFile) {
    myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);//hari
    myFile.print(" ");
    myFile.print(now.day(), DEC); //tanggal
    myFile.print('/');
    myFile.print(now.month(), DEC); //bulan
    myFile.print('/');
    myFile.print(now.year(), DEC); //tahun
    myFile.print(' ');
    myFile.print(now.hour(), DEC); //jam
    myFile.print(':');
    myFile.print(now.minute(), DEC); //tanggal
    myFile.print(':');
    myFile.print(now.second(), DEC); //detik
    myFile.print(",");
    myFile.print(robojax.getCurrentAverage(300), 3); //
    myFile.print(",");
    myFile.print(vIN1);
    myFile.print(" ");
    myFile.print(",");
    myFile.print(P1);
    myFile.print(" ");
    myFile.println(",");
    myFile.close(); // close the file
  }
  delay(1000);
}
