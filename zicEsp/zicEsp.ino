/*
  An example showing rainbow colours on a 1.8" TFT LCD screen
  and to show a basic example of font use.

  Make sure all the display driver and pin connections are correct by
  editing the User_Setup.h file in the TFT_eSPI library folder.

  Note that yield() or delay(0) must be called in long duration for/while
  loops to stop the ESP8266 watchdog triggering.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

#include "pin_config.h"

// For some weird reason this is not working...
// #define USER_SETUP_LOADED 1
// #include <User_Setups/Setup206_LilyGo_T_Display_S3.h>
//
// Therefor need to update directly the .pio/libdeps/zicEsp/TFT_eSPI/User_Setup_Select.h file

#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

void setup(void)
{
    Serial.begin(115200);
    Serial.println("zicEsp");

    tft.init();
    tft.setRotation(1);
}

void loop()
{
    Serial.print("Yo ");
    Serial.print(millis());
    Serial.println(" millis");

    tft.fillScreen(TFT_BLACK);

    // Draw some random filled ellipses
    for (int i = 0; i < 20; i++) {
        int rx = random(40);
        int ry = random(40);
        int x = rx + random(160 - rx - rx);
        int y = ry + random(128 - ry - ry);
        tft.fillEllipse(x, y, rx, ry, random(0xFFFF));
    }

    delay(2000);
    tft.fillScreen(TFT_BLACK);

    // Draw some random outline ellipses
    for (int i = 0; i < 20; i++) {
        int rx = random(40);
        int ry = random(40);
        int x = rx + random(160 - rx - rx);
        int y = ry + random(128 - ry - ry);
        tft.drawEllipse(x, y, rx, ry, random(0xFFFF));
    }

    delay(2000);
}
