#include "logger.h"
void loggerInit(){Serial.println("Logger ready");}
void logMsg(const String&m){Serial.println(m);}