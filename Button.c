/* Inkluderingsdirektiv: */
#include "GPIO.h"

/******************************************************************************
* Initieringsrutin för tryckknappar. * Ingående argument PIN utgör aktuellt 
* PIN-nummer sett till Arduino Uno (PIN 0 - 13), som är ekvivalent med följande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN på Arduino Uno        *
*     8 - 13                    B            PIN på Arduino Uno - 8           *
*******************************************************************************
*
* Först allokeras minne för ett nytt objekt av strukten Button döpt self. 
* Om minnesallokeringen misslyckas så returneras NULL direkt. Annars initieras 
* objektets instansvariabler. Specificerat PIN-nummer sparas, följt av att denna 
* undersöks för att ta reda på vilken I/O-port som tryckknappaen är ansluten 
* till, där 0 - 7 innebär att tryckknappen är ansluten till samma PIN på 
* I/O-port D. Annars om aktuellt PIN-nummer ligger mellan 8 - 13, så är 
* tryckknappen ansluten till PIN 0 - 5  på I/O-port B. En intern 
* pullup-resistor på tryckknappens PIN aktiveras via ettställning av 
* motsvarande bit i aktuellt PORT-register, vilket medför att tryckknappens
* insignal alltid är hög eller låg (0 eller 1).
******************************************************************************/
struct Button* new_Button(unsigned char* PIN)
{
	struct Button* self = (struct Button*)malloc(sizeof(struct Button));
	if (!self)
	{
		return NULL;
	}
	
	(*self).PIN = *PIN;
	(*self).interrupt_enabled = false;
	
	if ((*self).PIN >= 0 && (*self).PIN <= 7)
	{
		(*self).io_port = IO_PORTD;
		SET_BIT(PORTD, (*self).PIN);
	}
	
	else if ((*self).PIN >= 8 && (*self).PIN <= 13)
	{
		(*self).io_port = IO_PORTB;
		SET_BIT(PORTB, (*self).PIN);
	}
	
	return self;
}

/******************************************************************************
* Funktionen Button_is_pressed används för att indikera ifall en given 
* tryckknapp är nedtryckt. Om tryckknappen är ansluten till I/O-port B, så
* läses motsvarande PIN från registret PINB och returneras. Annars om 
* tryckknappen är ansluten till I/O-port B, så läses motsvarande PIN från 
* registret PIND och returneras. Annars vid fel så returneras false.
******************************************************************************/
enum bool Button_is_pressed(struct Button* self)
{
	if ((*self).io_port == IO_PORTB)
	{
		return (READ_BIT(PINB, (*self).PIN));
	}
	
	else if ((*self).io_port == IO_PORTD)
	{
		return (READ_BIT(PIND, (*self).PIN));
	}
	
	return false;	
}

/******************************************************************************
* Funktionen Button_enable_interrupt används för att aktivera PCI-avbrott på
* en given PIN som en tryckknapp är ansluten till. Först aktiveras avbrott
* globalt via ettställning av bit I (Interrupt Flag) i statusregistret SREG.
* Därefter undersöks använd I/O-port. Om tryckknappen är ansluten till
* I/O-port B, så ettställs biten PCIE0 (PIN Change Interrupt Enable 0) i
* kontrollregistret PCICR (PIN Change Interrupt Control Register), följt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK0 (PIN Change
* Mask Register 0). Annars om tryckknappen är ansluten till I/O-port D,
* så ettställs biten PCIE2 (PIN Change Interrupt Enable 2) i kontrollregistret
* PCICR (PIN Change Interrupt Control Register), följt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK2 (PIN Change
* Mask Register 2). Slutligen sätts instansvariabeln interrupt_enabled till
* true för att indikera att abrott nu är aktiverat.
******************************************************************************/
void Button_enable_interrupt(struct Button* self)
{
	if ((*self).io_port == IO_PORTB)
	{
		union Byte byte = new_Byte(PCICR);
		byte.bits.bit0 = 1;
		ASSIGN(PCICR, byte.data);
		ASSIGN(PCMSK1, (*self).PIN);
	}
	
	else if ((*self).io_port == IO_PORTD)
	{
		union Byte byte = new_Byte(PCICR);
		byte.bits.bit2 = 1;
		ASSIGN(PCICR, byte.data);
		ASSIGN(PCMSK2, (*self).PIN);
	}
	
	(*self).interrupt_enabled = true;
	return;
}

/******************************************************************************
* Funktionen Button_disable_interrupt används för att inaktivera avbrott för
* en given PIN, där en tryckknapp är ansluten. Detta åstadkommes via
* nollställning av motsvarande bit i någon av registren PCMSK0 (I/O-port B)
* eller PCMSK2 (I/O-port D).
******************************************************************************/
void Button_disable_interrupt(struct Button* self)
{
	if ((*self).io_port == IO_PORTB)
	{
		union Byte byte = new_Byte(PCMSK0);
		byte.bits.bit0 = 0;
		ASSIGN(PCMSK0, byte.data);
	}
	
	else if ((*self).io_port == IO_PORTD)
	{
		union Byte byte = new_Byte(PCMSK2);
		byte.bits.bit2 = 0;
		ASSIGN(PCMSK2, byte.data);
	}
	
	(*self).interrupt_enabled = false;
	return;
}
