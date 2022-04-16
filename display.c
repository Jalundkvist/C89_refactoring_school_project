// Inkluderingsdirektiv:
#include "GPIO.h"

static void Display_on(Display* self);
static void Display_off(Display* self);
static void Display_toggle(Display* self);

/******************************************************************************
* Funktionen new_Display utgör initieringsrutin för objekt av strukten Led.
* Ingående argument PIN utgör aktuellt PIN-nummer sett till Arduino Uno
* (PIN 0 - 13), som är ekvivalent med följande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN på Arduino Uno        *
*     8 - 13                    B            PIN på Arduino Uno - 8           *
*******************************************************************************
*
* Objektet lagras på den tillfälliga variabeln self. 
* Om aktuellt PIN-nummer ligger mellan 0 - 7, så är displayen ansluten till 
* I/O-port D, vilket lagras via instansvariabeln io_port.
* Aktuellt PORT-nummer är då samma samma som PIN-numret, vilket lagras
* via instansvariabel PIN. Denna PIN sätts till utport genom att
* motsvarande bit i datariktningsregister DDRD (Data Direction Register D)
* ettställs. Bitvis OR |= används för att enbart ettställa aktuell bit utan 
* att påverka övriga bitar.
* 
* Motsvarande genomförs ifall aktuellt PIN-nummer ligger mellan 8 - 13, med
* skillnaden att I/O-porten då utgörs av I/O-port B, PIN-numret är lika med
* erhållet PIN-nummer på Arduino Uno - 8, och motsvarande PIN sätts till
* utport via ettställning av motsvarande bit i datariktningsregister DDRB
* (Data Direction Register B).
*
* Slutligen sätts pekarna till att peka på motsvarande funktioner, följt
* av att det nu initierade objektet returneras.
******************************************************************************/
Display new_Display(uint8_t PIN)
{
	Display self;
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
	self.on = Display_on;
	self.off = Display_off;
	self.toggle = Display_toggle;
	 return self;
}

/******************************************************************************
* Funktionen Display_on används för att tända en display. Ingående argument self
* utgör en pekare till display-objektet i fråga. Utefter aktuell I/O-port så 
* ettställs motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
void Display_on(Display* self)
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
* Funktionen Display_off används för att släcka en display. Ingående argument
* self utgör en pekare till 7 segment displayen. Utefter aktuell I/O-port så 
* nollställs motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
void Display_off(Display* self)
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
* Funktionen Display_toggle används för att toggla en display. För att genomföra
* detta undersöks medlemmen enabled. Om denna är true så är displayen tänd
* och då släcks displayen via anrop av funktionen Display_off (via pekaren off).
* Annars så tänds displayen via anrop av funktionen Display_on (via pekaren on).
******************************************************************************/
void Display_toggle(Display* self)
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