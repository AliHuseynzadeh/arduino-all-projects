#include <SPI.h>
#include <LoRa.h>

int counter = 0;
int inPin = 2;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status

void setup() {
  Serial.begin(9600);
  pinMode(inPin, INPUT);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E1)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
 val = digitalRead(inPin);  // read input value
  if (val == HIGH) { 
    counter++;
    Serial.println("magnet var");  
  } else {
     Serial.println("yoxdur");  
  }
LoRa.beginPacket();
  LoRa.print("Miqdar ");
  LoRa.print(counter);
  LoRa.endPacket();
  delay(400);
 // if(counter==10){ m++;  counter=0;  }
}
