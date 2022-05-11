// Inkluderar egen headerfil.
#include "watchdog.h"

/*********************************************************************************************
* Funktionen WD_initialize initierar Watchdog-timern och ställer in önskad timeout.
* globalt avbrott inaktiveras inför initiering och aktiveras efter
*
* Inparameter	:timeout (Makron som _512MS, _1S, _2S, _4S)
* Returnerar	: -- 
*********************************************************************************************/
void WD_initialize(const uint8_t timeout)
{
	asm("CLI");
	WDTCSR |= (1 << WDE) | (1 << WDCE);
	WDTCSR = (1 << WDE) | (uint8_t)(timeout);
	asm("SEI");
	return;
}

/*********************************************************************************************
* Funktionen WD_enable_system_reset aktiverar Watchdog-timern i System Reset Mode.
* Detta medför en återställning av mikrodatorn om funktionen WD_reset kontinuerligt inom 
* given tid som ställs in genom WD_initialize.
*
* Detta sker genom att anropa funktionen WD_reset följt av biten WDE 
* (Watchdog System Reset Enable) ettställs i statusregistret WDTCSR
* (Watchdog Timer Control and Status Register).
*
* Inparameter	: --
* Returnerar	: --
*********************************************************************************************/
void WD_enable_system_reset(void)
{
	WD_reset();
	WDTCSR |= (1 << WDE);
	return;
}

/*********************************************************************************************
* Funktionen WD_disable_system_reset inaktiverar Watchdog-timern från System Reset Mode.
*
* Detta sker genom att anropa funktionen WD_reset följt av biten WDE
* (Watchdog System Reset Enable) nollställs i statusregistret WDTCSR
* (Watchdog Timer Control and Status Register).
*
* Inparameter	: --
* Returnerar	: --
*********************************************************************************************/
void WD_disable_system_reset(void)
{
	WD_reset();
	WDTCSR &= ~(1 << WDE);
	return;
}

/*********************************************************************************************
* Funktionen WD_reset nollställer Watchdog-timern.
* Globalt avbrott inaktiveras i början av funktionen och aktiveras igen efter de två 
* instruktionerna har utförts.
*
* För återställning av timern så anropas assemblerinstruktionen WDR (Watchdog Reset).
* Följt av att biten Watchdog reset flag - WDRF - nollställs i MCUSR 
* (Microcontroll Unit Status Register).
*
* Inparameter	: --
* Returnerar	: --
*********************************************************************************************/
void WD_reset(void)
{
	asm("CLI");
	asm("WDR");
	MCUSR &= ~(1 << WDRF);
	asm("SEI");
	return;
}