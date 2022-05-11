/******************************************************************************
*								Skolprojekt									  *
* Fels�k och uppdatera en befintlig C-kod fr�n C89 till C99 och optimera	  *
* koden.																	  *
*																			  *
* Extra: L�gg till funktionalitet s� som, WDT, 7seg, vector, dynamisk timer,  *
*		 EEPROM																  *
* Implementerat:  Watchdog, 7-segmentsdisplay, Vector, Dynamisk timer.		  *
*******************************************************************************
* Innan s� exekverade inte koden p� grund av flera fel.						  *
* Makron som var feldefinierade och andra makron som anv�ndes p� fel st�llen. *
* Felaktiga ber�kningar korrigerades										  *
*																			  *
* Programmet var t�nkt att skriva ut temperatur seriellt var 60:e sekund.	  *
* Om knapp p� PIN 13 trycktes ner s� �terst�lldes timern samtidigt som        *
* temperaturen skrevs ut seriellt.											  *
* Vid varje utskrift s� togglades en lysdiod f�r indikation av att avl�sning  *
* har �gt rum.																  *
*******************************************************************************
* I det uppdaterade programmet s� har  delar av koden slopats d� de inte      *
* hade n�got anv�ndningsomr�de i den uppdaterade koden.						  *
*																			  *
* Borttagna filer: MemoryBlock.h, MemoryBlock.c, Byte.h, Byte.c			      *
*																			  *
* En LED har bytts ut mot tv� stycken 7-segments displayer som visar aktuell  *
* temperatur efter att en dynamisk timer aktiveras via tryckknapp p� PIN 13.  *
* Den dynamiska timern h�ller upp till 10 v�rden som sparar tiden mellan	  *
* varje tryckknapp. Summan av all tid divideras med det antalet tider som har *
*  sparats (upp till 10 v�rden).											  *
*																			  *
* Seriell �verf�ring inaktiveras efter initieringsrutinen har gjort sitt.	  *
* D�refter �r allt styrt via avbrottsrutiner d�r main loop som kontinuerligt  *
* �terst�ller r�knaren f�r Watchdog-timern s� l�nge programmet exekverar	  *
* normalt.																	  *
******************************************************************************/

// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Funktionen main utg�r programmets start- och slutpunkt. Programmets globala
* variabler initieras via anrop av funktionen setup. Efter att setup-rutinen
* har k�rts s� avaktiveras seriell �verf�ring via funktionen disable_serial
* d� temperaturen g�r att avl�sa p� 7 segmentsdisplayer.
*
* En while-loop anv�nds f�r att h�lla programmet ig�ng och f�r att �terst�lla 
* Watchdog-timern via WD_reset s� l�nge matningssp�nning tillf�rs.
* Om programmet fastnar i exekvering utanf�r mainloop s� �terst�lls programmet.
*
* Resterande del av programmet �r avbrottsgenererat.
******************************************************************************/
int main(void)
{	
	setup();
	disable_serial(true);
    while (true)
	{
	WD_reset();
	}
	return 0;
}
