#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

//Sätter klockfrekvensen till 16 MHz. 
#define F_CPU 16000000UL  

/******************************************************************************
* Inkluderingsdirektiv:
* <avr/io.h> Bibliotek för implementering av I/O gällande AVR-enheter.
* <avr/interrupt.h> Bibliotek för avbrott.
* <stdio.h> Bibliotek för implementering av I/O i C.
* <stdlib.h> C:s standardbibliotek.
* <util/delay.h> Bibliotek för att generera fördröjning.
******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

// Headerfil för inkludering av union för enkel bithantering. (borttagen då filen är inaktuell och ligger lagrad i mapp.)
// #include "Byte.h" 

/******************************************************************************
* För att aktivera avbrott globalt så ettställs I-flaggan (Interrupt Flag)
* i statusregistret SREG, vilket åstadkommes via assemblerinstruktionen SEI
* (Set Interrupt Flag). För att vid behov inaktivera avbrott globalt, vilket
* exempelvis är nödvändigt vid skrivning till EEPROM-minnet, så nollställs
* I-biten via assemblerinstruktionen CLI (Clear Interrupt Flag).
******************************************************************************/
#define ENABLE_INTERRUPTS asm("SEI")
#define DISABLE_INTERRUPTS asm("CLI")

/******************************************************************************
* Bitmakron för bitoperation:
* SET_BIT - Ettställer bit i specifikt register.
* CLEAR_BIT - Nollställer bit i specifikt register.
* READ_BIT - Avläser bit i enskilt register.
* ASSIGN - Tilldelar nytt värde, ettställer specifik bit
* SET - Ettställer samtliga bitar i register.
* CLEAR - Nollställer samtliga bitar i register.
*
* Avkommenterade då de inte används.
******************************************************************************/
/*
#define SET_BIT(REGISTER, BIT) REGISTER |= (1 << BIT)
#define CLEAR_BIT(REGISTER, BIT) REGISTER &= ~(1 << BIT)
#define READ_BIT(REGISTER, BIT) (REGISTER & (1 << BIT))
#define ASSIGN(REGISTER, BIT) REGISTER = (1 << BIT) 
#define SET(REGISTER) REGISTER = 0xFF 
#define CLEAR(REGISTER) REGISTER = 0x00*/

// Typdefinitioner:
typedef enum { false, true } bool;
typedef enum { IO_PORTB, IO_PORTC, IO_PORTD } IO_port; 
typedef enum { TIMER0, TIMER1, TIMER2 } TimerSelection;
typedef enum { DIGIT1, DIGIT2 } CurrentDigit;
typedef enum { DISPLAY1, DISPLAY2} CurrentDisplay;
//typedef enum { INT, DOUBLE, SIZE_T, CHAR } DataType;  //(Val av datatyp för vektor. - ej aktuell)

#endif /* DEFINITIONS_H_ */