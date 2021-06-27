const byte output26 = 27;
const byte interruptPin = 15;
volatile byte state = LOW;
void IRAM_ATTR isr(){ digitalWrite(26, LOW);
    Serial.println("inteerupt isledi");
}
void setup() {
  Serial.begin(9600);
  pinMode(output26, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr, CHANGE);
}

void loop() {
   int sensorvalue = digitalRead(interruptPin);
  Serial.println(sensorvalue);
  
  digitalWrite(26, HIGH);
}
