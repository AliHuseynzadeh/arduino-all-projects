const int LED = 12;
const int BUTTON = 9;
int buttonState = 0;
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
  }
  else {
    Serial.println("LED--");
    digitalWrite(LED, LOW);
  }
}
