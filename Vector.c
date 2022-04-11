// Inkluderar egen headerfil:
#include "Vector.h"

// Statiska funktioner:
static void Vector_resize(Vector* self, const size_t new_size);
static void Vector_push(Vector* self,const uint32_t new_element);
static void Vector_clear(Vector* self);
static void Vector_set(Vector* self, const size_t index, const uint32_t new_element);
static uint32_t Vector_sum(Vector* self);
static double Vector_average(Vector* self);
static void Vector_print(Vector* self);

/******************************************************************************
* Funktionen new_Vector används för att deklarera och initiera en ny dynamisk
* array, implementerad via ett objekt av strukten Vector. Först skapas ett
* nytt objekt av denna strukt, som döps till self, följt av att objektets
* medlemmar initieras. Pekaren data sätts till en nullpekare, då denna inte
* har någon befintlig minnesadress att peka på vid start (inget minne är
* allokerat för arrayen). Instansvariabel elements sätts till 0x00 vid start,
* då arrayen är tom. 
* Därefter sätts pekarna till att peka på motsvarande funktioner i filen.
* Slutligen returneras objektet, som nu är färdigt att
* användas som en dynamisk array.
******************************************************************************/
Vector new_Vector(void)
{
	Vector self;
	self.data = NULL;
	self.elements = 0x00;
	self.resize = Vector_resize;
	self.push = Vector_push;
	self.clear = Vector_clear;
	self.set = Vector_set;
	self.sum = Vector_sum;
	self.average = Vector_average;
	self.print = Vector_print;
	return self;
}

/******************************************************************************
* Funktionen Vector_resize används för att kunna uppdatera storleken på den
* dynamiska vectorn. Om den andra parametern är 0 så kommer vectorn tas bort
* via anrop på funktionen Vector_clear. Annars så reallokeras minne för till
* en tillfällig kopia som får innehållet från den befintliga vectorn som till
* sist skrivs tillbaka till vectorn som har ändrat storlek.
******************************************************************************/
static void Vector_resize(Vector* self, const size_t new_size)
{
	if (!new_size)
	{
		Vector_clear(self);
		return;
	}
	uint32_t* copy = (uint32_t*)realloc(self->data, sizeof(uint32_t)* new_size);
	
	if(!copy) return;
	self->elements = new_size;
	self->data = copy;
	return;
}

/******************************************************************************
* Funktionen Vector_push används för att lägga till ett nytt element längst
* bak i en array. Först skapas en kopia av befintligt fält, som döpt till
* copy. Detta kopiafält rymmer ytterligare ett element än det gamla fältet.
* Om minnesallokeringen misslyckades (brist på utrymme troligast) så returneras
* NULL från funktionen realloc. Då avslutar vi funktionen, men det gamla
* innehållet är kvar, då en kopia användes. Det nya elementet placeras längst
* bak i det nya fältet (index elements) och antalet element räknas upp.
* Datapekaren data sätts sedan till att peka på det nya fält som kopian
* copy pekar på. Eftersom minnet för det gamla fältet frigjordes ifall
* omallokeringen lyckades så krävs ingen deallokering.
******************************************************************************/
static void Vector_push(Vector* self,const uint32_t new_element)
{
	uint32_t* copy = (uint32_t*)realloc(self->data, sizeof(uint32_t)*(self->elements+1));
	if (!copy) return;
	copy[self->elements++] = new_element;
	self->data = copy;
	return;
}

/******************************************************************************
* Funktionen Vector_clear används för att tömma en dynamisk array och frigöra
* minne. Deallokeringen sker via anrop av funktionen free, där vår datapekare
* data, som pekar på det allokerade fältet, passeras. Pekaren data sätts
* till nullpekare, så att denna inte pekar där det tidigare fältet låg.
* Antalet element sätts till 0 för att indikera att arrayen är tom.
******************************************************************************/
static void Vector_clear(Vector* self)
{
	free(self->data);
	self->data = NULL;
	self->elements = 0x00;
	return;
}

/******************************************************************************
* Funktionen Vector_set används för att skriva över ett gammalt element.
* Ingående argument index utgör index till det element som skall skrivas över,
* medan new_element utgör det nya värdet. Om angivet index ligger utanför
* arrayen så görs ingen skrivning. Annars läggs det nya elementet på angivet
* index.
******************************************************************************/
static void Vector_set(Vector* self, const size_t index, const uint32_t new_element)
{
	if (index >= self->elements) return;
	self->data[index] = new_element;
	return;
}

/******************************************************************************
* Funktionen Vector_sum används för att beräkna summan av samtliga befintliga
* element- i en array. Om arrayen är tom så returneras summan 0. Annars sker
* iteration genom arrayen från första till sista element, där varje lagrat
* tal summeras via en lokal variabel sum, som returneras vid återhoppet.
******************************************************************************/
static uint32_t Vector_sum(Vector* self)
{
	if (!self->elements) return 0;
	uint32_t sum = 0x00;
	for (size_t i = 0; i < self->elements; i++)
	sum += self->data[i];
	return sum;
}

/******************************************************************************
* Funktionen Vector_average används för att beräkna medelvärdet av samtliga
* lagrade tal. Om arrayen är tom returneras medelvärdet 0. Annars beräknas
* summan av alla tal via anrop av funktionen Vector_sum. Medelvärdet beräknas
* sedan som summan dividerat på antalet element, där flyttalsdivision används
* för att erhålla ett korrekt värde (annars trunkeras eventuella decimaler).
******************************************************************************/
static double Vector_average(Vector* self)
{
	if (!self->elements) return 0;
	const uint32_t sum = Vector_sum(self);
	return sum/(double)self->elements;
}

/******************************************************************************
* Funktionen Vector_print används för att skriva ut befintligt innehåll i en
* dynamisk array. Om arrayen är tom så avslutas funktionen direkt. Annars
* initieras seriell överföring via anrop av funktionen init_serial. Sedan
* skrivs antalet lagrade element i arrayen ut, följt av att varje element
* skrivs ut var sin rad en efter en.
******************************************************************************/
static void Vector_print(Vector* self)
{
	if (!self->elements) return;
	init_serial();
	
	serial_print_unsigned("Number of elements: %lu\n", self->elements);
	serial_print_unsigned("Sum of all elements: %lu\n", Vector_sum(self));
	serial_print_integer("Rounded average: %ld\n", (int32_t)(Vector_average(self) + 0.5));
	
	serial_print("-------------------------------------------------------------------------------\n");
	for(register size_t i = 0; i < self->elements; i++)
	serial_print_unsigned("%lu\n", self->data[i]);
	serial_print("-------------------------------------------------------------------------------\n\n");
	return;
}