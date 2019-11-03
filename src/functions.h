#pragma once
#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <Arduino.h>

void Setup_Pins(void);
void Blink_Info_LED(void);
long ReadVcc(void);
void ReadDHTSensor(void);
void PrintResetReason(void);
#endif