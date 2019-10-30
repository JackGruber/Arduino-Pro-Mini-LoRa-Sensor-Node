#include <Arduino.h>
#include "io_pins.h"
#include "functions.h"
#include "lorawan.h"
#include "global.h"
#include "powerdown.h"

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting DHT ..."));
  PowerDownSetupWatchdog();
  ReadDHTSensor();
  Setup_Pins();
  LoRaWANSetup();
  
}

void loop() {
  LoraWANDo();
}