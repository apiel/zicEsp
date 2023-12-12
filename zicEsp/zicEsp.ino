#include "pin_config.h"

// For some weird reason this is not working...
// #define USER_SETUP_LOADED 1
// #include <User_Setups/Setup206_LilyGo_T_Display_S3.h>
//
// Therefor need to update directly the .pio/libdeps/zicEsp/TFT_eSPI/User_Setup_Select.h file

#include <SPI.h>
#include <TFT_eSPI.h>

#define TOUCH_MODULES_CST_SELF
#include <TouchLib.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);

int counter = 0;
void render()
{
    tft.fillScreen(TFT_BLACK);

    if (counter++ % 2 == 0) {
        // Draw some random filled ellipses
        for (int i = 0; i < 20; i++) {
            int rx = random(40);
            int ry = random(40);
            int x = rx + random(160 - rx - rx);
            int y = ry + random(128 - ry - ry);
            tft.fillEllipse(x, y, rx, ry, random(0xFFFF));
        }
    } else {
        // Draw some random outline ellipses
        for (int i = 0; i < 20; i++) {
            int rx = random(40);
            int ry = random(40);
            int x = rx + random(160 - rx - rx);
            int y = ry + random(128 - ry - ry);
            tft.drawEllipse(x, y, rx, ry, random(0xFFFF));
        }
    }
}

void setup(void)
{
    Serial.begin(115200);
    Serial.println("zicEsp");

    tft.init();
    tft.setRotation(1);

    Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
    if (!touch.init()) {
        Serial.println("Touch IC not found");
    }
}

unsigned long lastRender = 0;

void loop()
{
    if (touch.read()) {
        TP_Point t = touch.getPoint(0);
        Serial.printf("x:%04d y:%04d p:%04d \r\n", t.x, t.y, t.pressure);
    }

    unsigned long now = millis();
    if (now - lastRender > 2000) {
        lastRender = now;
        render();
    }

    delay(1);
}
