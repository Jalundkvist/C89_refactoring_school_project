// Inkludering av egen headerfil:
#include "DynamicTimer.h"

// Funktionsdeklarationer:
static void DynamicTimer_on(DynamicTimer* self);
static void DynamicTimer_off(DynamicTimer* self);
static void DynamicTimer_toggle(DynamicTimer* self);
static void DynamicTimer_count(DynamicTimer* self);
static bool DynamicTimer_elapsed(DynamicTimer* self);
static void DynamicTimer_clear(DynamicTimer* self);
static void DynamicTimer_update(DynamicTimer* self);
static void DynamicTimer_set_capacity(DynamicTimer* self, const size_t new_capacity);
static void DynamicTimer_print(DynamicTimer* self);
static inline size_t check_capacity(const size_t capacity);

/******************************************************************************
* Funktionen new_Dynamictimer skapar och initierar ett objekt av strukten
* DynamicTimer.
*
* Inparameter:	TimerSelection (TIMER0, TIMER1, TIMER2) & capacity, 
*				antal element som går att lagra.
*
* returnerar:	Initierat objekt av strukten DynamicTimer vilket även 
*				innefattar en timer och vector av respektive struct.
******************************************************************************/
DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity)
{
	DynamicTimer self;
	self.timer = new_Timer(timerSelection, 0x00);
	self.interrupt_vector = new_Vector();
	self.interrupt_counter = 0x00;
	self.capacity = check_capacity(capacity);
	self.next = 0x00;
	self.initiated = false;
	
	self.on = DynamicTimer_on;
	self.off = DynamicTimer_off;
	self.toggle = DynamicTimer_toggle;
	self.count = DynamicTimer_count;
	self.elapsed = DynamicTimer_elapsed;
	self.clear = DynamicTimer_clear;
	self.update = DynamicTimer_update;
	self.set_capacity = DynamicTimer_set_capacity;
	self.print = DynamicTimer_print;
	
	return self;
} // End of constructor.

/*********************************************************************************************
* Funktionen DynamicTimer_on sätter på den dynamiska timern via anrop på timerns on funktion
* ur Timer.c
*
* Inparameter	: Objektet.
* returnerar	: --
*********************************************************************************************/
static void DynamicTimer_on(DynamicTimer* self)
{
	self->timer.on(&self->timer);
	
	return;
} // End of function on

/*********************************************************************************************
* Funktionen DynamicTimer_off stänger av den dynamiskatimern via anrop på timerns off funktion
* ur Timer.c
*
* Inparameter	: Objektet.
* returnerar	: --
*********************************************************************************************/
static void DynamicTimer_off(DynamicTimer* self)
{
	self->timer.off(&self->timer);
	
	return;
} // End of function off

/*********************************************************************************************
* Funktionen DynamicTimer_toggle togglar aktuellt läge på den dynamiska timern via anrop på
* timerns funktion toggle ur Timer.c
*
* Inparameter	: Objektet.
* returnerar	: --
*********************************************************************************************/
static void DynamicTimer_toggle(DynamicTimer* self)
{
	self->timer.toggle(&self->timer);
	
	return;
} // End of function toggle

/*********************************************************************************************
* Funktionen DynamicTimer_count används för att timergenererade avbrott om den dynamiska
* timern är enabled.
* Räknar exekverade avbrott samt antal avbrott sen föregående knapptryck.
*
* Inparameter	: Objektet.
* returnerar	: --
*********************************************************************************************/
static void DynamicTimer_count(DynamicTimer* self)
{
	if (self->timer.enabled)
	{
		self->timer.executed_interrupts++;
		self->interrupt_counter++;
	}
	
	return;
} // End of function count

/*********************************************************************************************
* Funktionen DynamicTimer_elapsed indikerar returnerar true om timern har löpt ut, annars
* returneras false.
*
* Inparameter	: Objektet.
* returnerar	: Bool(true/false).
*********************************************************************************************/
static bool DynamicTimer_elapsed(DynamicTimer* self)
{
	if (!self->timer.required_interrupts)
	{
		return false;
	}
	
	return self->timer.elapsed(&self->timer);
} // End of function elapsed

/*********************************************************************************************
* Funktionen DynamicTimer_clear används för att nollställa den dynamiska timern och frigöra
* eventuellt allokerat minne.
*
* Inparameter	: Objektet.
* returnerar	: --
*********************************************************************************************/
static void DynamicTimer_clear(DynamicTimer* self)
{
	self->timer.off(&self->timer);
	self->timer.clear(&self->timer);
	self->interrupt_vector.clear(&self->interrupt_vector);
	self->interrupt_counter = 0x00;
	self->next = 0x00;
	self->initiated = false;
	
	return;
} // End of function clear

