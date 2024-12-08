#include <Arduino.h>

#define LEFT_VRX 27
#define LEFT_VRY 26

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("Hello World");

	pinMode(LEFT_VRX, INPUT);

	analogReadResolution(12); // Set the ADC resolution to 12 bits (0-4095)
	pinMode(LEFT_VRY, INPUT);
	pinMode(LEFT_VRY, INPUT);
}

void loop() {
	// put your main code here, to run repeatedly:
	int x = analogRead(LEFT_VRX);
	int y = analogRead(LEFT_VRY);

	Serial.print("X: ");
	Serial.print(x);
	Serial.print(" Y: ");
	Serial.println(y);

	delay(100);
}
