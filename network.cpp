#include "network.h"
#include "logger.h"
#include "config.h"
#include <Arduino.h>

// 1. Definir el modelo de módem ANTES de incluir TinyGSM
#define TINY_GSM_MODEM_SIM7000
#include <TinyGsmClient.h>

// 2. Pines internos de la LilyGO T-SIM7000G para el puerto Serie
#define MODEM_TX 27
#define MODEM_RX 26

// 3. Mapeamos SerialAT al puerto hardware Serial1 del ESP32
#define SerialAT Serial1

// 4. Instanciamos el módem y el cliente TCP (este último lo usaremos para Teltonika luego)
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

bool networkInit() {
    logMsg("Inicializando comunicacion con el modem...");
    
    // Abrimos el puerto serie a 115200 baudios, típico del SIM7000
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(3000); // Tiempo prudencial para que el puerto estabilice
    
    logMsg("Iniciando TinyGSM...");
    // El comando restart comprueba si hay respuesta y reinicia la lógica del módem
    if (!modem.restart()) {
        logMsg("ERROR: El modem no responde a comandos AT.");
        return false;
    }
    
    String modemInfo = modem.getModemInfo();
    logMsg("Modem Info: " + modemInfo);
    return true;
}

bool networkConnect() {
    logMsg("Esperando registro en la red celular...");
    // waitForNetwork espera hasta que la tarjeta SIM pille cobertura (GSM/LTE/NB-IoT)
    if (!modem.waitForNetwork()) {
        logMsg("ERROR: No se pudo registrar en la red.");
        return false;
    }
    logMsg("Registrado en la red celular con exito.");

    // Usamos el APN_FI que tienes en tu config.h (sin usuario ni contraseña)
    logMsg(String("Conectando a GPRS con APN: ") + APN_FI);
    if (!modem.gprsConnect(APN_FI, "", "")) {
        logMsg("ERROR: Fallo al levantar sesion GPRS.");
        return false;
    }
    
    logMsg("GPRS Conectado. ¡Tenemos Internet!");
    return true;
}

void networkDisconnect() {
    logMsg("Cerrando sesion GPRS...");
    modem.gprsDisconnect();
    logMsg("GPRS Desconectado.");
}

void sendTeltonika(float la, float lo, float sp) {
    // Por ahora mantenemos tu lógica original, luego implementaremos el envío TCP
    logMsg("AVL (Teltonika) " + String(la));
}
