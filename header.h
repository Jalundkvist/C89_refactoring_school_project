
#ifndef HEADER_H_
#define HEADER_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "GPIO.h" 
#include "Timer.h"
#include "DynamicTimer.h"
#include "Serial.h"
#include "ADC.h"
#include "watchdog.h"

// Globala variabler:
Display display;
Button button; 
Timer timer0, timer2;
DynamicTimer timer1; 
TempSensor tempSensor;


// Funktionsdeklarationer:
void setup(void);

#endif /* HEADER_H_ */