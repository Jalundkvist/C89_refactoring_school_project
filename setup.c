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
	led1 = new_Led(9);
	button = new_Button(13);
	button.enable_interrupt(&button);
	timer0 = new_Timer(TIMER0, 300);
	timer1 = new_Timer(TIMER1, 10000);
	timer1.on(&timer1);
	
	//tempSensor = new_TempSensor(&temp_PIN);
	
	init_serial();
	return;
}