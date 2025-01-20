// main.cpp
#include "config.h"
#include "status.h"
#include "cam.h"
#include "ESPNow.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

CameraPacket cameraPacket = {};
CameraMode status = CAM_BOOT;               // Set initial status
unsigned int pictureNumber = 0;

void setup() {
	Serial.begin(115200);

	WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

	// Initialize EEPROM with defined size
    if (!EEPROM.begin(EEPROM_SIZE)) {
        Serial.println("Failed to initialize EEPROM");
        while (1); // Stop execution if EEPROM fails
    }

	// Load pictureNumber from EEPROM
    pictureNumber = EEPROM.readUInt(PICTURE_NUMBER_ADDR);
    Serial.print("Loaded pictureNumber from EEPROM: ");
    Serial.println(pictureNumber);

	setupESPNow();

	delay(2500);

	setupCamera();
	setupSDCard();

	pinMode(STATUS_LED_PIN, OUTPUT);

	Serial.println("Setup complete.");
}

void loop() {
	reportStatus();
}
