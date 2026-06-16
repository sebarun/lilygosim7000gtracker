#include "config.h"
#include "sdLogger.h"
#include <SD.h>

File logFile;

void initSD() {
  if (!SD.begin(SD_CS)) {
    SerialMon.println("Error al inicializar SD");
    return;
  }
  SerialMon.println("SD lista");
}

void logToSD(float lat, float lon) {
  logFile = SD.open("/gpslog.txt", FILE_APPEND);
  if (logFile) {
    logFile.printf("Lat: %.6f, Lon: %.6f\n", lat, lon);
    logFile.close();
  }
}
