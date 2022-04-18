// Inkluderingsdirektiv:
#include "header.h"

static void init_GPIO(void);
static void init_timer(void);
static void init_analog(void);
/******************************************************************************
* Funktionen setup används för att initiera programmets olika objekt och
* globala variabler.
* först anropas initiering av GPIO enheter, sedan initieras de timerkretsar
* som skall användas genom init_timer. Följt av att temperaturavläsaren
* aktiveras med init_analog. till sist aktiveras seriell överföring via
* funktionen init_serial som avslutar funktionen setup med ett 
* välkomstmeddelande.
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
	serial_print("To initiate the dynamic measurment, please press the button on PIN 13!\n");
	
	return; // End of function setup.
}

/******************************************************************************
* Funktionen init_GPIO initierar 2 objekt av strukten Display samt ettställer
* hela Data Direction register D till utportar som används för att skriva
* siffror till objekten display1 & display2.
* Sedan initieras ett objekt av strukten button som används för uppdatera
* generera avbrott via knapptryck för att uppdatera det dynamiska mätsystemet.
*
* Inparameter	: -
* Returnerar	: -
******************************************************************************/
static void init_GPIO(void)
{
	display = new_Display(8,9);
	DDRD = 0xFF;
	button = new_Button(13);
	button.enable_interrupt(&button);
	
	return; // End of function init_GPIO
}

/******************************************************************************
* Funktionen init_timer initierar 2 objekt av strukten Timer samt ett objekt
* av strukten DynamicTimer.
* timer0 - används för debounce funktion för tryckknapp på PORTB5.
* timer1 - används för uppräkning av tid mellan knapptryck som dynamiskt 
* anpassar utskriftsintervallen av temperaturen.
* timer2 - används för skifta mellan att hålla display1 tänd och display2
* släckt och vice versa för att kunna hålla två olika siffror tända "samtidigt".
*
* Inparameter	: -
* Returnerar	: -
******************************************************************************/
static void init_timer(void)
{
	timer0 = new_Timer(TIMER0, 300);
	timer1 = new_DynamicTimer(TIMER1, 10);
	timer1.on(&timer1);
	timer2 = new_Timer(TIMER2, 200);
	
	return; // End of function init_timer
}
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
	
	return; // End of function init_analog
}