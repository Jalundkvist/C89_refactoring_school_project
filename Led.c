// Inkluderingsdirektiv:
#include "GPIO.h"

static void Led_on(Led* self);
static void Led_off(Led* self);
static void Led_toggle(Led* self);

/******************************************************************************
* Funktionen new_Led utgör initieringsrutin för objekt av strukten Led.
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
* Om aktuellt PIN-nummer ligger mellan 0 - 7, så är lysdioden ansluten till 
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
* Funktionen Led_on används för att tända en lysdiod. Ingående argument self
* utgör en pekare till led-objektet i fråga. Utefter aktuell I/O-port så 
* ettställs motsvarande bit i register PORTB eller PORTD.
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
* Funktionen Led_off används för att släcka en lysdiod. Ingående argument
* self utgör en pekare till lysdioden. Utefter aktuell I/O-port så nollställs
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
* Funktionen Led_toggle används för att toggla en lysdiod. För att genomföra
* detta undersöks medlemmen enabled. Om denna är true så är lysdioden tänd
* och då släcks lysdioden via anrop av funktionen Led_off (via pekaren off).
* Annars så tänds lysdioden via anrop av funktionen Led_on (via pekaren on).
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