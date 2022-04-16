// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Funktionen setup används för att initiera programmets globala variabler.
* Först implementeras en lysdiod på PIN 6 via ett objekt av strukten Led,
* som döps till led1. Sedan implementeras en tryckknapp på PIN 13 via ett
* objekt av struken Button, som döps till button. PCI-avbrott aktiveras på
* tryckknappens PIN för avläsning av aktuell rumstemperatur, där lysdioden
* togglas vid varje avläsning.
*
* Därefter implementeras timerkretsen Timer 0, som används för att generera
* en bouncetid på 300 ms efter nedtryckning av tryckknappar för att förhindra
* att kontaktstudsar orsakar multipla avbrott.
*
* Därefter implementeras en dynamisk timerkrets, Timer 1. Denna dynamiska timer
* initieras när avbrott genereras via tryckknapp på PIN 6.
* Den lagrar intervallen mellan avbrott på PIN 6 och räknar fram medelvärdet
* mellan totala summan i interrupt_vector dividerat med antal knapptryck
* (antalet knapptryck/element i vektorn begränsas av den andra ingående
* variabeln för new_DynamicTimer).
*
* Sedan deklareras en temperatursensor ansluten till analog PIN A1 via ett
* objekt av strukten tempSensor. Slutligen initeras seriell överföring via
* anrop av funktionen serial, vilket möjliggör transmission till PC följt
* av välkomst meddelande och instruktion för initiering av programmet.
******************************************************************************/
void setup(void)
{
	
	display1 = new_Display(8);
	display2 = new_Display(9);
	button = new_Button(13);
	button.enable_interrupt(&button);
	timer0 = new_Timer(TIMER0, 300);
	timer1 = new_DynamicTimer(TIMER1, 10);
	timer1.on(&timer1);
	timer2 = new_Timer(TIMER2, 1);
	tempSensor = new_TempSensor(1);
	
	init_serial();
	
	serial_print(" Welcome to this temperature measurment program!\n");
	serial_print("To initiate the dynamic measurment, please press the button on PIN 13!\n");
	
	return; // End of function setup.
}