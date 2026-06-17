#include "display.h"
#include "config.h"
#include "debug.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool displayReady = false;
String logLines[4] = {"", "", "", ""};

void initDisplay() {
    Wire.begin(OLED_SDA, OLED_SCL);
    // Address 0x3C is standard for 128x32 OLEDs
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        // Quitamos el DBG aquí para evitar bucle infinito si DBG llama a displayLog
        SerialMon.println("ERROR: Falla al inicializar OLED SSD1306");
        return;
    }
    
    displayReady = true;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Tracker Booting...");
    display.display();
    DBG("OLED inicializado correctamente");
}

void displayStatus(const String& networkStatus, const String& gpsStatus) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    display.setCursor(0, 0);
    display.print("RED: ");
    display.println(networkStatus);
    
    display.setCursor(0, 16);
    display.print("GPS: ");
    display.println(gpsStatus);
    
    display.display();
}

void displayMessage(const String& line1, const String& line2) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    display.setCursor(0, 0);
    display.println(line1);
    
    if (line2.length() > 0) {
        display.setCursor(0, 16);
        display.println(line2);
    }
    
    display.display();
}

void displayLog(const String& msg) {
    if (!displayReady) return;
    
    // Desplazar historial
    logLines[0] = logLines[1];
    logLines[1] = logLines[2];
    logLines[2] = logLines[3];
    logLines[3] = msg;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    
    for(int i = 0; i < 4; i++) {
        if (logLines[i].length() > 0) {
            display.println(logLines[i]);
        }
    }
    display.display();
}

void displayClear() {
    display.clearDisplay();
    display.display();
}
