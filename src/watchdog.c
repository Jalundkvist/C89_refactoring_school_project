// Inkluderar eget bibliotek
#include "watchdog.h"

/*********************************************************************************************
* Funktionen WDT_
*
* Inparameter	:
* Returnerar	: -- 
*********************************************************************************************/
void WDT_set_timeout(const uint8_t timeout)
{
	DISABLE_INTERRUPTS;
	WDTCSR |= (1 << WDE) | (1 << WDCE);
	WDTCSR = (1 << WDE) | (uint8_t)(timeout);
	ENABLE_INTERRUPTS;
	return;
}

/*********************************************************************************************
* Funktionen WDT_
*
* Inparameter	:
* Returnerar	: --
*********************************************************************************************/
void WDT_enable_system_reset(void)
{
	WDT_reset();
	WD_ENABLE_SYSTEM_RESET;
	return;
}

/*********************************************************************************************
* Funktionen WDT_
*
* Inparameter	:
* Returnerar	: --
*********************************************************************************************/
void WDT_disable_system_reset(void)
{
	WDT_reset();
	WD_DISABLE_SYSTEM_RESET;
	return;
}

/*********************************************************************************************
* Funktionen WDT_
*
* Inparameter	:
* Returnerar	: --
*********************************************************************************************/
void WDT_reset(void)
{
	DISABLE_INTERRUPTS;
	WD_RESET;
	MCUSR &= ~(1 << WDRF);
	ENABLE_INTERRUPTS;
	return;
}