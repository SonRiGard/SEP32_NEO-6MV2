#include <TinyGPSPlus.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
// The TinyGPSPlus object
TinyGPSPlus gps;
void displayInfo();
void updateSerial();
void displayInfoBT();

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(3000);
}


void loop() {
  //updateSerial();
  while (Serial2.available() > 0){
    if (gps.encode(Serial2.read())){
      displayInfo();
      displayInfoBT();
    }
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid()){
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Lng: ");
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }  
  else
  {
    Serial.print(F("INVALID"));
  }
}


void displayInfoBT()
{
  char str[30]={};
  sprintf(str, "Location: " );
  SerialBT.write((uint8_t*)str, strlen(str));
  if (gps.location.isValid()){
    sprintf(str, "Lat:%i,Lng: %i\n",gps.location.lat(), gps.location.lng());
    SerialBT.write((uint8_t*)str, strlen(str));
  }  
  else
  {
    sprintf(str, "INVALID\n");
    SerialBT.write((uint8_t*)str, strlen(str));
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
 while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}