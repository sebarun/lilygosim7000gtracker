#include "sms.h"
#include "gps.h"
#include "logger.h"
void handleSMS(){float la,lo,sp;gpsEnable();getGPS(la,lo,sp);logMsg(String("https://maps.google.com/?q=")+la+","+lo);gpsDisable();}