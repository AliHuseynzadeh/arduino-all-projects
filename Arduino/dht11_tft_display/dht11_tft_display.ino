#include <DHT.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#define DHTPIN 17
#define DHTTYPE DHT11
#define BUTTON1PIN 35
#define BUTTON2PIN 0

DHT dht(DHTPIN, DHTTYPE);
TFT_eSPI tft = TFT_eSPI();

bool useFahrenheit       = true; // Default to Fahrenheit
bool showTemp            = true; // Default to Temp / Humidity
long lastDebounceButton1 = 0;    // Holds Button1 last debounce
long lastDebounceButton2 = 0;    // Holds Button2 last debounce
long debounceDelay       = 0;  // 200ms between re-polling

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON1PIN, INPUT);
  pinMode(BUTTON2PIN, INPUT);
  //attachInterrupt(BUTTON1PIN, toggleButton1, FALLING);
  //attachInterrupt(BUTTON2PIN, toggleButton2, FALLING);
  dht.begin();
  tft.begin();
  tft.setRotation(0); //Landscape
  
}

void loop() {
  float t = dht.readTemperature(/*useFahrenheit*/);
  float h = dht.readHumidity();
  float hi = dht.computeHeatIndex(t, h, useFahrenheit);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLUE);
  tft.setCursor(50, 120);
  tft.drawLine(20,100, 300, 100, TFT_BLUE);

  
  tft.setFreeFont(&Orbitron_Light_32);
  tft.setTextColor(TFT_RED);
  tft.setCursor(60, 140);//1UZUNLUQ , 2HUNDURLUK
  tft.print("ALI");
 }
