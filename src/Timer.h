#ifndef TIMER_H_
#define TIMER_H_

// Inkluderingsdirektiv:
#include "definitions.h"

/******************************************************************************
* Makron för initiering av timerkretsat 0-2 med en prescaler på 64. 
* Timer 1 som är en 16 bitars timer begränsas via OCR1A registret till att ha
* samma uppräkning som timer 0 & 2.
* Detta medför att avbrott genereras var 1.024 ms. 
* Enligt följande uträkning: ((16M/64)^-1 * 256).
******************************************************************************/
#define INIT_TIMER0 TCCR0B = (1<<CS01)|(1<<CS00)
#define INIT_TIMER1 TCCR1B = (1<<CS11)|(1<<CS10)|(1<<WGM12)
#define INIT_TIMER2 TCCR2B = (1<<CS22)

#define SET_TIMER1_LIMIT OCR1A = 256
#define INTERRUPT_TIME 1.024f

/******************************************************************************
* För att aktivera Timer 0 i Normal Mode ettställs biten TOIE0 (Timer/Counter
* Overflow Interrupt Enable 0) i maskregistret TIMSK0 (Timer/Counter Mask
* Register 0). För att inaktivera avbrott nollställs i stället detta register.
* Avbrottsvektor för Timer 0 i Normal Mode är TIMER0_OVF_vect.
* 
* För att aktivera Timer 1 i CTC Mode så ettställs biten OCIE1A (Output Compare
* Interrupt Enable 1A) i maskregistret TIMSK1 (Timer/Counter Mask Register 1). 
* För att inaktivera avbrott nollställs i stället detta register.
* Avbrottsvektor för Timer 1 i CTC Mode är TIMER1_COMPA_vect.
* 
* För att aktivera Timer 2 i Normal Mode ettställs biten TOIE2 (Timer/Counter
* Overflow Interrupt Enable 2) i maskregistret TIMSK2 (Timer/Counter Mask
* Register 2). För att inaktivera avbrott nollställs i stället detta register.
* Avbrottsvektor för Timer 2 i Normal Mode är TIMER2_OVF_vect.
******************************************************************************/
#define ENABLE_TIMER0 TIMSK0 = (1 << TOIE0)
#define ENABLE_TIMER1 TIMSK1 = (1 << OCIE1A)
#define ENABLE_TIMER2 TIMSK2 = (1 << TOIE2)

#define DISABLE_TIMER0 TIMSK0 = 0x00
#define DISABLE_TIMER1 TIMSK1 = 0x00
#define DISABLE_TIMER2 TIMSK2 = 0x00

/******************************************************************************
* Strukten Timer används för att implementera mikrodatorns timerkretsar via
* timerobjekt. Mikrodatorns tre timerkretsar Timer 0 - 2 kan användas med
* valbar fördröjningstid. Samtliga timerkretsar räknar upp till 256 innan
* timergenererat avbrott sker, vilket medför avbrott var 1.024:e millisekund 
* för en given timer när denna är aktiverad (frekvensen sätts via respektive
* timers prescaler-bitar vid initieringen).
*
* Avbrottsvektorer för respektive timerkrets är följande:
*
* Timer 0: TIMER0_OVF_vect - Normal Mode.
* Timer 1: TIMER1_COMPA_vect - CTC Mode, maxvärde för uppräkning satt till 256.
* Timer 2: TIMER2_OVF_vect - Normal Mode.
******************************************************************************/
typedef struct Timer
{
	bool enabled;
	TimerSelection timerSelection;
	uint32_t executed_interrupts;
	uint32_t required_interrupts;
	
	void (*on)(struct Timer*);
	void (*off)(struct Timer*);
	void (*toggle)(struct Timer*);
	void (*count)(struct Timer*);
	bool (*elapsed)(struct Timer*);
	void (*clear)(struct Timer*);
	void (*reset)(struct Timer*);
	void (*set)(struct Timer*, const uint32_t);
	
} Timer;

// Funktionsdeklarationer:
Timer new_Timer(const TimerSelection timerSelection, const uint32_t delay_time);


#endif /* TIMER_H_ */