// Inkluderingsdirektiv:
#include "GPIO.h"

static bool Button_is_pressed(struct Button* self);
static void Button_enable_interrupt(struct Button* self);
static void Button_disable_interrupt(struct Button* self);

/******************************************************************************
* Initieringsrutin f�r tryckknappar. * Ing�ende argument PIN utg�r aktuellt 
* PIN-nummer sett till Arduino Uno (PIN 0 - 13), som �r ekvivalent med f�ljande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN p� Arduino Uno        *
*     8 - 13                    B            PIN p� Arduino Uno - 8           *
*******************************************************************************
* F�rst initieras nytt objekt av strukten Button d�pt self. 
* Sedan initieras objektets instansvariabler. Specificerat PIN-nummer sparas,
* f�ljt av att denna unders�ks f�r att ta reda p� vilken I/O-port som 
* tryckknappen �r ansluten till, d�r 0 - 7 inneb�r att tryckknappen �r 
* ansluten till samma PIN p� I/O-port D. 
* Annars om aktuellt PIN-nummer ligger mellan 8 - 13, s� �r 
* tryckknappen ansluten till PIN 0 - 5  p� I/O-port B. En intern 
* pullup-resistor p� tryckknappens PIN aktiveras via ettst�llning av 
* motsvarande bit i aktuellt PORT-register, vilket medf�r att tryckknappens
* insignal alltid �r h�g eller l�g (0 eller 1).
******************************************************************************/
Button new_Button(uint8_t PIN)
{
	Button self;
	self.interrupt_enabled = false;
	if (PIN >= 0 && PIN <= 7)
	{
		self.io_port = IO_PORTD;
		self.PIN = PIN;
		PORTD |= (1<< self.PIN);
	}
	
	else if (PIN >= 8 && PIN <= 13)
	{
		self.io_port = IO_PORTB;
		self.PIN = PIN - 8;
		PORTB |= (1<< self.PIN);
	}
	self.is_pressed = Button_is_pressed;
	self.enable_interrupt = Button_enable_interrupt;
	self.disable_interrupt = Button_disable_interrupt;
	return self;
}

/******************************************************************************
* Funktionen Button_is_pressed anv�nds f�r att indikera ifall en given 
* tryckknapp �r nedtryckt. Om tryckknappen �r ansluten till I/O-port B, s�
* l�ses motsvarande PIN fr�n registret PINB och returneras. Annars om 
* tryckknappen �r ansluten till I/O-port B, s� l�ses motsvarande PIN fr�n 
* registret PIND och returneras. Annars vid fel s� returneras false.
******************************************************************************/
static bool Button_is_pressed(Button* self)
{
	if (self->io_port == IO_PORTD)
	{
		return (PIND & (1<<self->PIN));
	}
	
	else if (self->io_port == IO_PORTB)
	{
		return (PINB & (1<<self->PIN));
	}
	
	return false;	
}

/******************************************************************************
* Funktionen Button_enable_interrupt anv�nds f�r att aktivera PCI-avbrott p�
* en given PIN som en tryckknapp �r ansluten till. F�rst aktiveras avbrott
* globalt via ettst�llning av bit I (Interrupt Flag) i statusregistret SREG.
* D�refter unders�ks anv�nd I/O-port. Om tryckknappen �r ansluten till
* I/O-port B, s� ettst�lls biten PCIE0 (PIN Change Interrupt Enable 0) i
* kontrollregistret PCICR (PIN Change Interrupt Control Register), f�ljt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK0 (PIN Change
* Mask Register 0). Annars om tryckknappen �r ansluten till I/O-port D,
* s� ettst�lls biten PCIE2 (PIN Change Interrupt Enable 2) i kontrollregistret
* PCICR (PIN Change Interrupt Control Register), f�ljt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK2 (PIN Change
* Mask Register 2). Slutligen s�tts instansvariabeln interrupt_enabled till
* true f�r att indikera att abrott nu �r aktiverat.
******************************************************************************/
static void Button_enable_interrupt(Button* self)
{
	asm("SEI");
	
	if (self->io_port == IO_PORTB)
	{
		PCICR |= (1<<PCIE0);
		PCMSK0 |= (1<<self->PIN);
	}
	
	else if (self->io_port == IO_PORTD)
	{
		PCICR |= (1<<PCIE2);
		PCMSK2 |= (1<<self->PIN);
	}
	
	self->interrupt_enabled = true;
	return;
}

/******************************************************************************
* Funktionen Button_disable_interrupt anv�nds f�r att inaktivera avbrott f�r
* en given PIN, d�r en tryckknapp �r ansluten. Detta �stadkommes via
* nollst�llning av motsvarande bit i n�gon av registren PCMSK0 (I/O-port B)
* eller PCMSK2 (I/O-port D).
******************************************************************************/
static void Button_disable_interrupt(Button* self)
{
	if (self->io_port == IO_PORTD)
	{
		PCMSK2 &= ~(1<<self->PIN);
	}
	

	else if(self->io_port == IO_PORTB)
	{
		PCMSK0 &= ~(1<<self->PIN);
	}
	
	self->interrupt_enabled = false;
	return;
}
