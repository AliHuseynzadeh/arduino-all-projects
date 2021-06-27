
unsigned long lastTime;
unsigned long a=10000;
void setup() {
  Serial.begin(9600);
    Serial.println("SYSTEM STARED");
}

void loop() {

//Serial.println(millis());
if(millis() - lastTime == a)
 { Serial.println(lastTime);
  Serial.println("1 saat tamam oldu");
 // lastTime = millis();
    Serial.println(lastTime);
   delay(500);
   }
}
