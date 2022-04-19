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
* Funktionen new_Display utg�r initieringsrutin f�r objekt av strukten Led.
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
* Om aktuellt PIN-nummer ligger mellan 0 - 7, s� �r displayen ansluten till 
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
* Funktionen Display_on anv�nds f�r att t�nda en display. Ing�ende argument self
* utg�r en pekare till display-objektet i fr�ga. Utefter aktuell I/O-port s� 
* ettst�lls motsvarande bit i register PORTB eller PORTD.
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
* Funktionen Display_off anv�nds f�r att sl�cka en display. Ing�ende argument
* self utg�r en pekare till 7 segment displayen. Utefter aktuell I/O-port s� 
* nollst�lls motsvarande bit i register PORTB eller PORTD.
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
* Funktionen Display_enable anv�nds f�r att toggla en display. F�r att genomf�ra
* detta unders�ks medlemmen enabled. Om denna �r true s� �r displayen t�nd
* och d� sl�cks displayen via anrop av funktionen Display_off (via pekaren off).
* Annars s� t�nds displayen via anrop av funktionen Display_on (via pekaren on).
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
* Funktionen display_write anv�nds f�r att skriva ut en siffra 0 - 9 p� en 7-segmentsdisplay.
* Eftersom PIN 1 - 7 anv�nds f�r segmenten, samtidigt som alla makron �r gjorda f�r PIN 0 - 6,
* s� skiftas bitarna i den bin�ra koden ett steg �t v�nster via bitvis skiftning.
* Vid fel (om siffran �verstiger nio, s� sl�cks displayen).
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

static inline void write_MSB(const uint8_t digit) // Ing�ende argument �r ett makro ZERO - NINE. MSB f�r siffra �r bit 6.
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