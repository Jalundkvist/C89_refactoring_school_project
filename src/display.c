// Inkluderingsdirektiv:
#include "GPIO.h"

static uint8_t Check_IO_port(uint8_t PIN);
static void Display_on(Display* self, CurrentDisplay currentDisplay);
static void Display_off(Display* self, CurrentDisplay currentDisplay);
static void Display_write(const uint8_t digit);
static void Display_update_digit(Display* self, const uint8_t temp);
static inline void write_MSB(const uint8_t digit);

/******************************************************************************
* Funktionen new_Display utg�r initieringsrutin f�r objekt av strukten Display.
* Ing�ende argument PIN utg�r aktuellt PIN-nummer sett till Arduino Uno
* (PIN 0 - 13), som �r ekvivalent med f�ljande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN p� Arduino Uno        *
*     8 - 13                    B            PIN p� Arduino Uno - 8           *
*******************************************************************************
* 
* F�r 7 segments displayer s� fungerar koden enbart om D1_PIN och D2_PIN 
* ligger mellan PIN 9-12 d� �vriga pins �r upptagna.
*
* Returnerar ett objekt av strukten Display som kan hantera visa ett heltal
* mellan 00 till 99 p� 7 segmentsdisplayer kopplade i bokstavsf�ljd fr�n
* PIN 2-8 och display 1 till angiven D1_pin och display 2 till D2_pin.
*
* Inparameter	: uint8_t Display 1 PIN & uint8_t Display 2 PIN.
* Returnerar	: initialized objekt of struct Display.
******************************************************************************/
Display new_Display(uint8_t D1_PIN, uint8_t D2_PIN)
{
	Display self;
	
	self.D1_PIN = Check_IO_port(D1_PIN);
	self.D2_PIN = Check_IO_port(D2_PIN);
	
	
	self.currentDigit = DIGIT1;
	
	self.on = Display_on;
	self.off = Display_off;
	self.update_digit = Display_update_digit;
	self.enabled = true;
	return self;
} // End of function new_Display.

/******************************************************************************
* Funktionen Check_IO_port kontrollerar vilken PIN som displayen ligger p�.
* Om PIN ligger p� PORTB, PIN 9-12 s� aktiveras korrekt PIN i DDRD
* annars s� returneras PIN utan att n�got data direction register ettst�lls.
*
* Inparameter	: uint8_t PIN
* Returnerar	: uint8_t PIN
******************************************************************************/
static uint8_t Check_IO_port(uint8_t PIN)
{	
	if (PIN >= 9 && PIN <= 12)
	{
		PIN = PIN - 8;
		DDRB |= (1 << PIN);
		return PIN;
	}
	else return PIN;
} // End of function Check_IO_port

/******************************************************************************
* Funktionen Display_on anv�nds f�r att t�nda en display. Ing�ende argument self
* utg�r en pekare till display-objektet i fr�ga och aktuell display. 
* och t�nder 
*
* Inparameter	: Object of struct Display, CurrentDisplay(DISPLAY1/DISPLAY2)
* Returnerar	: --
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
} // End of function Display_on

/******************************************************************************
* Funktionen Display_off anv�nds f�r att sl�cka en display. Ing�ende argument
* self utg�r en pekare till 7 segment displayen. Utefter aktuell I/O-port s� 
* nollst�lls motsvarande bit f�r port B.
*
* Inparameter	: Object of struct Display, CurrentDisplay(DISPLAY1/DISPLAY2)
* Returnerar	: --
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
} // End of function Display_off

/*********************************************************************************************
* Funktionen display_write anv�nds f�r att skriva ut en siffra 0 - 9 p� en 7-segmentsdisplay.
* Eftersom PIN 2 - 8 anv�nds f�r segmenten, samtidigt som alla makron �r gjorda f�r PIN 0 - 6,
* s� skiftas den bin�ra koden tv� steg �t v�nster via bitvis skiftning.
* Vid fel (om siffran �verstiger nio, s� sl�cks displayen).
*
* D� PIN 8 ligger p� port B s� anv�nds funktionen write_MSB f�r att t�nda segmentet kopplat
* till PIN 8 ifall MSB �r ett tal, annars s� st�ngs PIN 8 av.
*
* Typecasts till uint8_t anv�nds f�r att motverka varningsmeddelande f�r trunkering vid
* bitvis skiftning.
*
* Inparameter	: uint8_t digit
* Returnerar	: --
*********************************************************************************************/
static void Display_write(const uint8_t digit)
{
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
} // End of function Display_write.

/*********************************************************************************************
* Funktionen write_MSB l�ser av ing�ende tal som i detta program �r 7 bitars tal.
* MSB[6:0] l�ses av och lagras p� variabeln number. PIN 8 aktiveras ifall number �r ett tal, 
* annars avaktiveras PIN 8.
*
* Inparameter	: uint8_t digit.
* Returnerar	: --
*********************************************************************************************/
static inline void write_MSB(const uint8_t digit)
{
	const uint8_t number = digit & (1 << 6);
	
	if (number) 
		PORTB |= (1<<0);
	else 
		PORTB &= ~(1<<0);
	return;
} // End of function write_MSB

/*********************************************************************************************
* Funktionen Display_update_digit.
* Ing�ende argument: Display objekt, osignerat 8 bitars heltal.
* Ing�ende tal f�r temperatur bryts up i tv� ental som ska representera tiotalet och entalet.
*
* Beroende p� vilken display som har varit aktiv s� skiftas aktuell display och beroende p�
* vilken display s� skrivs f�rsta eller andra siffran ut till r�tt display.
*
* Denna funktion anropas via avbrott genererat av timer.
*
* Inparameter	: Object of struct Display, uint8_t temp
* Returnerar	: --
*********************************************************************************************/
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
} // End of function Display_update_digit