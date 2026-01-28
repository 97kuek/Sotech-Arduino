const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration of the echo pulse
  // Timeout: 30ms (approx 5m range)
  // Distance = Duration * 0.034 / 2
  // Max distance 500cm.
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    // Timeout or out of range (pulseIn returns 0 on timeout)
    Serial.println("-1");
  } else {
    float distance = (duration * 0.034) / 2;
    if (distance > 500) {
      Serial.println("-1");
    } else {
      Serial.println(distance);
    }
  }

  delay(100);
}
