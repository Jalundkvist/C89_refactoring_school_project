// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Funktionen setup anv�nds f�r att initiera programmets globala variabler.
* F�rst implementeras en lysdiod p� PIN 6 via ett objekt av strukten Led,
* som d�ps till led1. Sedan implementeras en tryckknapp p� PIN 13 via ett
* objekt av struken Button, som d�ps till button. PCI-avbrott aktiveras p�
* tryckknappens PIN f�r avl�sning av aktuell rumstemperatur, d�r lysdioden
* togglas vid varje avl�sning.
*
* D�refter implementeras timerkretsen Timer 0, som anv�nds f�r att generera
* en bouncetid p� 300 ms efter nedtryckning av tryckknappar f�r att f�rhindra
* att kontaktstudsar orsakar multipla avbrott.
*
* D�refter implementeras en dynamisk timerkrets, Timer 1. Denna dynamiska timer
* initieras n�r avbrott genereras via tryckknapp p� PIN 6.
* Den lagrar intervallen mellan avbrott p� PIN 6 och r�knar fram medelv�rdet
* mellan totala summan i interrupt_vector dividerat med antal knapptryck
* (antalet knapptryck/element i vektorn begr�nsas av den andra ing�ende
* variabeln f�r new_DynamicTimer).
*
* Sedan deklareras en temperatursensor ansluten till analog PIN A1 via ett
* objekt av strukten tempSensor. Slutligen initeras seriell �verf�ring via
* anrop av funktionen serial, vilket m�jligg�r transmission till PC f�ljt
* av v�lkomst meddelande och instruktion f�r initiering av programmet.
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