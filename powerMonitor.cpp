#include "config.h"
#include "powerMonitor.h"

float leerVoltaje(int pin) {
  int raw = analogRead(pin);
  float volt = (raw / 4095.0) * 2.0 * 3.3; // Ajusta según divisor resistivo real
  return volt;
}
