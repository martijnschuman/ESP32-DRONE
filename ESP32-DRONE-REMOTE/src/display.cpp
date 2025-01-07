#include "display.h"

unsigned long previousMillis = 0; 			// For text display and LED blinking

Adafruit_SSD1305 display(X_PIXELS, Y_PIXELS, &Wire, -1);

void setupDisplay() {
    // Initialize the display
    if (!display.begin(SSD1305_I2C_ADDRESS, true)) {
        Serial.println("SSD1309 allocation failed");
        while (1);
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    display.display();
    delay(2000);

    display.clearDisplay();
}
