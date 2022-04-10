
#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define F_CPU 160000000UL /* Sätter klockfrekvensen till 16 MHz. */

/* Inkluderingsdirektiv: */
#include <avr/io.h> /* Bibliotek för implementering av I/O gällande AVR-enheter. */
#include <avr/interrupt.h> /* Bibliotek för avbrott. */
#include <stdio.h> /* Bibliotek för implementering av I/O i C. */
#include <stdlib.h> /* C:s standardbibliotek. */
#include "Byte.h" /* Headerfil för inkludering av union för enkel bithantering. */

/******************************************************************************
* För att aktivera avbrott globalt så ettställs I-flaggan (Interrupt Flag)
* i statusregistret SREG, vilket åstadkommes via assemblerinstruktionen SEI
* (Set Interrupt Flag). För att vid behov inaktivera avbrott globalt, vilket
* exempelvis är nödvändigt vid skrivning till EEPROM-minnet, så nollställs
* I-biten via assemblerinstruktionen CLI (Clear Interrupt Flag).
******************************************************************************/
#define ENABLE_INTERRUPTS sei() /* Aktiverar avbrott globalt. */
#define DISABLE_INTERRUPTS cli() /* Inaktiverar avbrott globalt. */

/******************************************************************************
* I programmet används bitmakron för bitoperationer. Bitmakron var mycket 
* utbrett innan inline-funktioner togs i bruk med C99.
******************************************************************************/
#define SET_BIT(REGISTER, BIT) REGISTER |= (1 << BIT) /* Ettställer bit i specifikt register. */
#define CLEAR_BIT(REGISTER, BIT) REGISTER &= ~(1 << BIT) /* Nollställer bit i specifikt register. */
#define READ_BIT(REGISTER, BIT) (REGISTER &= (1 << BIT)) /* Avläser bit i enskilt register. */
#define ASSIGN(REGISTER, BIT) REGISTER = (1 << BIT) /* Tilldelar nytt värde, ettställer specifik bit */
#define SET(REGISTER) REGISTER = 0xFF /* Ettställer samtliga bitar i register. */
#define CLEAR(REGISTER) REGISTER = 0x00 /* Nollställer samtliga bitar i register. */

/* Typdefinitioner: */
enum bool { false, true }; /* Realiserar datatypen bool. */
enum IO_port { IO_PORTB, IO_PORTC, IO_PORTD }; /* Enumeration för I/O-portar. */
enum TimerSelection { TIMER0, TIMER1, TIMER2 }; /* Enumeration för timerkretsar. */
enum DataType { INT, DOUBLE, SIZE_T, CHAR }; /* Val av datatyp för vektor. */

#endif /* DEFINITIONS_H_ */