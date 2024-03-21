#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

RTC_DS3231 rtc;
File myFile;

LiquidCrystal_I2C lcd(0x27, 20, 4);
int relayPin = 7;
int startButtonPin = 5;
int pinCS = 6;

int yearsNow;
int monthsNow;
int daysNow;
int minutesNow;
int secondsNow;
int hoursNow;

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(relayPin, OUTPUT);
  pinMode(startButtonPin, INPUT_PULLUP);
  digitalWrite(relayPin, HIGH);

  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("SENSOR BENZOPYRENE");
  lcd.setCursor(4, 2);
  lcd.print("Kel 4 AQAI");
  delay(3000);
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("Tekan Tombol Untuk");
  lcd.setCursor(2, 2);
  lcd.print("Memulai Sampling");

  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(F(__DATE__ " " __TIME__)));

  if (SD.begin(pinCS)) {
    Serial.println("SD card is ready to use.");
  } else {
    Serial.println("SD card initialization failed");
    return;
  }
}

void loop() {
  if (digitalRead(startButtonPin) == LOW) {
    for (long k = 1; k++;) {

      digitalWrite(relayPin, LOW);

      DateTime now = rtc.now();
      yearsNow = now.year();
      monthsNow = now.month();
      daysNow = now.day();
      minutesNow = now.minute();
      secondsNow = now.second();
      hoursNow = now.hour();
      String dateTimeStr = String(yearsNow) + "-" + String(monthsNow) + "-" + String(daysNow) + " " + String(hoursNow) + ":" + String(minutesNow) + ":" + String(secondsNow);
      Serial.println(dateTimeStr);

      int MQ135 = analogRead(A3);
      int MQ138 = analogRead(A2);
      int MQ4 = analogRead(A1);
      int MQ6 = analogRead(A0);

      String msg = "*" + String(MQ6) + "," + String(MQ4) + "," + String(MQ138) + "," + String(MQ135) + "," + String(k) + "#";
      Serial.print(msg);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MQ6   = " + String(MQ6));
      lcd.setCursor(14, 0);
      lcd.print("PPM");
      lcd.setCursor(0, 1);
      lcd.print("MQ4   = " + String(MQ4));
      lcd.setCursor(14, 1);
      lcd.print("PPM");
      lcd.setCursor(0, 2);
      lcd.print("MQ138 = " + String(MQ138));
      lcd.setCursor(14, 2);
      lcd.print("PPM");
      lcd.setCursor(0, 3);
      lcd.print("MQ135 = " + String(MQ135));
      lcd.setCursor(14, 3);
      lcd.print("PPM");

      Serial3.print(msg);

      Serial.println(".");
      Serial.println("[Arduino] MQ6 = " + String(MQ6) + " PPM");
      Serial.println("[Arduino] MQ4 = " + String(MQ4) + " PPM");
      Serial.println("[Arduino] MQ138 = " + String(MQ138) + " PPM");
      Serial.println("[Arduino] MQ135 = " + String(MQ135) + " PPM");
      Serial.println("[Arduino] Sampling Ke-" + String(k));

      myFile = SD.open("mq135.txt", FILE_WRITE);
      if (myFile) {
        myFile.print(dateTimeStr);
        myFile.print(",");
        myFile.println(String(MQ135));
        Serial.println("succeeded opening.txt");
        lcd.setCursor(18, 3);
        lcd.print("OK");
        myFile.close();
      } else {
        Serial.println("error opening.txt");
        lcd.setCursor(18, 3);
        lcd.print("X");
      }
      myFile = SD.open("mq138.txt", FILE_WRITE);
      if (myFile) {
        myFile.print(dateTimeStr);
        myFile.print(",");
        myFile.println(String(MQ138));
        Serial.println("succeeded opening.txt");
        lcd.setCursor(18, 2);
        lcd.print("OK");
        myFile.close();
      } else {
        Serial.println("error opening.txt");
        lcd.setCursor(18, 2);
        lcd.print("X");
      }
      myFile = SD.open("mq4.txt", FILE_WRITE);
      if (myFile) {
        myFile.print(dateTimeStr);
        myFile.print(",");
        myFile.println(String(MQ4));
        Serial.println("succeeded opening.txt");
        lcd.setCursor(18, 1);
        lcd.print("OK");
        myFile.close();
      } else {
        Serial.println("error opening.txt");
        lcd.setCursor(18, 1);
        lcd.print("X");
      }
      myFile = SD.open("mq6.txt", FILE_WRITE);
      if (myFile) {
        myFile.print(dateTimeStr);
        myFile.print(",");
        myFile.println(String(MQ6));
        Serial.println("succeeded opening.txt");
        lcd.setCursor(18, 0);
        lcd.print("OK");
        myFile.close();
      } else {
        Serial.println("error opening.txt");
        lcd.setCursor(18, 0);
        lcd.print("X");
      }

      if (Serial3.available())  //apabila serial 3 available, akan ngeread msg yg udh dikirim
      {
        String Data = "";  // Baca Data yang dikirim dari ESP
        while (Serial3.available() > 0) {
          Data += char(Serial3.read());
          delay(50);
        }
        Serial.println(Data);
      }

      delay(1000);
    }
  }

  else {
    digitalWrite(relayPin, HIGH);
  }
}