/* Inkluderingsdirektiv: */
#include "header.h"

/******************************************************************************
* Funktionen setup anv�nds f�r att initiera programmets globala variabler.
* F�rst implementeras en lysdiod p� PIN 9 via ett objekt av strukten Led, 
* som d�ps till led1. Sedan implementeras en tryckknapp p� PIN 13 via ett 
* objekt av struken Button, som d�ps till button. PCI-avbrott aktiveras p� 
* tryckknappens PIN f�r avl�sning av aktuell rumstemperatur, d�r lysdioden
* togglas vid varje avl�sning.
* 
* D�refter implementeras timerkretsen Timer 0, som anv�nds f�r att generera
* en bouncetid p� 300 ms efter nedtryckning av tryckknappar f�r att f�rhindra
* att kontaktstudsar orsakar multipla avbrott. Ytterligare en timerkrets, 
* Timer 1, anv�nds f�r att m�ta temperaturen med ett visst intervall, vilket
* vid start �r 60 sekunder. D�rmed aktiveras denna timer direkt. Sedan 
* deklareras en temperatursensor ansluten till analog PIN A1 via ett objekt
* av strukten tempSensor. Slutligen initeras seriell �verf�ring via anrop av 
* funktionen serial, vilket m�jligg�r transmission till PC.
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