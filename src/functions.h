#pragma once
#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <Arduino.h>
#include <dht.h>

void Setup_Pins(void);
void Blink_Info_LED(void);
long ReadVcc(void);
void ReadDHTSensor(void);

#endif