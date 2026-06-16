
#include "config.h"
#include "debug.h"
#include "smsControl.h"
#include "gpsManager.h"
#include "powerMonitor.h"
#include "sdLogger.h"
#include "iotClient.h"
#include "logger.h"
#include "power.h"
#include "gps.h"
#include "sms.h"
#include "network.h"
#include "storage.h"
#include "battery.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// --- PINES DEL LIS3DH Y RELÉ ---
#define RELAY_PIN_SAFE      13
#define PIN_MOTION          32 // Conectado a INT1 del LIS3DH (Pin RTC)

TinyGsm modem(SerialAT);

void setup(){
  SerialMon.begin(UART_BAUD);
  delay(3000);
  SerialMon.println("Iniciando SIM7000G...");
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
    // Inicializar módem
  SerialAT.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(600);
  modem.restart();

  // PIN de la SIM
  if (strlen(GSM_PIN) > 0) {
    modem.simUnlock(GSM_PIN);
  }

  if (!modem.waitForNetwork()) {
    SerialMon.println("Error: no hay red");
    while (true) delay(1000);
  }
  SerialMon.println("Red conectada");

  modem.gprsC(apn, gprsUser, gprsPass);
  if (modem.isGprsConnected()) {
    SerialMon.println("GPRS conectado");
  }

  initSD();
  initMQTT();
  // 1. Inicializar placa y periféricos
  loggerInit();
  storageInit();
  batteryInit();
  powerInit();
  
  logMsg("=== BOOT TRACKER ===");
  
  // 2. Despertar el módem
  powerOnModem();

  // 3. Inicializar comunicación y red
  if (networkInit()) {
      
      // Mantenemos tu lógica original de SMS que incluye pedir el GPS
      handleSMS();
      
      if (networkConnect()) {
          // Aquí enviaremos los datos vía GPRS más adelante
          networkDisconnect(); 
      }
  }
  
  // 4. Apagar el módem para no gastar batería durante el sueño
  powerOffModem();

  // 5. Dormir el ESP32
  enterDeepSleep();
}
void loop(){
  if (!trackerMode) {
    procesarSMS();
  } else {
    unsigned long now = millis();
    if (now - lastTracker >= TRACKER_INTERVAL) {
      lastTracker = now;
      if (getGPSFix()) {
        logToSD(lati, longi);
        enviarMQTT(lati, longi);
      }
      // Deep sleep hasta siguiente intervalo
      esp_sleep_enable_timer_wakeup((uint64_t)TRACKER_INTERVAL * 1000);
      esp_deep_sleep_start();
    }
  }
}
