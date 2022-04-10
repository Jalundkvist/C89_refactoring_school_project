
#ifndef GPIO_H_
#define GPIO_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
* Strukten Led används för implementering av lysdioder, som kan placeras på
* någon av digitala PINs 0 - 13 på Arduino Uno. Varje lysdiod kan tändas, 
* släckas och togglas.
******************************************************************************/
struct Led
{
	unsigned char PIN; /* Aktuellt PIN-nummer. */
	enum bool enabled; /* Indikerar ifall lysdioden är på eller inte. */
	enum IO_port io_port; /* I/O-port som lysdioden är ansluten till. */
};

/******************************************************************************
* Strukten Button används för implementering av tryckknappar, som kan placeras 
* på någon av digitala PINs 0 - 13 på Arduino Uno. Det finns möjlighet att
* enkelt läsa av ifall tryckknappen är nedtryckt. Det finns också möjlighet 
* att aktivera samt inaktivera PCI-avbrott på tryckknappens PIN.
* 
* Avbrottsvektorer gällande PCI-avbrott för respektive I/O-port är följande:
*
* I/O-port B (PIN 8 - 13): PCINT0_vect
* I/O-port C (PIN A0 - A5): PCINT1_vect - används dock inte för tryckknappar.
* I/O-port D (PIN 0 - 7): PCINT2_vect
******************************************************************************/
struct Button
{
	unsigned char PIN; /* Aktuellt PIN-nummer. */
	enum IO_port io_port; /* Använd I/O-port. */
	enum bool interrupt_enabled; /* Indikerar ifall PCI-avbrott är aktiverat. */
};

/* Funktionsdeklarationer: */
struct Led* new_Led(unsigned char* PIN); 
void Led_on(struct Led* self);
void Led_off(struct Led* self);
void Led_toggle(struct Led* self);
void Led_blink(struct Led* self, unsigned short* delay_time);
struct Button* new_Button(unsigned char* PIN); 
enum bool Button_is_pressed(struct Button* self);
void Button_enable_interrupt(struct Button* self);
void Button_disable_interrupt(struct Button* self);
void delay(unsigned short* delay_time);

#endif /* GPIO_H_ */