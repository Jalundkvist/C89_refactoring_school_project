/* Inkluderingsdirektiv: */
#include "header.h"

/******************************************************************************
* Funktionen main utg�r programmets start- och slutpunkt. Programmets globala
* variabler initieras via anrop av funktionen setup. En goto-sats anv�nds f�r
* att h�lla ig�ng programmet s� l�nge matningssp�nning tillf�rs. Resterande
* del av programmet �r avbrottsgenererat, s� hopp sker kontinuerligt till
* adressen d�pt loop.
******************************************************************************/
int main(void)
{	
	
	setup();
	
	
	long num1 = -25;
	unsigned long num2 = 25;
	serial_print("Test if working\n");
	serial_print_integer("Test integer: %d \n", &num1);
	serial_print_unsigned("Test unsigned: %u \n", &num2);
    while (true);
	{
		
	}
	
	return 0;
}

