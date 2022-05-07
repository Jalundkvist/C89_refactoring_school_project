// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Avbrottsrutin f�r PCI-avbrott f�r I/O-port B. Vid aktivering av denna
* avbrottsrutin inaktiveras PCI-avbrott p� tryckknappens PIN 13, vilket i
* detta fall �r enda k�llan till PCI-avbrott p� I/O-porten i fr�ga. Detta g�rs
* f�r att f�rhindra p�verkan av kontaktstudsar, som annars kan medf�r att 
* multipla avbrott �ger rum kort efter varandra n�r knappen studsar. Timer 0
* aktiveras f�r att efter 300 ms �teraktivera PCI-avbrott p� PIN 13. Ifall
* nedtryckning av tryckknappen orsakade aktuellt avbrott, s� m�ts aktuell
* rumstemperatur och skrivs ut i en seriell terminal. . Samtidigt nollst�lls Timer 1. ----------
******************************************************************************/
ISR (PCINT0_vect)
{
	button.disable_interrupt(&button);
	timer0.on(&timer0);
	
	if (button.is_pressed(&button))
	{
		timer1.update(&timer1);
		tempSensor.print_temperature(&tempSensor);
		if (!timer2.enabled)
			timer2.on(&timer2);
	}
	
	return;
}

/******************************************************************************
* Avbrottsrutin f�r Timer 0 i Normal Mode, vilket sker var 1.024:e millisekund
* d� timern i fr�ga �r aktiverad. Denna avbrottsrutin anv�nds f�r att generera 
* en de-bouncetid p� 300 ms, d�r PCI-avbrott p� PIN 13 h�lls inaktiverat efter 
* ett givet avbrott f�r att f�rhindra att multipla �ger rum p� grund av 
* kontakstudsar. N�r tillr�ckligt m�nga avbrott har �gt rum s� att timern har 
* l�pt ut, s� inaktiveras Timer 0 och PCI-avbrott p� PIN 13 �teraktiveras.
******************************************************************************/
ISR (TIMER0_OVF_vect)
{
	timer0.count(&timer0);
	
	if (timer0.elapsed(&timer0))
	{
		timer0.off(&timer0);
		button.enable_interrupt(&button);	
	}
	
	return;
}

/******************************************************************************
* Avbrottsrutin f�r Timer 1 i CTC Mode, vilket sker var 1.024:e millisekund d� 
* timern i fr�ga �r aktiverad. Denna avbrottsrutin anv�nds f�r att m�ta <-----------------
* rumstemperaturen var 60:e sekund, alternativt 60 sekunder efter senaste
* knapptryckning. Varje g�ng denna rutin aktiveras s� r�knas antalet exekverade 
* avbrott upp. N�r tillr�ckligt m�nga avbrott har �gt rum s� att timern har l�pt 
* ut, s� m�ts rumstemperaturen och skrivs ut i terminalen.
******************************************************************************/
ISR (TIMER1_COMPA_vect)
{
	timer1.count(&timer1);	
	
	if (timer1.elapsed(&timer1))
	{
		tempSensor.print_temperature(&tempSensor);
	}
	return;
}
/******************************************************************************
* 
******************************************************************************/
ISR (TIMER2_OVF_vect) // <-----------
{
	display.update_digit(&display, tempSensor.rounded_temp);
	return;
}