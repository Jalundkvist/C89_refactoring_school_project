#ifndef DYNAMICTIMER_H_
#define DYNAMICTIMER_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "Timer.h"
#include "Vector.h"
#include "Serial.h"

// Makro för begränsning av capacity:
#define MAX_CAPACITY 100

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