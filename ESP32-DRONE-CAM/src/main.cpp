// main.cpp
#include "config.h"
#include "status.h"
#include "cam.h"
#include "ESPNow.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void setup() {
	Serial.begin(115200);

	WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

	setupESPNow();

	delay(1000);

	setupCamera();
	setupSDCard();

	pinMode(STATUS_LED_PIN, OUTPUT);

	Serial.println("Setup complete.");
}

void loop() {
	reportStatus();
}
