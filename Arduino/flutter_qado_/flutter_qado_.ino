#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <typeinfo>
using namespace std;
#include "DHT.h"

#define DHTPIN 22
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#define led 13

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
long f2 = 0;
int density = 0;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
bool releSatus ;

void dht11() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  else {
    String str = "";
    str += t;
    str += ",";
    str += h;
    pCharacteristic->setValue((char*)str.c_str());
    pCharacteristic->notify();

    Serial.println(h);
    Serial.println(F(" %  Temperature: "));
    Serial.println(t);
  }

}


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("**********  ");
        Serial.print("New value: ");
        if (value.length() > 4) {
          string s = value;

          //        Serial.println(value);
          for (int i = 0; i < value.length(); i++) {
            Serial.print(value[i]);

          }

          int n = s.length();
          char char_array[n + 1];
          strcpy(char_array, s.c_str());
          for (int i = 0; i < n; i++) {

            Serial.print(char_array[i]);
          }
          //  printf("float value : %1.0f\n" , (float) atoll(char_array));
          f2 = atoll(char_array);
          Serial.print("long  ");
          Serial.println(f2);

          //turnOff(f2);
        }
        if (value == "a") {
          digitalWrite(led, LOW);
          Serial.println ("Yandi");
          releSatus = true;
        }
        if (value == "b") {
          digitalWrite(led, HIGH);
          Serial.println ("Sondu");
          releSatus = false;
        }

        Serial.println();
        Serial.println("**********");
        dht11();

      }
      dht11();
    }

};

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  // Create the BLE Device
  BLEDevice::init("ESP32 GET NOTI FROM DEVICE");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");


  Serial.println(F("DHTxx test!"));

  dht.begin();
}
//
//void turnOff(long f)
//{ Serial.println(millis());
//  if (millis() % f == 0) {
//    Serial.println("timer activated");
//    digitalWrite(led, HIGH);
//    releSatus = true;
//    Serial.println(millis());
//  }
//}

void loop() {
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
    dht11();
  }
  Serial.print("timer f2 : ");

  Serial.println(f2);

  Serial.print("millis     ");
  Serial.println(millis());
  if (f2 > 10000) {

    Serial.print("ikinci f     ");
    if (millis() % f2 == 0) {
      Serial.print("Led high low : ");
      digitalWrite(led, LOW);
      releSatus = true;
      delay(1000);

    }
  }

 
Serial.print("releSatus: "); 
Serial.println(releSatus);
  
  //delay(1000);
  dht11();
}
