#include "config.h"
#include "gpsManager.h"
#include "debug.h"

extern TinyGsm modem;

void enableGPSHardware() {
  DBG("Powering on GPS Hardware (+CGPIO=0,48,1,1)...");
  modem.sendAT("+CGPIO=0,48,1,1");
  if (modem.waitResponse(10000L) != 1) {
    DBG("Set GPS Power HIGH Failed");
  }
  modem.enableGPS();
}

void disableGPSHardware() {
  DBG("Powering off GPS Hardware (+CGPIO=0,48,1,0)...");
  modem.sendAT("+CGPIO=0,48,1,0");
  if (modem.waitResponse(10000L) != 1) {
    DBG("Set GPS Power LOW Failed");
  }
  modem.disableGPS();
}

bool getGPSFix() {
  enableGPSHardware();

  DBG("Waiting for GPS fix (Timeout: 90s)...");
  pinMode(LED_PIN, OUTPUT);

  unsigned long start = millis();
  unsigned long timeout = 90000; // 90 seconds timeout
  float flat = 0, flon = 0;
  bool fix = false;

  while (millis() - start < timeout) {
    // Toggle LED to indicate searching
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    if (modem.getGPS(&flat, &flon)) {
      lati = flat;
      longi = flon;
      DBG("GPS FIX OBTAINED: Lat=%.6f, Lon=%.6f", lati, longi);
      fix = true;
      break;
    }
    delay(2000); // Wait 2 seconds before polling again
  }

  // Restore LED to off state (active-low LED_PIN is HIGH when off)
  digitalWrite(LED_PIN, HIGH);

  disableGPSHardware();
  return fix;
}

