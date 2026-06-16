#include "config.h"

// APN fi.omv.es
const char apn[]      = "fi.omv.es";
const char gprsUser[] = "";
const char gprsPass[] = "";

// MQTT
const char* mqtt_server = "test.mosquitto.org";
const int   mqtt_port   = 1883;
const char* mqtt_topic  = "sim7000/tracker";

// Variables globales de GPS
float lati = 0.0;
float longi = 0.0;

// Estado
bool relayState = false;
bool trackerMode = false;
