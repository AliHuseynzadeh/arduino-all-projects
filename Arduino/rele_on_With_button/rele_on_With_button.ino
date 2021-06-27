const int LED = 12;
const int BUTTON = 9;
int buttonState = 0;
unsigned long a = 10000;
unsigned long lastTime;
bool but=0;
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  buttonState = digitalRead(BUTTON);
  if (buttonState == HIGH) {
    Serial.println("LED++");
    digitalWrite(LED, HIGH);
    but=1;
 }
  else {
  //  Serial.println("LED--");
  
  }
  if(but==1){
  if (millis() - lastTime == a)
    { Serial.println(lastTime);
      Serial.println("vaxt tamam oldu");
      lastTime = millis();
      Serial.println(lastTime);
        digitalWrite(LED, LOW);
      but=0;
    }}
}
