/* Inkluderingsdirektiv: */
#include "header.h"

/******************************************************************************
* Funktionen main utgör programmets start- och slutpunkt. Programmets globala
* variabler initieras via anrop av funktionen setup. En goto-sats används för
* att hålla igång programmet så länge matningsspänning tillförs. Resterande
* del av programmet är avbrottsgenererat, så hopp sker kontinuerligt till
* adressen döpt loop.
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

