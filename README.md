# Projekt-1-ELA21
Inlämningsuppgift för inbyggda system 1.

För ändringar som har utförts från originalkoden som var skriven i C89, se -> changelog.md

## Skolprojekt
 Felsök och uppdatera en befintlig C-kod från C89 till C99 och optimera
 koden.

 Extra: Lägg till funktionalitet så som, WDT, 7seg, vector, dynamisk timer,
 EEPROM
 Implementerat:  Watchdog, 7-segmentsdisplay, Vector, Dynamisk timer.
 *****************************************************************************
 Innan så exekverade inte koden på grund av flera fel.
 Makron som var feldefinierade och andra makron som användes på fel ställen.
 Felaktiga beräkningar korrigerades

 Programmet var tänkt att skriva ut temperatur seriellt var 60:e sekund.
 Om knapp på PIN 13 trycktes ner så återställdes timern samtidigt som
 temperaturen skrevs ut seriellt.
 Vid varje utskrift så togglades en lysdiod för indikation av att avläsning
 har ägt rum.
 *****************************************************************************
 I det uppdaterade programmet så har  delar av koden slopats då de inte
 hade något användningsområde i den uppdaterade koden.

 Borttagna filer: MemoryBlock.h, MemoryBlock.c, Byte.h, Byte.c	

 En LED har bytts ut mot två stycken 7-segments displayer som visar aktuell
 temperatur efter att en dynamisk timer aktiveras via tryckknapp på PIN 13.
 Den dynamiska timern håller upp till 10 värden som sparar tiden mellan
 varje tryckknapp. Summan av all tid divideras med det antalet tider som har
 sparats (upp till 10 värden).

 Seriell överföring inaktiveras efter initieringsrutinen har gjort sitt.
 Därefter är allt styrt via avbrottsrutiner där main loop som kontinuerligt
 återställer räknaren för Watchdog-timern så länge programmet exekverar
 normalt.
 
 7 segmentsdisplayerna fungerar på ett sådant sätt att dom skiftar mellan
 display 1 och display 2 varje millesekund samtidigt som PIN 2-8 skiftar
 värden för att representera rätt värde på displayen.

 *****************************************************************************
 
 ##                      PINS USED ON ARDUINO UNO
			 PIN 2-8 for 7seg segments
			 PIN 10  for first  digit
			 PIN 11  for second digit
			 PIN 13  for button
			 PIN A1  for tmp36 sensor
 *****************************************************************************
 
## Diskussion

Ett intressant skolprojekt som framhäver ens egna preferenser angående utforming av kod. Projektet gav även en större insikt i hur kod som är tillämpad enligt C89 kan se ut och hur man kan anpassa den till den något mer moderna C99. 
I slutet av detta projekt så har man fått en bättre felsökningsmetod och man känner sig mer bekväm med att felsöka C-kod.

Projektet kändes något överväldigande först då det var väldigt mycket fel att åtgärda.

--- MORE TO COME- --
