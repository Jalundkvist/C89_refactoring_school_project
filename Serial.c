/* Inkluderingsdirektiv: */
#include "Serial.h"

 /******************************************************************************
 * Funktionen init_serial används för att initiera seriell överföring. För att
 * inte genomföra multipla initieringar, så undersöks först ifall den statiska
 * lokala variabeln serial_initialized är true. Denna varibel är satt till
 * statisk så att minne enbart allokeras för denna en gång, där startvärdet
 * sätts till false. Därmed skapas inte en ny lokal variant av denna variabel
 * varje gång funktionen exekverar. Ifall denna variabels värde senare ändras 
 * till true, så bibehålls detta värde. Varje gång funktionen exekverar så
 * undersöks därmed ifall denna variabels värde är true.  Om detta är fallet, 
 * vilket indikerar att seriell överföring redan har initierats, så avslutas 
 * funktionen direkt. Annars aktiveras seriell transmission för asynkron 
 * överföring av en byte (motsvarar ett tecken) i taget med en bithastighet 
 * på 115 220 kbps. För att första transmitterade utskrift skall hamna längst
 * till vänster på den första raden så transmitteras ett vagnreturstecken \r, 
 * följt av ett nolltecken \0 för att indikera att transmissionen är slutförd.
 ******************************************************************************/
void init_serial(void)
{
	static enum bool serial_initialized = false; 
	
	if (serial_initialized)
	{
		 return;
	}
	
	ENABLE_SERIAL_TRANSMISSION;
	SET_BAUD_RATE_TO_115220_KBPS;
	CARRIAGE_RETURN;
	END_TRANSMISSION;
	serial_initialized = true;
	return;
}

/******************************************************************************
* Funktionen serial_print används för att transmittera ett textstycke via
* seriell överföring. Ingående argument s utgör en pekare till textstycket.
* Varje tecken i strängen transmitteras en efter en tills ett nolltecken nås,
* där funktionen write_byte används för att skicka respektive tecken. Ifall
* ett nyradstecken \n transmitteras så trasmitteras ett vagnreturstecken \r
* direkt efter för att efterföljande tecken skall hamna längst till vänster
* på nästa rad. Transmissionen avslutas med att ett nolltecken \0 för att
* indikera textstyckets slut.
******************************************************************************/
void serial_print(char* s)
{
	unsigned long i = 0;
		
	loop:
		if (s[i])
		{
			write_byte(&s[i]);
			
			if (s[i++] == '\n')
			{
				CARRIAGE_RETURN;
			}
				
			goto loop;
		}
	
	END_TRANSMISSION;
	return;
}

/******************************************************************************
* Funktionen Serial_print_integer används för att sammansätta ett textstycke
* med ett signerat heltal. Ingående argument s utgör en pekare till textstycket,
* medan number utgör det signerade talet. Först deklareras en sträng döpt text,
* som rymmer 100 tecken. Denna sträng initieras till tom vid start genom att
* denna tilldelas ett nolltecken \0 längst fram (på index 0). Sedan sammansätts
* innehållet som pekaren s pekar på med det signerade heltalet number och
* lagras i strängen text, vilket åstadkommes via anrop av funktionen sprintf 
* från biblioteket stdio.h. Detta sammansatta textstycke, som nu lagras i 
* strängen text, passeras sedan som ingående argument vid anrop av funktionen 
* serial_print för transmission.
******************************************************************************/
 void serial_print_integer(char* s, long* number)
{
	char text[SIZE];
	sprintf(s, text, *number);
	serial_print(s);
	return;
}

/******************************************************************************
* Funktionen serial_print_unsigned används för att sammansätta ett textstycke
* med ett osignerat heltal. Ingående argument s utgör en pekare till aktuellt
* textstycket, medan number utgörs av det osignerade talet. Först deklareras 
* en sträng döpt text, som rymmer 100 tecken. Därefter sammansätts innehållet 
* som pekaren s pekar på med det osignerade heltalet number och lagras i 
* strängen text, vilket åstadkommes via anrop av funktionen sprintf från 
* biblioteket stdio.h. Detta sammansatta textstycke, som nu lagras i strängen 
* text, passeras sedan som ingående argument vid anrop av funktionen 
* serial_print för transmission.
******************************************************************************/
void serial_print_unsigned(char* s, unsigned long* number)
{	
	char text[SIZE];
	sprintf(s, text, *number);
	serial_print(s);
	return;
}

/******************************************************************************
* Funktionen write_byte används för att transmittera en byte, vilket motsvarar
* ett tecken. Ingående argument data utgörs av aktuellt tecken som skall
* transmitteras. Först väntar vi in att eventuellt föregående tecken har 
* transmitterats, vilket sker då dataregistret UDR0 (USART Data Register 0).
* Så fort detta register är tomt så placeras det nya tecknet data i detta
* register för transmission. 
******************************************************************************/
void write_byte(char* data)
{
	WAIT_FOR_PREVIOUS_TRANSMISSION_TO_FINISH;
	UDR0 = *data;
	return;
}