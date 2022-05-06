// Inkluderingsdirektiv:
#include "header.h"

static void init_GPIO(void);
static void init_timer(void);
static void init_analog(void);

/******************************************************************************
* Funktionen setup används för att initiera programmets olika objekt och
* globala variabler.
* Seriell överföring aktiveras, aktuella timers aktiveras, objekt för
* temperaturavläsning aktiveras, generella I/O enheter aktiveras.
* 
* Välkommstmeddelande skrivs ut följd av att watchdog timer aktiveras med en
* 2 sekunders räknare som återställer hela systemet ifall resetfunktionen i
* main inte anropas inom de två sekunder.
*
* Inparameter	: -
* Returnerar	: -
******************************************************************************/
void setup(void)
{
	init_serial();
	init_timer();	
	init_analog();
	init_GPIO();
	serial_print("Welcome to this temperature measurment program!\n");
	serial_print("To initiate the dynamic measurment, please press the button on PIN 13.\n");
	serial_print("The temperature will appear on the two seven segment displays\n");
	WDT_set_timeout(_2S);
	WDT_enable_system_reset();
	
	return; 
} // End of function setup.

/******************************************************************************
* Funktionen init_GPIO initierar 2 objekt av strukten Display
* Sedan initieras ett objekt av strukten button som används för uppdatera
* generera avbrott via knapptryck för att uppdatera det dynamiska mätsystemet.
*
* Till sist aktiveras 7 pins, PIN 2-7 i DDRD via hexadecimal FC och PIN 8 
* i DDRB (Data direction register).
*
* Inparameter	: -
* Returnerar	: -
******************************************************************************/
static void init_GPIO(void)
{
	display = new_Display(10, 11);
	button = new_Button(13);
	button.enable_interrupt(&button);
	
	DDRD = 0xFC;
	DDRB |= (1<<0);
	
	return;
} // End of function init_GPIO

/******************************************************************************
* Funktionen init_timer initierar 2 objekt av strukten Timer samt ett objekt
* av strukten DynamicTimer.
* timer0 - används för debounce funktion för tryckknapp på PORTB5.
* timer1 - används för uppräkning av tid mellan knapptryck som dynamiskt 
* anpassar utskriftsintervallen av temperaturen.
* timer2 - används för uppdatera tiotal/ental på två 7-segments displayer
* med 1.024 ms mellanrum.
*
* Inparameter	: -
* Returnerar	: -
******************************************************************************/
static void init_timer(void)
{
	timer0 = new_Timer(TIMER0, 300);
	timer1 = new_DynamicTimer(TIMER1, 10);
	timer1.on(&timer1);
	timer2 = new_Timer(TIMER2, 1);
	
	return; 
} // End of function init_timer
/******************************************************************************
* Funktionen init_analog initierar ett objekt av strukten TempSensor som
* möjliggör avläsning av temperatur.
*
* Inparameter	: - 
* Returnerar	: -
******************************************************************************/
static void init_analog(void)
{
	tempSensor = new_TempSensor(1);
	return; 
} // End of function init_analog