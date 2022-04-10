/* Inkluderingsdirektiv: */
#include "header.h"

/******************************************************************************
* Funktionen setup används för att initiera programmets globala variabler.
* Först implementeras en lysdiod på PIN 9 via ett objekt av strukten Led, 
* som döps till led1. Sedan implementeras en tryckknapp på PIN 13 via ett 
* objekt av struken Button, som döps till button. PCI-avbrott aktiveras på 
* tryckknappens PIN för avläsning av aktuell rumstemperatur, där lysdioden
* togglas vid varje avläsning.
* 
* Därefter implementeras timerkretsen Timer 0, som används för att generera
* en bouncetid på 300 ms efter nedtryckning av tryckknappar för att förhindra
* att kontaktstudsar orsakar multipla avbrott. Ytterligare en timerkrets, 
* Timer 1, används för att mäta temperaturen med ett visst intervall, vilket
* vid start är 60 sekunder. Därmed aktiveras denna timer direkt. Sedan 
* deklareras en temperatursensor ansluten till analog PIN A1 via ett objekt
* av strukten tempSensor. Slutligen initeras seriell överföring via anrop av 
* funktionen serial, vilket möjliggör transmission till PC.
******************************************************************************/
void setup(void)
{
	unsigned char led1_PIN = 9;
	unsigned char button_PIN = 13;
	unsigned char temp_PIN = 1;
	
	enum TimerSelection timer0_selection = TIMER0;
	enum TimerSelection timer1_selection = TIMER1;
	
	float timer0_delay = 300;
	float timer1_delay = 500;
	
	led1 = new_Led(&led1_PIN);
	button = new_Button(&button_PIN);
	Button_enable_interrupt(button);
	
	timer0 = new_Timer(&timer0_selection, &timer0_delay);
	timer1 = new_Timer(&timer1_selection, &timer1_delay);
	Timer_on(timer1);
	
	tempSensor = new_TempSensor(&temp_PIN);
	
	init_serial();
	return;
}