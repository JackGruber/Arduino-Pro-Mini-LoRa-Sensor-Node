#include "functions.h"
#include "io_pins.h"

void Setup_Pins()
{
    pinMode(PIN_INFO_LED, OUTPUT);
}

void Blink_Info_LED()
{
    digitalWrite(PIN_INFO_LED, HIGH);
    delay(10);
    digitalWrite(PIN_INFO_LED, LOW);
    delay(10);
}