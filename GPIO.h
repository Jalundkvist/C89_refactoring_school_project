
#ifndef GPIO_H_
#define GPIO_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "Serial.h"

// Makrodefinitioner f�r 7 segmentsdisplay.
#define ZERO 0x40
#define ONE 0x79
#define TWO 0x24
#define THREE 0x30
#define FOUR 0x19
#define FIVE 0x12
#define SIX 0x02
#define SEVEN 0x78
#define EIGHT 0x00
#define NINE 0x10
#define A 0x08
#define B 0x03
#define C 0x46
#define D 0x21
#define E 0x06
#define F 0x0E

/******************************************************************************
* Strukten Display anv�nds f�r implementering av 7 segmentsdisplay, som b�r
* placeras p� n�gon av digitala PINs 8 - 13 (PORTB) p� Arduino Uno. 
* Varje display kan t�ndas, sl�ckas och togglas.
******************************************************************************/
typedef struct Display
{
	uint8_t D1_PIN;
	uint8_t D2_PIN;
	bool enabled;
	CurrentDigit currentDigit;
	CurrentDisplay currentDisplay;
	void (*on)(struct Display*, CurrentDisplay);
	void (*off)(struct Display*, CurrentDisplay);
	void (*enable)(struct Display*);
	void (*update_digit)(struct Display*, uint8_t temp);
}Display;

/******************************************************************************
* Strukten Button anv�nds f�r implementering av tryckknappar, som kan placeras 
* p� n�gon av digitala PINs 0 - 13 p� Arduino Uno. Det finns m�jlighet att
* enkelt l�sa av ifall tryckknappen �r nedtryckt. Det finns ocks� m�jlighet 
* att aktivera samt inaktivera PCI-avbrott p� tryckknappens PIN.
* 
* Avbrottsvektorer g�llande PCI-avbrott f�r respektive I/O-port �r f�ljande:
*
* I/O-port B (PIN 8 - 13):	PCINT0_vect
* I/O-port C (PIN A0 - A5): PCINT1_vect - anv�nds dock inte f�r tryckknappar.
* I/O-port D (PIN 0 - 7):	PCINT2_vect
******************************************************************************/
typedef struct Button
{
	unsigned char PIN;
	IO_port io_port;
	bool interrupt_enabled;
	bool (*is_pressed)(struct Button*);
	void (*enable_interrupt)(struct Button*);
	void (*disable_interrupt)(struct Button*);
} Button;

// Funktionsdeklarationer:
Display new_Display(const uint8_t D1_PIN, const uint8_t D2_PIN); 
Button new_Button(const uint8_t PIN);


#endif /* GPIO_H_ */