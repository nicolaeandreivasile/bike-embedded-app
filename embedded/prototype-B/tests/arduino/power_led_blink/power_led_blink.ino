/*
 * Power LED blink test
 */

const int ledPin = 4;

void setup() {
  // Set GPIO4 as an output pin
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Turn off the LED
  digitalWrite(ledPin, LOW);
  delay(1000);

  // Turn on the LED
  digitalWrite(ledPin, HIGH);
  delay(1000);
}
