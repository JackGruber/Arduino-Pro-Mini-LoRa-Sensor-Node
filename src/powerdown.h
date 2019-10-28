#pragma once
#ifndef _POWERDOWN_H
#define _POWERDOWN_H

#include <Arduino.h>

void PowerDownUpdateMicros(void);
void PowerDown(void);
void PowerDownSetupWatchdog(void);
void PowerDownTXIntervall(void);

#endif