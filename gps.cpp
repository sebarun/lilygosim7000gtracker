#include "gps.h"
#include "logger.h"
bool getGPS(float&la,float&lo,float&sp){la=40;lo=-3;sp=0;return true;}
void gpsEnable(){logMsg("GNSS ON");}
void gpsDisable(){logMsg("GNSS OFF");}