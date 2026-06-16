
#pragma once

// Selección del módem
#define TINY_GSM_MODEM_SIM7000

#include <Arduino.h>
#include <TinyGsmClient.h>

// Serial
#define SerialMon Serial
#define SerialAT  Serial1

// Configuración UART
#define UART_BAUD 115200
#define PIN_TX    27
#define PIN_RX    26

// Pines
#define PWR_PIN    4
#define LED_PIN   12
#define RELAY_PIN 23
#define SD_CS      5

// ADC
#define BAT_ADC   35
#define SOLAR_ADC 34

// SIM PIN
#define GSM_PIN "1336"
#define ADMIN "+34600000000"
#define APN_FI "fi.omv.es"
#define APN_PERSONAL "datos.personal.com"
#define BAT_LOW 20
// Tracker
#define TRACKER_INTERVAL  300000   // 5 minutos

// APN (solo declaraciones) 
extern const char apn[];
extern const char gprsUser[];
extern const char gprsPass[];

// MQTT (solo declaraciones)
extern const char* mqtt_server;
extern const int   mqtt_port;
extern const char* mqtt_topic;

// Variables globales de GPS
extern float lati;
extern float longi;

// Estado global
extern bool relayState;
extern bool trackerMode;
