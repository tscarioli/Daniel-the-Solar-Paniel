// Daniel the Solar Paniel code
// by tscarioli
// 12/05/2023

// INA 219 libraries
// source: http://scipia.co.kr/cms/blog/248?ckattempt=3
#include <Wire.h>
#include <INA219_WE.h>
#define I2C_ADDRESS 0x40

INA219_WE ina219(I2C_ADDRESS);
//temperature sensor (DHT) library
#include <DHT11.h>
//digital pin 8
DHT11 dht11(8);

//SD card sensor library
#include <SPI.h>
#include <SD.h>
File myFile;
const int chipSelect = 10;

//display library
#include <LiquidCrystal.h>
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  Serial.begin(9600);
  Wire.begin();
  if (!ina219.init()) {
    Serial.println("INA219 not connected!");
  }
  Serial.println("INA219 Current Sensor with solar panel");

  lcd.begin(16, 2);//starts display
  lcd.setCursor(0,0);

  Serial.print("Initializing SD card...");
  //prints error message if SD doesn't work
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    lcd.print("SD card not working"); //prints the message to the display
    return;
  }
  Serial.println("initialization done.");
  lcd.print("SD card working");
  delay(1000);


  myFile = SD.open("data.csv", FILE_WRITE); //opens data.csv
  if (myFile) {
    Serial.println("data.csv:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening data.csv");
  }
}

void loop() {
  lcd.clear();
  myFile = SD.open("data.csv", FILE_WRITE); //opens data.csv

  float current_mA = 0.0;
  //voltage measured by arduino
  int sensorValue = analogRead(A4);
  float voltage = sensorValue * (5.0 / 1023.0);
  //current from INA219
  current_mA = ina219.getCurrent_mA();
  float power_mW = (voltage * current_mA);

  //temperature sensor variables
  float temperature = dht11.readTemperature();
  float humidity = dht11.readHumidity();

  //time
  int time = millis()/1000;


  //INA 219 prints
  Serial.print("Voltage [V]: "); Serial.println(voltage);
  Serial.print("Current[mA]: "); Serial.println(current_mA);
  Serial.print("Power [mW]: "); Serial.println(power_mW);
  //DHT prints
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println("%");
  //time prints
  Serial.print("Time [S]: "); Serial.println(time);

  //time stamps
  myFile.print(time); myFile.print(",");
  //INA 219 SD prints
  myFile.print(voltage); myFile.print(",");
  myFile.print(current_mA); myFile.print(",");
  myFile.print(power_mW); myFile.print(",");
  //DHT prints
  myFile.print(temperature); myFile.print(",");
  myFile.print(humidity); myFile.println(",");

  Serial.println();
  myFile.close();
  
  lcd.setCursor(0,0);
  lcd.print("Power: "); lcd.print(power_mW);
  lcd.setCursor(0, 1);
  lcd.print("Temp: "); lcd.print(int(temperature)); lcd.print(" tm:"); lcd.print(time);

  delay(1000);
}
