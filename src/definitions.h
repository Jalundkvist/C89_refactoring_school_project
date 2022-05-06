#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

//S�tter klockfrekvensen till 16 MHz. 
#define F_CPU 16000000UL  

/******************************************************************************
* Inkluderingsdirektiv:
* <avr/io.h> Bibliotek f�r implementering av I/O g�llande AVR-enheter.
* <avr/interrupt.h> Bibliotek f�r avbrott.
* <stdio.h> Bibliotek f�r implementering av I/O i C.
* <stdlib.h> C:s standardbibliotek.
* <util/delay.h> Bibliotek f�r att generera f�rdr�jning.
******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

// Headerfil f�r inkludering av union f�r enkel bithantering. (borttagen d� filen �r inaktuell och ligger lagrad i mapp.)
// #include "Byte.h" 

/******************************************************************************
* F�r att aktivera avbrott globalt s� ettst�lls I-flaggan (Interrupt Flag)
* i statusregistret SREG, vilket �stadkommes via assemblerinstruktionen SEI
* (Set Interrupt Flag). F�r att vid behov inaktivera avbrott globalt, vilket
* exempelvis �r n�dv�ndigt vid skrivning till EEPROM-minnet, s� nollst�lls
* I-biten via assemblerinstruktionen CLI (Clear Interrupt Flag).
******************************************************************************/
#define ENABLE_INTERRUPTS asm("SEI")
#define DISABLE_INTERRUPTS asm("CLI")

/******************************************************************************
* Bitmakron f�r bitoperation:
* SET_BIT - Ettst�ller bit i specifikt register.
* CLEAR_BIT - Nollst�ller bit i specifikt register.
* READ_BIT - Avl�ser bit i enskilt register.
* ASSIGN - Tilldelar nytt v�rde, ettst�ller specifik bit
* SET - Ettst�ller samtliga bitar i register.
* CLEAR - Nollst�ller samtliga bitar i register.
*
* Avkommenterade d� de inte anv�nds.
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
//typedef enum { INT, DOUBLE, SIZE_T, CHAR } DataType;  //(Val av datatyp f�r vektor. - ej aktuell)

#endif /* DEFINITIONS_H_ */