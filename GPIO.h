
#ifndef GPIO_H_
#define GPIO_H_

// Inkluderingsdirektiv:
#include "definitions.h"

/******************************************************************************
* Strukten Display anv�nds f�r implementering av lysdioder, som kan placeras p�
* n�gon av digitala PINs 0 - 13 p� Arduino Uno. 
* Varje lysdiod kan t�ndas, sl�ckas och togglas.
******************************************************************************/
typedef struct Display
{
	uint8_t PIN;
	bool enabled;
	IO_port io_port;
	void (*on)(struct Display*);
	void (*off)(struct Display*);
	void (*toggle)(struct Display*);
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
Display new_Display(uint8_t PIN); 
Button new_Button(uint8_t PIN); 

#endif /* GPIO_H_ */