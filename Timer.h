#ifndef TIMER_H_
#define TIMER_H_

// Inkluderingsdirektiv:
#include "definitions.h"

/******************************************************************************
* Vid initiering sätts timerkretsar Timer 0 - 2 till att räkna upp utan
* prescaler, vilket innebär en uppräkningsfrekvens på 16 MHz, som motsvarar en
* uppräkningshastighet på 62.5 us. Därmed tar det 256 * 62.5u = 0.016 ms mellan
* varje timergenererat avbrott för en given timer när denna är aktiverad, då
* samtliga timerkretsar sätts till att räkna upp till 256, då avbrott sker och
* timerkretsen nollställs (automatiskt för Timer 0 samt Timer 2, då overflow
* sker för dessa 8-bitars timerkretsar, medan Timer 1 i stället nollställs av 
* hårdvaran i CTC Mode, då denna uppgår till 16-bitar och därmed är långt från
* full vid uppräkning till 256).
*
* För att initiera timerkrets Timer 0 utan prescaler i Normal Mode så ettställs
* biten CS00 (Clock Select 0 bit 0) i kontrollregistret TCCR0B (Timer/Counter
* Control Register 0B). Därmed sätts uppräkningsfrekvensen till 16 MHz. 
*
* För att initiera timerkrets Timer 1 utan prescaler i CTC Mode, så att avbrott 
* sker vid ett lägre värde än när overflow sker, vilket annars sker vid 
* uppräkning till 65 536 för en 16-bitars räknare, så ettställs bitar CS10 
* (Clock Select 1 bit 0) samt WGM12 (Waveform Generator Mode 2 bit 2) i 
* kontrollregistret TCCR1B (Timer/Counter Control Register 1B). CS10 används 
* för att ställa in uppräkningsfrekvensen utan prescaler (16 MHz), medan WGM12 
* används för att Timer 1 skall arbeta i CTC Mode i stället för Normal Mode.
* För att ställa in att timergenererat avbrott skall ske på Timer 1 vid 
* uppräkning till 256, såsom övriga timerkretsar, så skrivs detta heltal till
* registret OCR1A (Output Compare Register 1A). Vid uppräkning till 256 sker
* då ett CTC-avbrott och Timer 1 nollställs av hårdvaran. Detta måste göras
* vid initiering av Timer 1.
*
* För att initiera timerkrets Timer 2 utan prescaler i Normal Mode så ettställs
* biten CS20 (Clock Select 2 bit 0) i kontrollregistret TCCR2B (Timer/Counter
* Control Register 2B). Därmed sätts uppräkningsfrekvensen till 16 MHz, precis
* som för Timer 0. Dessa register uppgår båda till 8-bitar, vilket alltså
* medför overflow vid uppräkning till 256.
******************************************************************************/
#define INIT_TIMER0 TCCR0B = (1<<CS00)
#define INIT_TIMER1 TCCR1B = (1<<CS10)|(1<<WGM12)
#define INIT_TIMER2 TCCR2B = (1<<CS20)

#define SET_TIMER1_LIMIT OCR1A = 256
#define INTERRUPT_TIME 0.016f

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
	unsigned long executed_interrupts;
	unsigned long required_interrupts;
} Timer;

/* Funktionsdeklarationer: */
Timer* new_Timer(TimerSelection* timerSelection, float* delay_time);
void Timer_on(struct Timer* self);
void Timer_off(struct Timer* self);
void Timer_toggle(struct Timer* self);
void Timer_count(struct Timer* self);
bool Timer_elapsed(struct Timer* self);
void Timer_clear(struct Timer* self);
void Timer_reset(struct Timer* self);
void Timer_set(struct Timer* self, float* delay_time);
void init_timer(TimerSelection* timerSelection);
unsigned long get_required_interrupts(float* delay_time);

#endif /* TIMER_H_ */