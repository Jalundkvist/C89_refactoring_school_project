
#ifndef GPIO_H_
#define GPIO_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
* Strukten Led används för implementering av lysdioder, som kan placeras på
* någon av digitala PINs 0 - 13 på Arduino Uno. 
* Varje lysdiod kan tändas, släckas och togglas.
******************************************************************************/
typedef struct Led
{
	uint8_t PIN;
	bool enabled;
	IO_port io_port;
	void (*on)(struct Led* self);
	void (*off)(struct Led* self);
	void (*toggle)(struct Led* self);
}Led;

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
	unsigned char PIN; /* Aktuellt PIN-nummer. */
	IO_port io_port; /* Använd I/O-port. */
	bool interrupt_enabled; /* Indikerar ifall PCI-avbrott är aktiverat. */
} Button;

/* Funktionsdeklarationer: */
Led new_Led(uint8_t PIN); 

Button* new_Button(unsigned char* PIN); 
bool Button_is_pressed(struct Button* self);
void Button_enable_interrupt(struct Button* self);
void Button_disable_interrupt(struct Button* self);
void delay(unsigned short* delay_time);

#endif /* GPIO_H_ */