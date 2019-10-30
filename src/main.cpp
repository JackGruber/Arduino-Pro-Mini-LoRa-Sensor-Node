#include <Arduino.h>
#include "io_pins.h"
#include "functions.h"
#include "lorawan.h"
#include "global.h"
#include "powerdown.h"

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting DHT ..."));
  delay(3000);
  PowerDownSetupWatchdog();
  Setup_Pins();
  Blink_Info_LED();
  LoRaWANSetup();  
}

void loop() {
  LoraWANDo();
}