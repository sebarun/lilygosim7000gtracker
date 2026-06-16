#include "power.h"
#include "logger.h"
#include <Arduino.h>
#include <esp_sleep.h>

#define SIM7000_PWRKEY_PIN 33

void powerInit() {
    // Configuramos el pin del módem como salida
    pinMode(SIM7000_PWRKEY_PIN, OUTPUT);
    // Aseguramos el estado HIGH inicial (apoyando a tu resistencia de 10k)
    digitalWrite(SIM7000_PWRKEY_PIN, HIGH);
    logMsg("Power init OK");
}

void powerOnModem() {
    logMsg("Encendiendo SIM7000G (Pin 33 LOW)...");
    
    // Pulso bajo para encender
    digitalWrite(SIM7000_PWRKEY_PIN, LOW);
    delay(1200); // El manual del SIM7000 pide al menos 1 segundo
    
    // Devolvemos a estado alto
    digitalWrite(SIM7000_PWRKEY_PIN, HIGH);
    
    // Le damos tiempo al firmware del módem para arrancar y estar listo
    delay(3000); 
    logMsg("SIM7000G Encendido.");
}

void powerOffModem() {
    logMsg("Apagando SIM7000G...");
    
    // El apagado (Power Down) normal también requiere un pulso de >1.2s
    digitalWrite(SIM7000_PWRKEY_PIN, LOW);
    delay(1200);
    digitalWrite(SIM7000_PWRKEY_PIN, HIGH);
    
    // Esperamos a que se apague de forma segura
    delay(2000);
    logMsg("SIM7000G Apagado de forma segura.");
}

void enterDeepSleep() {
    logMsg("Entrando en Deep Sleep...");
    // NOTA: Actualmente lo tienes en 5 segundos (5000000 microsegundos).
    // Para un tracker real, esto suele ser mucho mayor (ej. 3 a 5 minutos).
    esp_sleep_enable_timer_wakeup(5000000); 
    esp_deep_sleep_start();
}
