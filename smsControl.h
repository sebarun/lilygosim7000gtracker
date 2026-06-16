#pragma once
#include <Arduino.h>
#include "config.h"

String leerSMS();
void procesarSMS();

extern bool relayState;
extern bool trackerMode;
extern float lati, longi;
