
#ifndef GPIO_H_
#define GPIO_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
* Strukten Led anv�nds f�r implementering av lysdioder, som kan placeras p�
* n�gon av digitala PINs 0 - 13 p� Arduino Uno. 
* Varje lysdiod kan t�ndas, sl�ckas och togglas.
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
	unsigned char PIN; /* Aktuellt PIN-nummer. */
	IO_port io_port; /* Anv�nd I/O-port. */
	bool interrupt_enabled; /* Indikerar ifall PCI-avbrott �r aktiverat. */
} Button;

/* Funktionsdeklarationer: */
Led new_Led(uint8_t PIN); 

Button* new_Button(unsigned char* PIN); 
bool Button_is_pressed(struct Button* self);
void Button_enable_interrupt(struct Button* self);
void Button_disable_interrupt(struct Button* self);
void delay(unsigned short* delay_time);

#endif /* GPIO_H_ */