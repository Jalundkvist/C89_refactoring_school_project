// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Funktionen main utgör programmets start- och slutpunkt. Programmets globala
* variabler initieras via anrop av funktionen setup. En while-loop används för
* att hålla igång programmet så länge matningsspänning tillförs. Resterande
* del av programmet är avbrottsgenererat.
******************************************************************************/
int main(void)
{	
	setup();
	DISABLE_SERIAL_TRANSMISSION;
	
    while (true)
	{
	WDT_reset();
	}
	return 0;
}



// Av Jacob Lundkvist