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
* Funktionen new_Vector anv�nds f�r att deklarera och initiera en ny dynamisk
* array, implementerad via ett objekt av strukten Vector. F�rst skapas ett
* nytt objekt av denna strukt, som d�ps till self, f�ljt av att objektets
* medlemmar initieras. Pekaren data s�tts till en nullpekare, d� denna inte
* har n�gon befintlig minnesadress att peka p� vid start (inget minne �r
* allokerat f�r arrayen). Instansvariabel elements s�tts till 0x00 vid start,
* d� arrayen �r tom. 
* D�refter s�tts pekarna till att peka p� motsvarande funktioner i filen.
* Slutligen returneras objektet, som nu �r f�rdigt att
* anv�ndas som en dynamisk array.
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
* Funktionen Vector_resize anv�nds f�r att kunna uppdatera storleken p� den
* dynamiska vectorn. Om den andra parametern �r 0 s� kommer vectorn tas bort
* via anrop p� funktionen Vector_clear. Annars s� reallokeras minne f�r till
* en tillf�llig kopia som f�r inneh�llet fr�n den befintliga vectorn som till
* sist skrivs tillbaka till vectorn som har �ndrat storlek.
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
* Funktionen Vector_push anv�nds f�r att l�gga till ett nytt element l�ngst
* bak i en array. F�rst skapas en kopia av befintligt f�lt, som d�pt till
* copy. Detta kopiaf�lt rymmer ytterligare ett element �n det gamla f�ltet.
* Om minnesallokeringen misslyckades (brist p� utrymme troligast) s� returneras
* NULL fr�n funktionen realloc. D� avslutar vi funktionen, men det gamla
* inneh�llet �r kvar, d� en kopia anv�ndes. Det nya elementet placeras l�ngst
* bak i det nya f�ltet (index elements) och antalet element r�knas upp.
* Datapekaren data s�tts sedan till att peka p� det nya f�lt som kopian
* copy pekar p�. Eftersom minnet f�r det gamla f�ltet frigjordes ifall
* omallokeringen lyckades s� kr�vs ingen deallokering.
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
* Funktionen Vector_clear anv�nds f�r att t�mma en dynamisk array och frig�ra
* minne. Deallokeringen sker via anrop av funktionen free, d�r v�r datapekare
* data, som pekar p� det allokerade f�ltet, passeras. Pekaren data s�tts
* till nullpekare, s� att denna inte pekar d�r det tidigare f�ltet l�g.
* Antalet element s�tts till 0 f�r att indikera att arrayen �r tom.
******************************************************************************/
static void Vector_clear(Vector* self)
{
	free(self->data);
	self->data = NULL;
	self->elements = 0x00;
	return;
}

/******************************************************************************
* Funktionen Vector_set anv�nds f�r att skriva �ver ett gammalt element.
* Ing�ende argument index utg�r index till det element som skall skrivas �ver,
* medan new_element utg�r det nya v�rdet. Om angivet index ligger utanf�r
* arrayen s� g�rs ingen skrivning. Annars l�ggs det nya elementet p� angivet
* index.
******************************************************************************/
static void Vector_set(Vector* self, const size_t index, const uint32_t new_element)
{
	if (index >= self->elements) return;
	self->data[index] = new_element;
	return;
}

/******************************************************************************
* Funktionen Vector_sum anv�nds f�r att ber�kna summan av samtliga befintliga
* element- i en array. Om arrayen �r tom s� returneras summan 0. Annars sker
* iteration genom arrayen fr�n f�rsta till sista element, d�r varje lagrat
* tal summeras via en lokal variabel sum, som returneras vid �terhoppet.
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
* Funktionen Vector_average anv�nds f�r att ber�kna medelv�rdet av samtliga
* lagrade tal. Om arrayen �r tom returneras medelv�rdet 0. Annars ber�knas
* summan av alla tal via anrop av funktionen Vector_sum. Medelv�rdet ber�knas
* sedan som summan dividerat p� antalet element, d�r flyttalsdivision anv�nds
* f�r att erh�lla ett korrekt v�rde (annars trunkeras eventuella decimaler).
******************************************************************************/
static double Vector_average(Vector* self)
{
	if (!self->elements) return 0;
	const uint32_t sum = Vector_sum(self);
	return sum/(double)self->elements;
}

/******************************************************************************
* Funktionen Vector_print anv�nds f�r att skriva ut befintligt inneh�ll i en
* dynamisk array. Om arrayen �r tom s� avslutas funktionen direkt. Annars
* initieras seriell �verf�ring via anrop av funktionen init_serial. Sedan
* skrivs antalet lagrade element i arrayen ut, f�ljt av att varje element
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