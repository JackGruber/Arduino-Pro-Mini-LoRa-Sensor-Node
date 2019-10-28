#pragma once
#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <Arduino.h>
#include <DHT.h>

extern DHT DHTSENSOR;
extern volatile int POWER_DOWN_SLEEP_COUNTER;

#endif