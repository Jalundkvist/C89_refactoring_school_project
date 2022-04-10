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
	
    while (true);
	{
		
	}
	return 0;
}

