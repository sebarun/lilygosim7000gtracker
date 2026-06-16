
#include "config.h"
#include "logger.h"
#include "power.h"
#include "gps.h"
#include "sms.h"
#include "network.h"
#include "storage.h"
#include "battery.h"
void setup(){
  Serial.begin(115200);
  loggerInit();
  storageInit();
  batteryInit();
  logMsg("BOOT");
  powerInit();
  handleSMS();
  enterDeepSleep();
}
void loop(){}
