void setup() {
  // 13 = Built-in LED on Arduino LEONARDO
  pinMode(13, OUTPUT);
  // Serial1 = 0 -> RX1 pin
  Serial1.begin(115200);
}

void loop() {
  if(Serial1.available()){
    char c = Serial1.read();
    if(c == 'p'){
        digitalWrite(13, HIGH);
    }else if(c == 'g'){
        digitalWrite(13, LOW);
    }
    Serial.print(c);
  }    
}
