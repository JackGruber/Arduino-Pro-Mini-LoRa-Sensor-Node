#include <Arduino.h>
#include "io_pins.h"
#include "functions.h"
#include "lorawan.h"
#include "global.h"
#include "powerdown.h"

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting DHT ..."));
  PrintResetReason();
  delay(3000);
  DHTSENSOR.setup(PIN_DHT);
  PowerDownSetupWatchdog();
  Setup_Pins();
  Blink_Info_LED();
  LoRaWANSetup();  
}

void loop() {
  LoraWANDo();
}