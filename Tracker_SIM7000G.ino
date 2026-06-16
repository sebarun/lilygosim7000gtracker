
#include "config.h"
#include "debug.h"
#include "smsControl.h"
#include "gpsManager.h"
#include "powerMonitor.h"
#include "sdLogger.h"
#include "iotClient.h"
#include "power.h"
#include "network.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// --- PINES DEL LIS3DH Y RELÉ ---
#define RELAY_PIN_SAFE      13
#define PIN_MOTION          32 // Conectado a INT1 del LIS3DH (Pin RTC)

TinyGsm modem(SerialAT);

// Variable para el temporizador del tracker
unsigned long lastTracker = 0;

void setup(){
  
  // 1. Inicializar placa y periféricos
  initMQTT();
  initSD();
  powerInit();
  
  logMsg("=== BOOT TRACKER ===");
  
  // 2. Despertar el módem
  powerOnModem();
    // Inicializar módem
  DBG("Iniciando comunicacion AT con SIM7000G...");
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(3000); // Dar tiempo al puerto serie
  if (!modem.restart()){
    DBG("ERROR: El modem no responde.");
    // Aquí podríamos poner un reinicio del ESP32 si falla críticamente
  }

  // PIN de la SIM
  if (strlen(GSM_PIN) > 0) {
    modem.simUnlock(GSM_PIN);
  }
  // 6. Conexión a la red Celular
  DBG("Esperando red...");
  if (!modem.waitForNetwork()) {
    DBG("Error: no hay red");
    delay(5000);
    ESP.restart(); // Si no hay red, reiniciamos el sistema completo
  }
  DBG("Red celular conectada.");
  
  // 7. Conexión a Internet (GPRS)
  DBG("Conectando GPRS...");
  modem.gprsConnect(apn, gprsUser, gprsPass);
  if (modem.isGprsConnected()) {
    DBG("GPRS conectado exitosamente.");
  }
  // Encender GPS interno
  modem.enableGPS();
}
  


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
    // Si estamos en modo SMS, procesamos la bandeja de entrada
    procesarSMS();
    delay(2000); // Pequeña pausa para no saturar el puerto serie
    
  } else {
    // MODO TRACKER: Enviar posición cada intervalo de tiempo
    unsigned long now = millis();
    
    if (now - lastTracker >= TRACKER_INTERVAL) {
      lastTracker = now;
      DBG("Modo Tracker: Intentando obtener GPS...");
      
      if (getGPSFix()) {
        logToSD(lati, longi);
        enviarMQTT(lati, longi);
      } 
        else {
        DBG("Modo Tracker: Sin fix GPS.");
      }
      // Deep sleep hasta siguiente intervalo
      //esp_sleep_enable_timer_wakeup((uint64_t)TRACKER_INTERVAL * 1000);
      //esp_deep_sleep_start();
    }
  }
}
