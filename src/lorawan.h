#pragma once
#ifndef _LORAWAN_H
#define _LORAWAN_H

#include <Arduino.h>
#include <lmic.h>
#include "lora_credentials.h"

// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the prototype TTN network initially.
static const PROGMEM u1_t NWKSKEY[16] = TTN_NWKSKEY;

// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the prototype TTN network initially.
static const u1_t PROGMEM APPSKEY[16] = TTN_APPSKEY;

// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = TTN_DEVADDR ; 

static osjob_t sendjob;

void os_getArtEui (u1_t* buf);
void os_getDevEui (u1_t* buf);
void os_getDevKey (u1_t* buf);

void LoRaWANSetup(void);
void LoraWANDo_send(osjob_t* j);
void LoraWANDo(void);

#endif