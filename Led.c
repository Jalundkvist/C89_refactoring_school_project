// Inkluderingsdirektiv:
#include "GPIO.h"

static void Led_on(Led* self);
static void Led_off(Led* self);
static void Led_toggle(Led* self);

/******************************************************************************
* Funktionen new_Led utg�r initieringsrutin f�r objekt av strukten Led.
* Ing�ende argument PIN utg�r aktuellt PIN-nummer sett till Arduino Uno
* (PIN 0 - 13), som �r ekvivalent med f�ljande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN p� Arduino Uno        *
*     8 - 13                    B            PIN p� Arduino Uno - 8           *
*******************************************************************************
*
* Objektet lagras p� den tillf�lliga variabeln self. 
* Om aktuellt PIN-nummer ligger mellan 0 - 7, s� �r lysdioden ansluten till 
* I/O-port D, vilket lagras via instansvariabeln io_port.
* Aktuellt PORT-nummer �r d� samma samma som PIN-numret, vilket lagras
* via instansvariabel PIN. Denna PIN s�tts till utport genom att
* motsvarande bit i datariktningsregister DDRD (Data Direction Register D)
* ettst�lls. Bitvis OR |= anv�nds f�r att enbart ettst�lla aktuell bit utan 
* att p�verka �vriga bitar.
* 
* Motsvarande genomf�rs ifall aktuellt PIN-nummer ligger mellan 8 - 13, med
* skillnaden att I/O-porten d� utg�rs av I/O-port B, PIN-numret �r lika med
* erh�llet PIN-nummer p� Arduino Uno - 8, och motsvarande PIN s�tts till
* utport via ettst�llning av motsvarande bit i datariktningsregister DDRB
* (Data Direction Register B).
*
* Slutligen s�tts pekarna till att peka p� motsvarande funktioner, f�ljt
* av att det nu initierade objektet returneras.
******************************************************************************/
Led new_Led(uint8_t PIN)
{
	Led self;
	self.enabled = false;
	self.PIN = PIN;
	
	if (self.PIN >= 0 && self.PIN <= 7) 
	{
		self.io_port = IO_PORTD;
		DDRD |= (1 << self.PIN);
	}
	
	else if (self.PIN >= 8 && self.PIN <= 13)
	{
		self.PIN = PIN - 8;
		self.io_port = IO_PORTB;
		DDRB |= (1 << self.PIN);
	}
	self.on = Led_on;
	self.off = Led_off;
	self.toggle = Led_toggle;
	 return self;
}

/******************************************************************************
* Funktionen Led_on anv�nds f�r att t�nda en lysdiod. Ing�ende argument self
* utg�r en pekare till led-objektet i fr�ga. Utefter aktuell I/O-port s� 
* ettst�lls motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
void Led_on(Led* self)
{
	if (self->io_port == IO_PORTB)
	{
		PORTB |=  (1 << self->PIN);
	}
	
	else if (self->io_port == IO_PORTD)
	{
		PORTD |= (1 << self->PIN);
	}
	
	self->enabled = true;
	return;	
}

/******************************************************************************
* Funktionen Led_off anv�nds f�r att sl�cka en lysdiod. Ing�ende argument
* self utg�r en pekare till lysdioden. Utefter aktuell I/O-port s� nollst�lls
* motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
void Led_off(Led* self)
{
	if (self->io_port == IO_PORTB)
	{
		PORTB &= ~(1<<self->PIN);
	}
	
	else if (self->io_port == IO_PORTD)
	{
		PORTD &= ~(1<<self->PIN);
	}
	
	self->enabled = false;
	return;
}

/******************************************************************************
* Funktionen Led_toggle anv�nds f�r att toggla en lysdiod. F�r att genomf�ra
* detta unders�ks medlemmen enabled. Om denna �r true s� �r lysdioden t�nd
* och d� sl�cks lysdioden via anrop av funktionen Led_off (via pekaren off).
* Annars s� t�nds lysdioden via anrop av funktionen Led_on (via pekaren on).
******************************************************************************/
void Led_toggle(Led* self)
{
	if (self->enabled)
	{
		self->off(self);
	}
	
	else
	{
		self->on(self);
	}
	
	return;
}