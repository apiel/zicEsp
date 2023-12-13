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

#include <WiFi.h>

const char* ssid = "alex";
const char* password = "SuperStar86";
WiFiServer server(5555);
WiFiClient client;
bool connected = false;

TFT_eSPI tft = TFT_eSPI();
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);

void setup(void)
{
    Serial.begin(115200);
    // Serial.begin(230400);
    // Serial1.begin(460800);
    // Serial1.begin(921600);
    // Serial2.begin(921600);
    // Serial2.begin(9600);
    Serial.println("zicEsp");
    // Serial.setTimeout(10);

    tft.init();
    tft.setRotation(1);

    Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
    if (!touch.init()) {
        Serial.println("Touch IC not found");
    }

    tft.fillScreen(TFT_BLACK);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("...");
    }
    Serial.print("WiFi connected with IP:");
    Serial.println(WiFi.localIP());

    server.begin();
}

const byte numChars = 128;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

void recvWithEndMarker()
{
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        } else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void parseData(String str)
{
    // split the string with spaces
    str.trim();
    int index = str.indexOf(' ');
    // Serial.printf("index: %d\r\n", index);
    if (index != -1) {
        String cmd = str.substring(0, index);
        String val = str.substring(index + 1);
        // Serial.printf("cmd: %s val: %s\r\n", cmd.c_str(), val.c_str());
        if (cmd == "drawRect") {
            // drawRect 10 10 100 100 0x07FF
            // drawRect 50 50 100 100 0xF800
            // tft.fillScreen(TFT_BLACK);

            const char* ptr = val.c_str();
            int32_t x = atoi(strtok((char*)ptr, " "));
            int32_t y = atoi(strtok(NULL, " "));
            int32_t w = atoi(strtok(NULL, " "));
            int32_t h = atoi(strtok(NULL, " "));
            uint32_t color = strtoul(strtok(NULL, " "), NULL, 16);
            tft.drawRect(x, y, w, h, color);
        } else if (cmd == "drawRectLoop") {
            for (int i = 0; i < 100; i++) {
                tft.drawRect(100 + i, 10, 100, 100, 0x07FF);
                delay(10);
                tft.drawRect(100 + i, 10, 100, 100, TFT_BLACK);
            }
        } else {
            Serial.printf("Unknown command: %s\r\n", str.c_str());
        }
    } else {
        Serial.printf("Received: %s\r\n", str.c_str());
    }
}

void showNewData()
{
    if (newData == true) {
        // Serial.print("This just in ... ");
        // Serial.println(receivedChars);
        newData = false;

        String str = String(receivedChars);
        parseData(str);
    }
}

void loop()
{
    if (touch.read()) {
        TP_Point t = touch.getPoint(0);
        Serial.printf("x:%04d y:%04d p:%04d \r\n", t.x, t.y, t.pressure);
        // log_e("log_e");
        Serial.println(WiFi.localIP());
    }

    // if (Serial.available()) {
    //     String str = Serial.readString();

    //     // split the string with spaces
    //     str.trim();
    //     int index = str.indexOf(' ');
    //     // Serial.printf("index: %d\r\n", index);
    //     if (index != -1) {
    //         String cmd = str.substring(0, index);
    //         String val = str.substring(index + 1);
    //         // Serial.printf("cmd: %s val: %s\r\n", cmd.c_str(), val.c_str());
    //         if (cmd == "drawRect") {
    //             // drawRect 10 10 100 100 0x07FF
    //             // drawRect 50 50 100 100 0xF800
    //             tft.fillScreen(TFT_BLACK);

    //             const char* ptr = val.c_str();
    //             int32_t x = atoi(strtok((char*)ptr, " "));
    //             int32_t y = atoi(strtok(NULL, " "));
    //             int32_t w = atoi(strtok(NULL, " "));
    //             int32_t h = atoi(strtok(NULL, " "));
    //             uint32_t color = strtoul(strtok(NULL, " "), NULL, 16);
    //             tft.drawRect(x, y, w, h, color);
    //         } else if (cmd == "drawRectLoop") {
    //             for (int i = 0; i < 100; i++) {
    //                 tft.drawRect(100 + i, 10, 100, 100, 0x07FF);
    //                 delay(10);
    //                 tft.drawRect(100 + i, 10, 100, 100, TFT_BLACK);
    //             }
    //         } else {
    //             Serial.printf("Unknown command: %s\r\n", str.c_str());
    //         }
    //     } else {
    //         Serial.printf("Received: %s\r\n", str.c_str());
    //     }
    // }

    // recvWithEndMarker();
    // showNewData();

    if (!connected) {
        // listen for incoming clients
        client = server.available();
        if (client) {
            Serial.println("Got a client !");
            if (client.connected()) {
                Serial.println("and it's connected!");
                connected = true;
            } else {
                Serial.println("but it's not connected!");
                client.stop(); // close the connection:
            }
        }
    } else {
        if (client.connected()) {
            while (client.available()) {
                // Serial.write(client.read());
                String str = client.readString();
                parseData(str);
                Serial.printf("NC Received: %s\r\n", str.c_str());
            }



            // while (Serial.available()) {
            //     char r = Serial.read();
            //     Serial.write(r); // local echo
            //     client.write(r);
            // }
        } else {
            Serial.println("Client is gone");
            client.stop(); // close the connection:
            connected = false;
        }
    }

    delay(1);
}
