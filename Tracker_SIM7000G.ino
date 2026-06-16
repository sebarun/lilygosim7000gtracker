
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
  
  // 1. Inicializar placa y periféricos
  loggerInit();
  storageInit();
  batteryInit();
  powerInit();
  
  logMsg("=== BOOT TRACKER ===");
  
  // 2. Despertar el módem
  powerOnModem();
  
  // 3. Ejecutar las tareas del rastreador
  handleSMS();
  
  // 4. Apagar el módem para no gastar batería durante el sueño
  powerOffModem();

  // 5. Dormir el ESP32
  enterDeepSleep();
}
void loop(){}
