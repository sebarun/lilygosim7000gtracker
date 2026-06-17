#include "config.h"
#include "smsControl.h"
#include "gpsManager.h"
#include "powerMonitor.h"
#include "debug.h"

#define SMS_TARGET "+34625864654"

extern TinyGsm modem;
extern bool relayState;
extern bool trackerMode;
extern float lati, longi;

// --- Leer SMS ---
String leerSMS() {
  modem.sendAT("+CMGL=\"REC UNREAD\"");
  String res = modem.stream.readString();
  return res;
}

// --- Procesar comandos SMS ---
void procesarSMS() {
  String sms = leerSMS();
  if (sms.length() == 0) return;
  sms.toUpperCase();

  if (sms.indexOf("ON") >= 0) {
    digitalWrite(RELAY_PIN, HIGH);
    relayState = true;
    modem.sendSMS(SMS_TARGET, "Relé ACTIVADO ✅");
  }
  else if (sms.indexOf("OFF") >= 0) {
    digitalWrite(RELAY_PIN, LOW);
    relayState = false;
    modem.sendSMS(SMS_TARGET, "Relé DESACTIVADO ❌");
  }
  else if (sms.indexOf("GPS") >= 0) {
    if (getGPSFix()) {
      String url = "https://maps.google.com/?q=" + String(lati, 6) + "," + String(longi, 6);
      modem.sendSMS(SMS_TARGET, "Ubicación: " + url);
    } else {
      modem.sendSMS(SMS_TARGET, "No fix GPS 🛰️");
    }
  }
  else if (sms.indexOf("MODO SMS") >= 0) {
    trackerMode = false;
    modem.sendSMS(SMS_TARGET, "Modo cambiado a SMS 📩");
  }
  else if (sms.indexOf("MODO TRACKER") >= 0) {
    trackerMode = true;
    modem.sendSMS(SMS_TARGET, "Modo cambiado a TRACKER 🛰️");
  }
  else if (sms.indexOf("STATUS") >= 0) {
    float vbatt = leerVoltaje(BAT_ADC);
    float vsolar = leerVoltaje(SOLAR_ADC);

    String msg = "Estado:\n";
    msg += String("Relé: ") + (relayState ? "ON ✅" : "OFF ❌") + "\n";
    msg += "Bat: " + String(vbatt, 2) + "V\n";
    msg += "Solar: " + String(vsolar, 2) + "V\n";

    if (lati != 0 && longi != 0) {
      msg += "GPS: https://maps.google.com/?q=" + String(lati, 6) + "," + String(longi, 6);
    } else {
      msg += "GPS: No fix 🛰️";
    }

    modem.sendSMS(SMS_TARGET, msg);
  }
}
