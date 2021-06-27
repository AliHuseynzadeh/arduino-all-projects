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

// Переменная для хранения HTTP-запроса
String header;

// Вспомогательные переменные для сохранения текущего состояния вывода
String output26State = "off";

// Назначение выходных переменных для контактов GPIO
const int output26 = 26;


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
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p { font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #50B8B4; color: white; font-size: 1rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .reading { font-size: 1.4rem; }
    .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
    .button2 {background-color: #555555
  </style>
</head>
<body>
  <div class="topnav">
    <h1>Birthday Suprise</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p><i class="fas fa-thermometer-half" style="color:#059e8a;"></i> Istilik</p><p><span class="reading"><span id="temp">%TEMPERATURE%</span> &deg;C</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-tint" style="color:#00add6;"></i> Rutubet</p><p><span class="reading"><span id="hum">%HUMIDITY%</span> &percnt;</span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);

 source.addEventListener('temperature', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temp").innerHTML = e.data;
 }, false);

 source.addEventListener('humidity', function(e) {
  console.log("humidity", e.data);
  document.getElementById("hum").innerHTML = e.data;
 }, false);

}
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);
  // Statik ip yapılandırması
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Statik IP ayarlanamadı");
  }
  initWiFi();
  // Инициализировать выходные переменные в качестве выходов
  pinMode(output26, OUTPUT);
 // pinMode(output27, OUTPUT);
  // Установить выходы в низкое логическое состояние
  digitalWrite(26, HIGH);
  //digitalWrite(output27, LOW);

  //WiFiClient client = server.available();   // Слушаем входящих клиентов
  if (client) {                             // Если новый клиент подключился,
    Serial.println("New Client.");          // печатать сообщение в последовательный порт
    String currentLine = "";                // сделать строку (String) для хранения входящих данных от клиента
    while (client.connected()) {            // повторять, пока клиент подключается
      if (client.available()) {             // если есть байты для чтения от клиента,
        char c = client.read();             // прочитать байт, затем
        Serial.write(c);                    // напечатать в послед. монитор
        header += c;
        if (c == '\n') {                    // если байт является символом новой строки
          // если текущая строка пуста, у вас есть два символа новой строки подряд.
          // это конец HTTP-запроса клиента, поэтому отправить ответ:
          if (currentLine.length() == 0) {
            // Заголовки HTTP всегда начинаются с кода ответа (например, HTTP / 1.1 200 OK)
            // и контент-тип (Content-type), чтобы клиент знал, что ждет, а затем пустая строку:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // включам и выключаем GPIO
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(26, LOW);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(26, HIGH);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            // Отобразить текущее состояние и кнопки ON / OFF для GPIO 26
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // Если output26State выключен, отображаем кнопку ON    
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

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