/*********************************************************************************************
* Funktionen DynamicTimer_update används för att uppdatera den dynamiska timern.
* Om timern inte är aktiverad så initieras den följt av ett meddelande att den är initierad.
* 
* Timern börjar räkna efter den är initierad. För att få timern att börja uppdatera
* avläsning av temperaturen så måste tiden MELLAN knapptryck lagras i interrupt_vector.
* Så det krävs minimum av två knapptryck.
*
* Inparameter	: Objektet.
* returnerar	: --
*********************************************************************************************/
static void DynamicTimer_update(DynamicTimer* self)
{
	if (!self->initiated)
	{
		self->interrupt_counter = 0x00;
		self->initiated = true;
		serial_print("--------------------------------------------------------\n");
		serial_print("Dynamic timer initiated!\n");
		serial_print("--------------------------------------------------------\n");
		return;
	}
	if (self->interrupt_vector.elements < self->capacity)
	{
		self->interrupt_vector.push(&self->interrupt_vector, self->interrupt_counter);
	}
	else
	{
		self->interrupt_vector.set(&self->interrupt_vector, self->next, self->interrupt_counter);
	}
	
	self->interrupt_counter = 0x00;
	if (++self->next >= self->capacity)
	{
		self->next = 0x00;
	}
	self->timer.required_interrupts = self->interrupt_vector.average(&self->interrupt_vector);
	serial_print("--------------------------------------------------------\n");
	serial_print("Dynamic timer updated!\n");
	self->print(self);
	
	return;
} // End of function update

/*********************************************************************************************
* Funktionen DynamicTimer_set_capacity används för att ställa in / ändra kapaciteten på den
* dynamiska timern. Den nya kapaciteten jämförs om den är större eller lika med nuvarande
* kapacitet. Det nya värdet kontrolleras då med inline funktionen check_capacity för att
* säkerställa att värdet inte överstiger MAX_CAPACITY (se DynamicTimer.h).
*
* Om new_capacity är mindre så kontrolleras vilken halva av vektorn som inntehåller nyaste
* värden. De nyaste värderna plaseras längst fram i vektorn som sedan ändrar storlek.
*
* Inparameter	: Objektet, new_capacity
* returnerar	: --
*********************************************************************************************/
static void DynamicTimer_set_capacity(DynamicTimer* self, const size_t new_capacity)
{
	if (!new_capacity) return;
	else if(new_capacity >= self->capacity)
	{
		serial_print_unsigned("Vector capacity resized from %lu ", self->capacity);
		self->capacity = check_capacity(new_capacity);
		serial_print_unsigned("to new value %lu\n\n", self->capacity);
		return;
	}
		
	const size_t last = self->interrupt_vector.elements -1;
		
	if (self->next >= self->interrupt_vector.elements/2)
	{
		for (register size_t i = 0; i < self->interrupt_vector.elements / 2; i++)
		{
			self->interrupt_vector.data[i] = self->interrupt_vector.data[last - i];
		}
	}
		
	serial_print_unsigned("Vector capacity resized from %lu ", self->capacity);
	self->interrupt_vector.resize(&self->interrupt_vector, new_capacity);
	self->capacity = new_capacity;
	serial_print_unsigned("to new value %lu\n\n", self->capacity);
	
	return;
} // End of function set_capacity

/******************************************************************************
* Funktionen DynamicTimer_print används för att skriva ut information om en
* dynamisk timer, till exempel, aktuell fördröjningstid, antal lagrade element
* samt summan och genomsnittet av dessa.
*
* Inparameter	: Objektet.
* returnerar	: --
******************************************************************************/
static void DynamicTimer_print(DynamicTimer* self)
{
	serial_print("--------------------------------------------------------\n");
	serial_print_unsigned("Vector capacity: %lu\n", self->capacity);
	serial_print_unsigned("Number of elements: %lu\n", self->interrupt_vector.elements);
	serial_print_unsigned("Next element in vector: %lu\n", self->next);
	serial_print_unsigned("Sum of stored elements: %lu\n", self->interrupt_vector.sum(&self->interrupt_vector));
	serial_print_unsigned("Average of values from vector: %lu\n", 
		(uint32_t)(self->interrupt_vector.average(&self->interrupt_vector)+0.5));
	serial_print_unsigned("Delay time: %lu ms\n", self->timer.required_interrupts * INTERRUPT_TIME);
	serial_print("--------------------------------------------------------\n");
	
	return;
} // End of function print

/******************************************************************************
* Funktionen check_capacity är en inline funktion som kontrollerar ingående
* parameter mot makron MAX_CAPACITY. om värdet överstiger så returneras 
* max värdet istället som en säkerhetsåtgärd.
*
* Inparameter	: size_t capacity
* returnerar	: size_t capacity
******************************************************************************/
static inline size_t check_capacity(const size_t capacity)
{
	if (capacity > MAX_CAPACITY)
	{
		serial_print_unsigned("Dynamic Timer capacity declared as larger or equal to maximum capacity, capacity set to: %lu\n", MAX_CAPACITY);
		return MAX_CAPACITY;
	}
	
	return capacity;
} // End of function check_capacity