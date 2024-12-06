#include <Arduino.h>

int LED_PIN = 4;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	Serial.println("Hello World");

	pinMode(LED_PIN, OUTPUT);
}

void loop() {
	// put your main code here, to run repeatedly:
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);
	delay(1000);
}
