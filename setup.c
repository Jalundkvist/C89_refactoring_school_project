// Inkluderingsdirektiv:
#include "header.h"

static void init_GPIO(void);
static void init_timer(void);
static void init_analog(void);
/******************************************************************************
* Funktionen setup anv�nds f�r att initiera programmets olika objekt och
* globala variabler.
* f�rst anropas initiering av GPIO enheter, sedan initieras de timerkretsar
* som skall anv�ndas genom init_timer. F�ljt av att temperaturavl�saren
* aktiveras med init_analog. till sist aktiveras seriell �verf�ring via
* funktionen init_serial som avslutar funktionen setup med ett 
* v�lkomstmeddelande.
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
* Funktionen init_GPIO initierar 2 objekt av strukten Display samt ettst�ller
* hela Data Direction register D till utportar som anv�nds f�r att skriva
* siffror till objekten display1 & display2.
* Sedan initieras ett objekt av strukten button som anv�nds f�r uppdatera
* generera avbrott via knapptryck f�r att uppdatera det dynamiska m�tsystemet.
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
* timer0 - anv�nds f�r debounce funktion f�r tryckknapp p� PORTB5.
* timer1 - anv�nds f�r uppr�kning av tid mellan knapptryck som dynamiskt 
* anpassar utskriftsintervallen av temperaturen.
* timer2 - anv�nds f�r skifta mellan att h�lla display1 t�nd och display2
* sl�ckt och vice versa f�r att kunna h�lla tv� olika siffror t�nda "samtidigt".
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
* m�jligg�r avl�sning av temperatur.
*
* Inparameter	: - 
* Returnerar	: -
******************************************************************************/
static void init_analog(void)
{
	tempSensor = new_TempSensor(1);
	
	return; // End of function init_analog
}