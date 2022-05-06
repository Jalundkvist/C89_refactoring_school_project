// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Funktionen main utgör programmets start- och slutpunkt. Programmets globala
* variabler initieras via anrop av funktionen setup. Efter att setup-rutinen
* har körts så avaktiveras seriell överföring via funktionen disable_serial
* då temperaturen går att avläsa på 7 segmentsdisplayer.
*
* En while-loop används för att hålla programmet igång och för att återställa 
* Watchdog-timern via WDT_reset så länge matningsspänning tillförs.
* Om programmet fastnar i exekvering utanför mainloop så återställs programmet.
*
* Resterande del av programmet är avbrottsgenererat.
******************************************************************************/
int main(void)
{	
	setup();
	disable_serial(false);
	
    while (true)
	{
	WDT_reset();
	}
	return 0;
}



/****************************************************************************** 
*								Skolprojekt
* Felsök och uppdatera en befintlig C-kod från C89 till C99 och optimera koden.
*
* Innan så exekverade inte koden på grund av flera fel.
* Makron som var feldefinierade och andra makron som användes på fel ställen.
* Felaktiga beräkningar korrigerades


* Från fast temperaturavläsning var 60 sekund om inte timern återställs vid
* knapptryck som skriver ut temperaturen seriellt tidigare.
*
* I det uppdaterade programmet så har vissa delar av koden slopats då de inte
* hade något användningsområde.
* 
* Implementerat:  Watchdog, 7-segmentsdisplay, Vector, Dynamisk timer.
*
* En LED har bytts ut mot två stycken 7-segments displayer som visar aktuell
* temperatur efter att en dynamisk timer aktiveras via tryckknapp på PIN 13.
* Den dynamiska timern håller upp till 10 värden som sparar tiden mellan varje
* tryckknapp. Summan av all tid divideras med det antalet tider som har sparats
* (upp till 10 värden).
*
******************************************************************************/