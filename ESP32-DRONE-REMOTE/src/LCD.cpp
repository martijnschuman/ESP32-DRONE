#include "LCD.h"

unsigned long previousMillis = 0; 			// For text display and LED blinking

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setupLCD() {
    lcd.begin(LCD_COLUMNS, LCD_ROWS);
    lcd.init();
    lcd.backlight();
}

void displayLCD(const char* message, int row, int column) {
    clearRow(row);
    lcd.setCursor(column, row);
    lcd.print(message);
}

void clearLCD() {
    lcd.clear();
}

void clearRow(int row) {
    lcd.setCursor(0, row);
    lcd.print("                ");
}

void calibrationMenu() {
    while (!leftCalibrated && !rightCalibrated) {
        unsigned long currentMillis = millis();

        // Display calibration instructions
        if (currentMillis - previousMillis >= LCD_REFRESH_INTERVAL) {
            previousMillis = currentMillis;

			static int step = 0; // Step for cycling through main loop messages
			switch (step) {
				case 0:
					displayLCD("!WARNING!", 0, 0);
					displayLCD("!No calibration!", 1, 0);
					break;
				case 1:
					displayLCD("Press go", 0, 0);
					displayLCD("to calibrate", 1, 0);
					break;
			}
			step = (step + 1) % 2; // Cycle through messages
        }

        delay(250); // Delay to prevent button debouncing

        // Check if ok button are pressed
        if (checkOKButton()) {
            startCalibrateJoysticks();
            break; // Exit loop after successful calibration
        }
    }
}

void connectToDroneDisplay() {
    displayLCD("Press OK to", 0, 0);
    displayLCD("connect to drone", 1, 0);

    delay(LCD_REFRESH_INTERVAL * 2);
}

void connectedToDroneDisplay() {
    displayLCD("Connected to", 0, 0);
    displayLCD("drone", 1, 0);
    delay(LCD_REFRESH_INTERVAL * 2);
}

void connectToDroneFailedDisplay() {
    displayLCD("Failed to", 0, 0);
    displayLCD("connect to drone", 1, 0);
    delay(LCD_REFRESH_INTERVAL * 2);
}

void joystickDebugDisplay() {
    displayLCD("Joystick debug", 0, 0);
    delay(LCD_REFRESH_INTERVAL * 1.5);

    while (true) {
        displayLCD(("LXY:" + String(getLeftJoystickX()) + "," + String(getLeftJoystickY())).c_str(), 0, 0);
        displayLCD(("RXY:" + String(getRightJoystickX()) + "," + String(getRightJoystickY())).c_str(), 1, 0);
        delay(250); // Refresh rate for joystick values
    }
}
