#include "config.h"
#include "motion.h"
#include "debug.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Instancia del acelerómetro I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

// Pines I2C de la placa LilyGO
#define I2C_SDA 21
#define I2C_SCL 22

bool motionInit() {
    Wire.begin(I2C_SDA, I2C_SCL);
    
    // El LIS3DH en la LilyGO suele tener la dirección 0x19 (o 0x18)
    if (!lis.begin(0x19)) { 
        DBG("ERROR: No se encontro el LIS3DH");
        return false;
    }
    
    // Rango de 2G es suficiente para detectar movimiento de un vehículo
    lis.setRange(LIS3DH_RANGE_2_G);
    
    // Configuramos una interrupción básica para movimiento.
    // Parámetros: click (1 o 2), umbral de fuerza. Ajusta el 80 según tus pruebas.
    lis.setClick(1, 80); 
    
    DBG("Acelerometro LIS3DH inicializado OK.");
    return true;
}

void configureMotionWakeup() {
    // Configura el ESP32 para despertar del Deep Sleep 
    // cuando el LIS3DH mande un pulso HIGH (1) al pin PIN_MOTION (32)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_MOTION, 1);
    DBG("WakeUp por acelerometro (Pin 32) configurado.");
}
