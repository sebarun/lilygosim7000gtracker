#include "config.h"
#include "gpsManager.h"
#include "debug.h"

extern TinyGsm modem;

//float lati = 0, longi = 0;

bool getGPSFix() {
  modem.enableGPS();
  float flat = 0, flon = 0;
  if (modem.getGPS(&flat, &flon)) {
    lati = flat;
    longi = flon;
    DBG("GPS FIX: Lat=%.6f, Lon=%.6f", lati, longi);
    return true;
  }
  return false;
}
