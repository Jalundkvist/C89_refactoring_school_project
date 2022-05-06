// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Funktionen main utg�r programmets start- och slutpunkt. Programmets globala
* variabler initieras via anrop av funktionen setup. Efter att setup-rutinen
* har k�rts s� avaktiveras seriell �verf�ring via funktionen disable_serial
* d� temperaturen g�r att avl�sa p� 7 segmentsdisplayer.
*
* En while-loop anv�nds f�r att h�lla programmet ig�ng och f�r att �terst�lla 
* Watchdog-timern via WDT_reset s� l�nge matningssp�nning tillf�rs.
* Om programmet fastnar i exekvering utanf�r mainloop s� �terst�lls programmet.
*
* Resterande del av programmet �r avbrottsgenererat.
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
* Fels�k och uppdatera en befintlig C-kod fr�n C89 till C99 och optimera koden.
*
* Innan s� exekverade inte koden p� grund av flera fel.
* Makron som var feldefinierade och andra makron som anv�ndes p� fel st�llen.
* Felaktiga ber�kningar korrigerades


* Fr�n fast temperaturavl�sning var 60 sekund om inte timern �terst�lls vid
* knapptryck som skriver ut temperaturen seriellt tidigare.
*
* I det uppdaterade programmet s� har vissa delar av koden slopats d� de inte
* hade n�got anv�ndningsomr�de.
* 
* Implementerat:  Watchdog, 7-segmentsdisplay, Vector, Dynamisk timer.
*
* En LED har bytts ut mot tv� stycken 7-segments displayer som visar aktuell
* temperatur efter att en dynamisk timer aktiveras via tryckknapp p� PIN 13.
* Den dynamiska timern h�ller upp till 10 v�rden som sparar tiden mellan varje
* tryckknapp. Summan av all tid divideras med det antalet tider som har sparats
* (upp till 10 v�rden).
*
******************************************************************************/