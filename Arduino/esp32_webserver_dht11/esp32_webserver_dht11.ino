#include <WiFi.h>
#include <Wire.h>
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//DHT Sensor;
uint8_t DHTPin = 33;


// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity;

// Replace with your network credentials
const char* ssid = "Redmi"; // Enter SSID here
const char* password = "rafael666"; //Enter Password here

// Set web server port number to 80
WiFiServer server(80);
//------------on-off---------------------
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String output26State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
//-------------------------------------------------
// it wil set the static IP address to 192, 168, 1, 184
IPAddress local_IP(192, 168, 43, 2);
//it wil set the gateway static IP address to 192, 168, 1,1
IPAddress gateway(192, 168, 43, 1);

// Following three settings are optional
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, HIGH);
  pinMode(DHTPin, INPUT);
  dht.begin();

  // This part of code will try create static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
 }
 


void loop()
{
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client)
  { // If a new client connects,

    String request = client.readStringUntil('\r');
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    //on-off
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects,
      currentTime = millis();
      previousTime = currentTime;
      Serial.println("New Client.");          // print a message out in the serial port
      String currentLine = "";
      while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          header += c;
          if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              // turns the GPIOs on and off
              if (header.indexOf("GET /26/on") >= 0) {
                Serial.println("GPIO 26 on");
                output26State = "on";
                digitalWrite(26, LOW);
              } 
              else if (header.indexOf("GET /26/off") >= 0) {
                Serial.println("GPIO 26 off");
                output26State = "off";
                digitalWrite(output26, HIGH);

              }
              // Display current state, and ON/OFF buttons for GPIO 26
              client.println("<p>GPIO 26 - State " + output26State + "</p>");
              // If the output26State is off, it displays the ON button
              if (output26State == "off") {
                client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
              } 
              else {
                client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
              //---------------

              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<meta http-equiv=\"refresh\" content=\"2\">");
              client.println("<script>\n");
              client.println("setInterval(loadDoc,200);\n");
              client.println("function loadDoc() {\n");
              client.println("var xhttp = new XMLHttpRequest();\n");
              client.println("xhttp.onreadystatechange = function() {\n");
              client.println("if (this.readyState == 4 && this.status == 200) {\n");
              client.println("document.getElementById(\"webpage\").innerHTML =this.responseText}\n");
              client.println("};\n");
              client.println("xhttp.open(\"GET\", \"/\", true);\n");
              client.println("xhttp.send();\n");
              client.println("}\n");
              client.println("</script>\n)");
              // CSS to style the table

              client.println("<style>body { text-align: center; font-family: \"Arial\", Arial;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #555555;}");
              client.println("table { border-collapse: collapse; width:40%; margin-left:auto; margin-right:auto;border-spacing: 2px;background-color: white;border: 4px solid green; }");
              client.println("th { padding: 20px; background-color: #008000; color: white; }");
              client.println("tr { border: 5px solid green; padding: 2px; }");
              client.println("tr:hover { background-color:yellow; }");
              client.println("td { border:4px; padding: 12px; }");
              client.println(".sensor { color:black; font-weight: bold;  padding: 1px; }");

              // Web Page Heading
              client.println("</style></head><body><h1>Uv-Box</h1>");
              client.println("<h2>Tempratur nəzarət və idarəemə</h2>");
              client.println("<h2>Microcontrollerslab.com</h2>");
              client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
              client.println("<tr><td>Temp. Selsi </td><td><span class=\"sensor\">");
              client.println(dht.readTemperature());
              client.println(" *C</span></td></tr>");
              client.println("<tr><td>Temp. Ferenheyt</td><td><span class=\"sensor\">");
              client.println(1.8 * dht.readTemperature() + 32);
              client.println(" *F</span></td></tr>");
              client.println("<tr><td>Nəmlik</td><td><span class=\"sensor\">");
              client.println(dht.readHumidity());
              client.println(" %</span></td></tr>");
              client.println("</body></html>");

              client.println();
              Serial.println("Client disconnected.");
              Serial.println("");
            }
          }
        }
      }
    }
  }
}
