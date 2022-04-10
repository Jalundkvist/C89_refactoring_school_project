
#ifndef SERIAL_H_
#define SERIAL_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
* För att aktivera seriell transmission så ettställs biten TXCIE0 (Transfer 
* Channel Interrupt Enable 0) i kontrollregistret UCSR0B (USART Control and 
* Status Register 0B). 
* 
* För att sätta bithastigheten / Baud Rate för seriell överföring till 115 220 
* kbps (kilobits per second), så skrivs talet 7.67 till registret UBBR0 
* (USART Baud Rate Register 0) enligt formeln
*
* UBRR0 = F_CPU / (16 * Baud Rate) - 1 = 16M / (16 * 115 220) - 1 = 8.67 - 1,
* vilket avrundas till 7.67,
*
* där F_CPU är mikrodatorns klockfrekvens och Baud Rate är önskad bithastighet.
*
* För att vänta tills eventuellt föregående tecken har transmitterats, så
* implementeras en while-sats, som exekverar så länge dataregistret UDR0
* (USART Data Register 0) inte är tomt, vilket undersöks genom att kontrollera
* biten UDRE0 (USART Data Register Empty 0) i kontroll- och statusregistret
* UCSR0A (USART Control and Status Register A), som är nollställd så länge
* dataregister UDR0 inte är tomt.
*
* För att transmittera ett vagsnreturtecken \r, vilket genomförs för att nästa
* transmitterade tecken skall hamna längst till vänster på aktuell rad, så
* anropas funktionen write_byte, där tecknet \r sätts till ingående argument.
* För att transmittera ett nolltecken \0, vilket genomförs för att indikera
* när ett givet textstycke är slut, så anropas funktionen write_byte, där
* tecknet \0 sätts till ingående argument.
*
* Vid behov av att deklarera en sträng, vilket sker då text skall sammansättas
* med ett heltal, så används makrot SIZE för att sätta strängens kapacitet
* till 5 tecken (inklusive nolltecken).
******************************************************************************/
#define ENABLE_SERIAL_TRANSMISSION SET_BIT(UCSR0B, TXCIE0)
#define SET_BAUD_RATE_TO_115220_KBPS UBRR0 = 7.67f
#define WAIT_FOR_PREVIOUS_TRANSMISSION_TO_FINISH while (READ_BIT(UCSR0A, UDRE0)) ;
#define CARRIAGE_RETURN char r = '\r'; write_byte(&r)
#define END_TRANSMISSION char e = '\0'; write_byte(&e)
#define SIZE 5

/* Funktionsdeklarationer: */
void init_serial(void);
void serial_print(char* s);
void serial_print_integer(char* s, long* number);
void serial_print_unsigned(char* s, unsigned long* number);
void write_byte(char* data);

#endif /* SERIAL_H_ */