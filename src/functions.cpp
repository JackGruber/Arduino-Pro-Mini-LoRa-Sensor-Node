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

// https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long ReadVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bitRead(ADCSRA, ADSC));

  uint8_t adc_low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t adc_high = ADCH; // unlocks both
  long adc_result = (adc_high<<8) | adc_low;
  adc_result = 1125300L / adc_result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return adc_result; // Vcc in millivolts
}