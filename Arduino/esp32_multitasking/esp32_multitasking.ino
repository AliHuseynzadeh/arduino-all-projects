#include "libra.h"
long f2 = 0;
bool releStatus = false ;                                                ///true --> LOW -->yandi    //false--> HIGH --> sondu
bool timeStatus = 0;
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
    str += ",";
    str += releStatus;
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

class MyCallbacks:
  public BLECharacteristicCallbacks {
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
          Serial.println ("Yandi");
          releStatus = true;
        }
        if (value == "b") {
          Serial.println ("Sondu");
          releStatus = false;
        }
        if (releStatus) {
          digitalWrite(led, LOW);
        }
        else {
          digitalWrite(led, HIGH);
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
  pinMode(mot, INPUT);
  digitalWrite(led, HIGH);
  //////////////////////buzzer/////////////////////////
  ledcSetup(0, 1E5, 12);
  ledcAttachPin(15, 0);
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
void buzzerr() {
  ledcWriteTone(0, 800);
  delay(1000);
  uint8_t octave = 1;
  for (octave; octave < 255; octave ++) {
    ledcWriteNote(0, NOTE_C, octave);

  }
}

bool statuSender() {

}
void loop() {
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
    dht11();
  }
  Serial.print("timer f2 : ");

  Serial.println(f2);

  if (f2 > 10000) {
    timeStatus == 1;
    releStatus = true;
    digitalWrite(led, LOW);
    Serial.print("ikinci f     ");
    if (millis() % f2 == 0) {

      Serial.print("Led high low : ");
      releStatus = false;
      digitalWrite(led, HIGH);   //SO:N
      delay(1000);

    }
  }

  if (digitalRead(mot)) {
    Serial.print("hereket statusu: ");
    Serial.println(digitalRead(mot));
    releStatus = false;
    digitalWrite(led, HIGH);
    buzzerr();
  }
  else {
    ledcWriteTone(0, 0);
    if (timeStatus == 1) {
      releStatus = true;
      digitalWrite(led, LOW);
    }
    else {
      releStatus = false;
      digitalWrite(led, HIGH);
    }
  }

  Serial.print("releStatus: ");
  Serial.println(releStatus);

   delay(2000);
  dht11();
}
