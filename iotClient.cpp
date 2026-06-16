#include "config.h"
#include "iotClient.h"
#include "debug.h"
#include <PubSubClient.h>
#include "config.h"

extern TinyGsm modem;

TinyGsmClient gsmClient(modem);
PubSubClient mqtt(gsmClient);

void initMQTT() {
  mqtt.setServer(mqtt_server, mqtt_port);
}

void enviarMQTT(float lat, float lon) {
  if (!mqtt.connected()) {
    DBG("Conectando a MQTT...");
    if (!mqtt.connect("SIM7000Client")) {
      DBG("Error MQTT");
      return;
    }
  }
  String payload = String("{\"lat\":") + lat + ",\"lon\":" + lon + "}";
  mqtt.publish(mqtt_topic, payload.c_str());
  DBG("MQTT enviado: %s", payload.c_str());
}
