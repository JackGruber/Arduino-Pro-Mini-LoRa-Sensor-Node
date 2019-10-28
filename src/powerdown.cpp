#include "powerdown.h"
#include <avr/sleep.h> 
#include "settings.h"

volatile int POWER_DOWN_SLEEP_COUNTER = 0;


void PowerDownUpdateMicros()
{
    extern volatile unsigned long timer0_overflow_count;
    PowerDown();
    cli();
    // LMIC uses micros() to keep track of the duty cycle, so
    // hack timer0_overflow for a rude adjustment:  
    timer0_overflow_count+= 8 * 64 * clockCyclesPerMicrosecond();
    sei();
}

void PowerDown()
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

void PowerDownSetupWatchdog()
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

void PowerDownTXIntervall()
{
    Serial.println(F("PowerDown..."));
    Serial.flush();        
    
    int sleep_cycles = LORA_TX_INTERVAL / 8;

    for (int i=0; i<=sleep_cycles+1; i++) 
    {
      PowerDownUpdateMicros();
    }

    Serial.println(F("PowerUp..."));
}

ISR(WDT_vect)
{
    POWER_DOWN_SLEEP_COUNTER ++;
}