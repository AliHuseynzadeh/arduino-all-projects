//MucitPilot 2021/////
//ESP-32 Web Server Örneği
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>//https://github.com/khoih-prog/AsyncHTTPRequest_Generic/archive/master.zip
#include <ESPAsyncWebServer.h>//https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor
#include <DHT.h>//https://github.com/adafruit/DHT-sensor-library

// Ağ adı ve şifrenizi girin
const char* ssid = "TRONIQ";
const char* password = "AvengersOfTroniq";

// 80 portunu dinleyen  Bir AsyncWebServer nesnesi yaratıyoruz
AsyncWebServer server(80);

//  /events adresinde bir EventSource nesnesi yaratıyoruz
AsyncEventSource events("/events");

// Zamanlama ile ilgili değişkenler
unsigned long sonZaman = 0;
unsigned long beklemeSuresi = 3000; //5sn


//sıcaklık ve nem değişkenini tanımlıyoruz
float temperature;
float humidity;

//sabit ip için gerekli bilgiler///
// Sabit IP adresini girin
IPAddress local_IP(192, 168, 1, 88);
// Gateway IP adresini girin
IPAddress gateway(192, 168, 1, 254);
//subnetmask adresini girin
IPAddress subnet(255, 255, 255, 0);


//////////////DHT SENSÖR AYAR BLOĞU////////////////////
// DHT sensör GPIO14 pinine bağlı
#define DHTPIN 15
// DHT tipini seçin
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE); //bir dht nesnesi oluşturuyoruz
//////////////////////////////////////////////////////////

///sensörden veri okuma fonksiyonu
void sensorOku() {
  humidity = dht.readHumidity();
  delay(100);
  //veri okunamadıysa
  if (isnan(humidity)) {
    Serial.println("DHT sensoru rutubet  oxunamadı!!!");
    humidity = 0.0;
  }
  temperature = dht.readTemperature();
  delay(100);
  //veri okunamadıysa
  if (isnan(temperature)) {
    Serial.println("DHT sensoru istilik oxunamadı!!!");
    temperature = 0.0;
  }

}

// Wifi bağlantı fonksiyonu
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Wifiye bağlanır..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());//aldığı ip adresini yazdırıyoruz
}

String processor(const String& var) {
  sensorOku();
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return String(temperature);
  }
  else if (var == "HUMIDITY") {
    return String(humidity);
  }

  return String();
}

//sunucu sayfası için gerekli HTML kodları içeren fonksiyon
const char index_html[] PROGMEM = R"rawliteral(
%TEMPERATURE%,%HUMIDITY%
)rawliteral";

void setup() {
  Serial.begin(115200);
  // Statik ip yapılandırması
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Statik IP ayarlanamadı");
  }
  initWiFi();



  // Web Server'a gelen istekler için gerekli kısım
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Web server Event'ler için
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial.printf("İstemci Yeniden Baglandi! Aldigi son mesaj ID'si: %u\n", client->lastId());
    }

    client->send("Merhaba!", NULL, millis(), 5000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  if ((millis() - sonZaman) > beklemeSuresi) {//sensörden belirlenmiş zaman aralığında veri okuyoruz
    sensorOku();
    Serial.printf("Sıcaklık = %.2f ºC \n", temperature);
    Serial.printf("Nem = %.2f \n", humidity);
    Serial.println();

    // Sensörden okunan bilgileri EVENT olarak sunucuya gönder
    events.send("ping", NULL, millis());
    events.send(String(temperature).c_str(), "temperature", millis());
    events.send(String(humidity).c_str(), "humidity", millis());


    sonZaman = millis();
  }
}
