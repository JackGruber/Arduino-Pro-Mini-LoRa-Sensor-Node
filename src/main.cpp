#include <Arduino.h>
#include "io_pins.h"
#include "functions.h"
#include "lorawan.h"
#include "DHT.h"
#include "global.h"
#include "powerdown.h"

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting DHT ..."));
  Setup_Pins();
  LoRaWANSetup();
  PowerDownSetupWatchdog();
  DHTSENSOR.begin();
}

void loop() {
  LoraWANDo();
}