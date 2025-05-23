//cam.cpp
#include "Cam.h"

//Stores the camera configuration parameters
camera_config_t config;

void setupCamera(){
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG; //YUV422,GRAYSCALE,RGB565,JPEG

    // Select lower framesize if the camera doesn't support PSRAM
    if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
        config.jpeg_quality = 10; //10-63 lower number means higher quality
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }
    
    // Initialize the Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        setStatus(CAM_INIT_ERROR);
        return;
    }

    sensor_t * s = esp_camera_sensor_get();
    s->set_brightness(s, 0);     // -2 to 2
    s->set_contrast(s, 0);       // -2 to 2
    s->set_saturation(s, 0);     // -2 to 2
    s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
    s->set_aec2(s, 0);           // 0 = disable , 1 = enable
    s->set_ae_level(s, 0);       // -2 to 2
    s->set_aec_value(s, 300);    // 0 to 1200
    s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0);       // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    s->set_bpc(s, 0);            // 0 = disable , 1 = enable
    s->set_wpc(s, 1);            // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
    s->set_lenc(s, 1);           // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
    s->set_vflip(s, 0);          // 0 = disable , 1 = enable
    s->set_dcw(s, 1);            // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
}

void setupSDCard(){
    if (!SD_MMC.begin()) {
        setStatus(SD_INIT_ERROR);
        return;
    }

    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE){
        setStatus(SD_INIT_ERROR);
        return;
    }
    Serial.printf("Card type: %s\n", (cardType == CARD_MMC ? "MMC" :
                                      cardType == CARD_SD ? "SDSC" :
                                      cardType == CARD_SDHC ? "SDHC" : "UNKNOWN"));
}

void takePicture(){
    // Path where new picture will be saved in SD Card
    String path = "/picture" + String(pictureNumber) +".jpg";  

    // Take Picture with Camera
    camera_fb_t  * fb = esp_camera_fb_get();

    if(!fb) {
        setStatus(CAMERA_TAKE_ERROR);
        return;
    }

    // Save picture to microSD card
    fs::FS &fs = SD_MMC; 
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file){
        setStatus(SD_SAVE_ERROR);
    } 
    else {
        file.write(fb->buf, fb->len); // payload (image), payload length
        setStatus(CAM_PICTURE_SAVED);
        Serial.printf("Saved file to path: %s\n", path.c_str());
    }
    file.close();

    // Increment and save pictureNumber to EEPROM
    pictureNumber++;
    EEPROM.writeUInt(PICTURE_NUMBER_ADDR, pictureNumber);
    EEPROM.commit(); // Save changes to EEPROM

    Serial.print("Saved pictureNumber to EEPROM: ");
    Serial.println(pictureNumber);
    
    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
}