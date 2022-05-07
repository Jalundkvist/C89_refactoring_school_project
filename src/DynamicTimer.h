#ifndef DYNAMICTIMER_H_
#define DYNAMICTIMER_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "Timer.h"
#include "Vector.h"
#include "Serial.h"

// Makro f�r begr�nsning av capacity:
#define MAX_CAPACITY 100

/******************************************************************************
* Strukten DynamicTimer anv�nds f�r att skriva ut temperaturen med hj�lp av
* en genomsnittlig tid som tas fram genom att r�kna antal avbrott mellan
* knapptryckningar. N�r vektorn �r full s� byter den ut �ldst v�rde och 
* varje knapptryck uppdaterar objektet "timer"s variabel "required_interrupts"
* Som i f�ljd �ndrar hur ofta temperaturen m�ts/skrivs ut till 7segments
* displayer eller seriell monitor.
* 
* Objektet DynamicTimer inneh�ller wrapperfunktioner som anv�nder sig av 
* funktioner som �terfinns i andra objekt.
* Den dynamiska timern anv�nder sig av tv� objekt utanf�r denna fil.
* Den f�rsta �r ett objekt fr�n strukten Timer d�pt timer och den andra fr�n
* strukten Vector d�pt interrupt_vector.
******************************************************************************/
typedef struct DynamicTimer
{
	Timer timer;
	Vector interrupt_vector;
	uint32_t interrupt_counter;
	size_t capacity;
	size_t next;
	bool initiated;
	
	void (*on)(struct DynamicTimer*);
	void (*off)(struct DynamicTimer*);
	void (*toggle)(struct DynamicTimer*);
	void (*count)(struct DynamicTimer*);
	bool (*elapsed)(struct DynamicTimer*);
	void (*clear)(struct DynamicTimer*);
	void (*update)(struct DynamicTimer*);
	void (*set_capacity)(struct DynamicTimer*, const size_t new_capacity);
	void (*print)(struct DynamicTimer*);
} DynamicTimer;

// Externa funktioner:
DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity);

#endif /* DYNAMICTIMER_H_ */