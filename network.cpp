#include "network.h"
#include "config.h"
#include "debug.h"


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
    DBG("Inicializando comunicacion con el modem...");
    
    // Abrimos el puerto serie a 115200 baudios, típico del SIM7000
    SerialAT.begin(UART_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(3000); // Tiempo prudencial para que el puerto estabilice
    
    DBG("Iniciando TinyGSM...");
    // El comando restart comprueba si hay respuesta y reinicia la lógica del módem
    if (!modem.restart()) {
        DBG("ERROR: El modem no responde a comandos AT.");
        return false;
    }
    // Desbloquear SIM si hay PIN
    if (strlen(GSM_PIN) > 0) {
        modem.simUnlock(GSM_PIN);
    }
    String modemInfo = modem.getModemInfo();
    DBG("Modem Info: " + modemInfo);
    return true;
}

bool networkConnect() {
    DBG("Esperando registro en la red celular...");
    // waitForNetwork espera hasta que la tarjeta SIM pille cobertura (GSM/LTE/NB-IoT)
    if (!modem.waitForNetwork()) {
        DBG("ERROR: No se pudo registrar en la red.");
        return false;
    }
    DBG("Registrado en la red celular con exito.");

    // Usamos el APN_FI que tienes en tu config.h (sin usuario ni contraseña)
    DBG(String("Conectando a GPRS con APN: ") + apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        DBG("ERROR: Fallo al levantar sesion GPRS.");
        return false;
    }
    
    DBG("GPRS Conectado. ¡Tenemos Internet!");
    return true;
}

void networkDisconnect() {
    DBG("Cerrando sesion GPRS...");
    modem.gprsDisconnect();
    DBG("GPRS Desconectado.");
}

void sendTeltonika(float la, float lo, float sp) {
    // Por ahora mantenemos tu lógica original, luego implementaremos el envío TCP
    DBG("AVL (Teltonika) " + String(la));
}
