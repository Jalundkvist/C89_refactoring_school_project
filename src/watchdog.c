// Inkluderar egen headerfil.
#include "watchdog.h"

/*********************************************************************************************
* Funktionen WD_initialize initierar Watchdog-timern och st�ller in �nskad timeout.
* globalt avbrott inaktiveras inf�r initiering och aktiveras efter
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
* Detta medf�r en �terst�llning av mikrodatorn om funktionen WD_reset kontinuerligt inom 
* given tid som st�lls in genom WD_initialize.
*
* Detta sker genom att anropa funktionen WD_reset f�ljt av biten WDE 
* (Watchdog System Reset Enable) ettst�lls i statusregistret WDTCSR
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
* Funktionen WD_disable_system_reset inaktiverar Watchdog-timern fr�n System Reset Mode.
*
* Detta sker genom att anropa funktionen WD_reset f�ljt av biten WDE
* (Watchdog System Reset Enable) nollst�lls i statusregistret WDTCSR
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
* Funktionen WD_reset nollst�ller Watchdog-timern.
* Globalt avbrott inaktiveras i b�rjan av funktionen och aktiveras igen efter de tv� 
* instruktionerna har utf�rts.
*
* F�r �terst�llning av timern s� anropas assemblerinstruktionen WDR (Watchdog Reset).
* F�ljt av att biten Watchdog reset flag - WDRF - nollst�lls i MCUSR 
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