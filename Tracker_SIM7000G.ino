
#include "config.h"
#include "debug.h"
#include "power.h"
#include "network.h"
#include "smsControl.h"
#include "gpsManager.h"
#include "powerMonitor.h"
#include "sdLogger.h"
#include "iotClient.h"
#include "motion.h"
#include "display.h"

// --- PINES DEL LIS3DH Y RELÉ ---
#define RELAY_PIN_SAFE      13
#define PIN_MOTION          32 // Conectado a INT1 del LIS3DH (Pin RTC)

extern TinyGsm modem;

// Variable para el temporizador del tracker
unsigned long lastTracker = 0;

void setup(){
  // 1. Configurar MQTT, Relé y SD
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  // 1. Inicializar placa y periféricos
  
  initSD();
  // 2. Inicializar Acelerómetro y configurarlo para despertar al ESP32
  motionInit();
  configureMotionWakeup();
  //  . Encender Módem Físicamente (Mejora del Pin 33)
  powerInit();
  SerialMon.begin(UART_BAUD);
  delay(2000);
  initDisplay();
  DBG("=== BOOT TRACKER PROFESIONAL ===");
  powerOnModem();
  initMQTT();
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
  displayMessage("Buscando red celular...");
  DBG("Esperando red...");
  if (!modem.waitForNetwork()) {
    DBG("Error: no hay red");
    delay(5000);
    ESP.restart(); // Si no hay red, reiniciamos el sistema completo
  }
  DBG("Red celular conectada.");
  displayMessage("Red conectada", "Conectando GPRS...");
  
  // 7. Conexión a Internet (GPRS)
  DBG("Conectando GPRS...");
  modem.gprsConnect(apn, gprsUser, gprsPass);
  if (modem.isGprsConnected()) {
    DBG("GPRS conectado exitosamente.");
    displayStatus("GPRS OK", "Esperando GPS");
  }
  // 3. Inicializar comunicación y red
  if (networkInit()) {
      if (networkConnect()) {
          initMQTT(); // Preparamos MQTT ya que hay internet
          // 5. Obtener GPS y enviar datos
          DBG("Intentando obtener fix GPS...");
          displayStatus("GPRS OK", "Buscando GPS...");
          if (getGPSFix()) {
              displayStatus("GPRS OK", "GPS OK");
              logToSD(lati, longi);
              enviarMQTT(lati, longi);
              DBG("Datos enviados correctamente.");
          } else {
              DBG("No se pudo obtener fix GPS en este ciclo.");
          }
          // 6. Revisamos si hay comandos SMS pendientes antes de dormir
          procesarSMS();
        
          // Aquí enviaremos los datos vía GPRS más adelante
          networkDisconnect(); 
      }
  }
  
  // 4. Apagar el módem para no gastar batería durante el sueño
  powerOffModem();

  // 5. Dormir el ESP32
  DBG("Entrando en Deep Sleep esperando movimiento..."); 
  displayClear();
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
