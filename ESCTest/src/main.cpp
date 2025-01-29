#include <Arduino.h>
#include <ESP32Servo.h>

const int ESC_PIN = 27;  // GPIO pin connected to the ESC
const int MIN_THROTTLE = 1000;  // Minimum throttle (1ms pulse width)
const int MAX_THROTTLE_20_PERCENT = 1200;  // Maximum throttle at 20% (1.6ms pulse width)
const int FREQUENCY = 50;  // 50Hz frequency for ESC

// Initialize the Servo object for ESC control
Servo escServo;

void setup() {
  // Start Serial communication for debugging
  Serial.begin(115200);
  Serial.println("Starting PWM control using ESP32Servo...");

  // Attach the ESC to the specified GPIO pin
  escServo.setPeriodHertz(FREQUENCY);  // Set frequency to 50Hz
  escServo.attach(ESC_PIN, MIN_THROTTLE, MAX_THROTTLE_20_PERCENT); // Attach to the ESC pin

  // Initial throttle setting (this will arm the ESC if it's idle)
  Serial.println("Sending minimum throttle for arming sequence...");
  escServo.writeMicroseconds(MIN_THROTTLE);  // Minimum throttle to arm the ESC
  delay(2000);  // Wait for arming sequence

  // Send zero throttle value to complete arming sequence
  Serial.println("Sending zero throttle to complete the arming sequence...");
  escServo.writeMicroseconds(MIN_THROTTLE);  // Zero throttle
  delay(1000);  // Allow ESC to register the zero throttle

  // Optionally, send max throttle to test the motor at full speed
  // Serial.println("Sending maximum throttle to test full speed...");
  // escServo.writeMicroseconds(MAX_THROTTLE_20_PERCENT);  // Max throttle (20% of full throttle)
  // delay(2000);  // Allow the motor to run at full speed

  // Send zero throttle again (or fail-safe throttle if desired)
  Serial.println("Sending zero throttle again.");
  escServo.writeMicroseconds(MIN_THROTTLE);  // Zero throttle again
}

void loop() {
  // You can implement logic to gradually increase or decrease the throttle here
  // Example: Ramp up throttle from min to 20% max
  
  // Ramp up throttle from minimum to 20% of maximum (1600 µs)
  for (int pulse = MIN_THROTTLE; pulse <= MAX_THROTTLE_20_PERCENT; pulse += 10) {
    escServo.writeMicroseconds(pulse);
    Serial.print("Throttle: ");
    Serial.println(pulse);
    delay(50);  // Ramp up slowly
  }
  
  delay(2000); // Hold throttle for 2 seconds
  
  // Ramp down throttle from 20% of maximum to minimum
  for (int pulse = MAX_THROTTLE_20_PERCENT; pulse >= MIN_THROTTLE; pulse -= 10) {
    escServo.writeMicroseconds(pulse);
    Serial.print("Throttle: ");
    Serial.println(pulse);
    delay(50);  // Ramp down slowly
  }
  
  delay(2000); // Hold minimum throttle for 2 seconds
}
