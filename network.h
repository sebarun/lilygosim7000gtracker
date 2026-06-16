#pragma once
#include <Arduino.h>

bool networkInit();
bool networkConnect();
void networkDisconnect();
void sendTeltonika(float la, float lo, float sp);
