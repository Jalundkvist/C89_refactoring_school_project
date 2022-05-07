#ifndef WATCHDOG_H_
#define WATCHDOG_H_

// Inkluderingsdirektiv: 
#include "definitions.h"

// Makrodefinitioner:
#define _512MS ((1 << WDP2) | (1 << WDP0))
#define _1S  ((1<< WDP2) | (1 << WDP1))
#define _2S  ((1 << WDP2) | (1 << WDP1) | (1 << WDP0))
#define _4S  (1 << WDP3)

#define WD_RESET asm("WDR")
#define WD_ENABLE_SYSTEM_RESET WDTCSR |= (1 << WDE)
#define WD_DISABLE_SYSTEM_RESET WDTCSR &= ~(1 << WDE)


// externa funktioner
void WDT_set_timeout(const uint8_t timeout);
void WDT_enable_system_reset(void);
void WDT_disable_system_reset(void);
void WDT_reset(void);

#endif /* WATCHDOG_H_ */