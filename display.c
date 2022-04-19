// Inkluderingsdirektiv:
#include "GPIO.h"

static uint8_t Check_IO_port(uint8_t PIN);
static void Display_on(Display* self, CurrentDisplay currentDisplay);
static void Display_off(Display* self, CurrentDisplay currentDisplay);
static void Display_enable(Display* self);
static void Display_write(const uint8_t digit);
static void Display_update_digit(Display* self, const uint8_t temp);
static inline void write_MSB(const uint8_t digit);

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
Display new_Display(uint8_t D1_PIN, uint8_t D2_PIN)
{
	Display self;
	
	self.D1_PIN = Check_IO_port(D1_PIN);
	self.D2_PIN = Check_IO_port(D2_PIN);
	
	
	self.currentDigit = DIGIT1;
	
	self.on = Display_on;
	self.off = Display_off;
	self.enable = Display_enable;
	self.update_digit = Display_update_digit;
	self.enabled = true;
	return self;
}

static uint8_t Check_IO_port(uint8_t PIN)
{
	if (PIN >= 0 && PIN <= 7)
		return PIN;
	
	else if (PIN >= 8 && PIN <= 13)
	{
		PIN = PIN - 8;
		DDRB |= (1 << PIN);
	}
	return PIN;
}

/******************************************************************************
* Funktionen Display_on används för att tända en display. Ingående argument self
* utgör en pekare till display-objektet i fråga. Utefter aktuell I/O-port så 
* ettställs motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
void Display_on(Display* self, CurrentDisplay current_display)
{
	if (current_display == DISPLAY1)
	{
		PORTB |= (1<<self->D1_PIN);
	}
	else if (current_display == DISPLAY2)
	{
		PORTB |= (1<<self->D2_PIN);
	}
	
	
	return;	
}

/******************************************************************************
* Funktionen Display_off används för att släcka en display. Ingående argument
* self utgör en pekare till 7 segment displayen. Utefter aktuell I/O-port så 
* nollställs motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
void Display_off(Display* self, CurrentDisplay current_display)
{
	if (current_display == DISPLAY1)
	{
		PORTB &= ~(1<<self->D1_PIN);
	}
	
	else if (current_display == DISPLAY2)
	{
		PORTB &= ~(1<<self->D2_PIN);
	}
	return;
}

/******************************************************************************
* Funktionen Display_enable används för att toggla en display. För att genomföra
* detta undersöks medlemmen enabled. Om denna är true så är displayen tänd
* och då släcks displayen via anrop av funktionen Display_off (via pekaren off).
* Annars så tänds displayen via anrop av funktionen Display_on (via pekaren on).
******************************************************************************/
void Display_enable(Display* self)
{
	if (self->enabled)
	{
		self->off(self, DISPLAY1);
		self->off(self, DISPLAY2);
		self->enabled = false;
	}
	
	else
	{
		self->enabled = true;
	}
	
	return;
}

/*********************************************************************************************
* Funktionen display_write används för att skriva ut en siffra 0 - 9 på en 7-segmentsdisplay.
* Eftersom PIN 1 - 7 används för segmenten, samtidigt som alla makron är gjorda för PIN 0 - 6,
* så skiftas bitarna i den binära koden ett steg åt vänster via bitvis skiftning.
* Vid fel (om siffran överstiger nio, så släcks displayen).
*********************************************************************************************/
static void Display_write(const uint8_t digit)
{
	// #define ZERO 0x40 0100 0000
	if (digit == 0) 
	{
		PORTD = (uint8_t)(ZERO << 2);
		write_MSB(ZERO);
	}
	else if (digit == 1)
	{
		PORTD = (uint8_t)(ONE << 2);
		write_MSB(ONE);
	}
	else if (digit == 2) 
	{
		PORTD = (TWO << 2);
		write_MSB(TWO);
	}	
	else if (digit == 3) 
	{
		PORTD = (THREE << 2);
		write_MSB(THREE);
	}
	else if (digit == 4) 
	{
		PORTD = (FOUR << 2);
		write_MSB(FOUR);
	}	
	else if (digit == 5) 
	{
		PORTD = (FIVE << 2);
		write_MSB(FIVE);
	}	
	else if (digit == 6) 
	{
		PORTD = (SIX << 2);
		write_MSB(SIX);
	}	
	else if (digit == 7) 
	{
		PORTD = (uint8_t)(SEVEN << 2);
		write_MSB(SEVEN);
	}	
	else if (digit == 8) 
	{
		PORTD = (EIGHT << 2);
		write_MSB(EIGHT);
	}	
	else if (digit == 9) 
	{
		PORTD = (NINE << 2);
		write_MSB(NINE);
	}	
	else 
	{
		PORTD = 0x00;
		PORTB &= ~(1 << 0);
	}
	
	return;
}

static inline void write_MSB(const uint8_t digit) // Ingående argument är ett makro ZERO - NINE. MSB för siffra är bit 6.
{
	const uint8_t number = digit & (1 << 6);
	if (number) 
		PORTB |= (1<<0);
	else 
		PORTB &= ~(1<<0);
	return;
}

static void Display_update_digit(Display* self, uint8_t temp)
{
	if (!self->enabled) return;
	const uint8_t digit1 = temp/10;
	const uint8_t digit2 = temp - (digit1 * 10);
	
	if (self->currentDigit == DIGIT1)
		self->currentDigit = DIGIT2;
	else
		self->currentDigit = DIGIT1;
		
	if (self->currentDigit == DIGIT1)
	{
		self->on(self, DISPLAY1);
		self->off(self, DISPLAY2);
		Display_write(digit1);
	}
	else
	{
		self->on(self, DISPLAY2);
		self->off(self, DISPLAY1);
		Display_write(digit2);
	}
	
	return;
}