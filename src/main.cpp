#include <Arduino.h>
#include "io_pins.h"
#include "functions.h"
#include "lorawan.h"
#include "DHT.h"
#include "global.h"

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting DHT ..."));
  Setup_Pins();
  LoRaWANSetup();
  dht.begin();
}

void loop() {
  LoraWANDo();
}