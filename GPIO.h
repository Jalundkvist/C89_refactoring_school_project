
#ifndef GPIO_H_
#define GPIO_H_

// Inkluderingsdirektiv:
#include "definitions.h"

/******************************************************************************
* Strukten Display används för implementering av lysdioder, som kan placeras på
* någon av digitala PINs 0 - 13 på Arduino Uno. 
* Varje lysdiod kan tändas, släckas och togglas.
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
* Strukten Button används för implementering av tryckknappar, som kan placeras 
* på någon av digitala PINs 0 - 13 på Arduino Uno. Det finns möjlighet att
* enkelt läsa av ifall tryckknappen är nedtryckt. Det finns också möjlighet 
* att aktivera samt inaktivera PCI-avbrott på tryckknappens PIN.
* 
* Avbrottsvektorer gällande PCI-avbrott för respektive I/O-port är följande:
*
* I/O-port B (PIN 8 - 13):	PCINT0_vect
* I/O-port C (PIN A0 - A5): PCINT1_vect - används dock inte för tryckknappar.
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