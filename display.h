#pragma once
#include <Arduino.h>

void initDisplay();
void displayStatus(const String& networkStatus, const String& gpsStatus);
void displayMessage(const String& line1, const String& line2 = "");
void displayLog(const String& msg);
void displayClear();
