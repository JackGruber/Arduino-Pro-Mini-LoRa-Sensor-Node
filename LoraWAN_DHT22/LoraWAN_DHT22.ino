/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the (early prototype version of) The Things Network.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in g1,
 *  0.1% in g2).
 *
 * Change DEVADDR to a unique address!
 * See http://thethingsnetwork.org/wiki/AddressSpace
 *
 * Do not forget to define the radio type correctly in config.h.
 *
 *******************************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <avr/sleep.h> 
#include <DHT.h>

// Local include files
#include "lora.h"



volatile int sleep_counter = 0; // Schlafzyklen mitzÃ¤hlen

// Pinmapping
const byte PIN_INFO_LED       = 9; 


#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

uint8_t mydata[5];

void setup() {
    Serial.begin(115200);
    Serial.println(F("Starting ..."));

    dht.begin();
       
    Setup_LMIC();

    Setup_Watchdog();
    Setup_Pins();

    // Start job
    do_send(&sendjob);
}

void loop() {
  
    Do_LoraWAN(); 
    Blink_Info_LED();
}

/* TTN Decoder

function Decoder(bytes, port)
{
  var vcc = (bytes[0] + 200)/100;
  var humidity = bytes[1];
  var temperature = ((bytes[2] << 8) | bytes[3]) / 10;

  return {
    measurement: {
      vcc: vcc,
      humidity: humidity,
      temperature : temperature
    },
  };
}

*/
void Read_Data()
{
    int32_t vcc = ( Read_Vcc() / 10) - 200;
    
    float humidity_org = dht.readHumidity();
    int32_t humidity = humidity_org;
    
    float temperature_org = dht.readTemperature();
    int32_t temp = (temperature_org * 10);
     
    if ( isnan(humidity_org) || isnan(temperature_org) )
    {
      Serial.println(F("Failed to read from DHT sensor!"));
    }

    if ( isnan(humidity_org) )
    { 
      mydata[2] = 255;    
      mydata[2] = 255;
    }
    else 
    { 
      mydata[2] = temp >> 8;
      mydata[3] = temp & 0xFF;
    }

    if ( isnan(humidity_org)) { mydata[1] = 255; }
    else { mydata[1] = humidity; }
    
    mydata[0] = vcc;
    
    Serial.print(F("VCC: "));
    Serial.println(mydata[0]);
    
    Serial.print(F("Humidity: "));
    Serial.println(mydata[1]);
    
    Serial.print(F("Temperature: "));
    Serial.print(mydata[2]);
    Serial.print(F(" "));
    Serial.println(mydata[3]);
}

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


ISR(WDT_vect)
{
    sleep_counter ++;
}

// https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long Read_Vcc() {
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


void Power_Down_Update_Micros()
{
    extern volatile unsigned long timer0_overflow_count;
    Power_Down();
    cli();
    // LMIC uses micros() to keep track of the duty cycle, so
    // hack timer0_overflow for a rude adjustment:  
    timer0_overflow_count+= 8 * 64 * clockCyclesPerMicrosecond();
    sei();
}

void Power_Down()
{
  ADCSRA &= ~(1<<ADEN);                 // ADC Disable for power saving
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Use the Power Down sleep mode
  cli();                                // Disable interrupts
  WDTCSR |= (1<<WDIE);                  // WDTIE: Watchdog Timer Interrupt Enable
  sleep_enable();                       // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sleep_bod_disable();                  // Disable Brown-Out Detection
  sei();                                // Enable interrupts
  sleep_mode();                         // go to sleep and wait for interrupt...
  sleep_disable();                      // Clear SE bit
  WDTCSR &= ~(1<<WDIE);                 // WDTIE: Watchdog Timer Interrupt Disable
  sei();                                // Enable interrupts

  ADCSRA |= (1<<ADEN);                  // ADC Enable
}

void Setup_Watchdog()
{
  cli(); // Disable global interrupts
  MCUSR &= ~(1<<WDRF); // Clear WDRF: Watchdog Reset Flag
  WDTCSR |= (1<<WDCE) | (1<<WDE); // WDCE: Watchdog Change Enable, WDE: Watchdog System Reset Enable

  // Watchdog Timer Prescaler
  //WDTCSR = (1<<WDP2) | (1<<WDP0);             // 0.5 seconds
  //WDTCSR = (1<<WDP2) | (1<<WDP1);             // 1.0 seconds
  //WDTCSR = (1<<WDP2) | (1<<WDP1) | (1<<WDP0); // 2.0 seconds
  //WDTCSR = (1<<WDP3);                         // 4.0 seconds
  WDTCSR = (1<<WDP3) | (1<<WDP0);             // 8.0 seconds

  sei(); // Enable global interrupts
}

