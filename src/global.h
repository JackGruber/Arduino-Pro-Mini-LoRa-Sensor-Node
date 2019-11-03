#pragma once
#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <Arduino.h>
#include <dht.h>

extern volatile int POWER_DOWN_SLEEP_COUNTER;
extern float HUMIDITY;
extern float TEMPERATURE;
extern DHT DHTSENSOR;
#endif