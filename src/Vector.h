#ifndef VECTOR_H_
#define VECTOR_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "Serial.h"

/******************************************************************************
* Strukten Vektor används för att lagra heltal av datatypen uint32_t i en
* dynamisk array. Arrayen allokeras på heapen, där medlemmen data pekar på
* första element, medan elements räknar antalet befintliga element (heltal).
******************************************************************************/
typedef struct Vector
{
	uint32_t* data;
	size_t elements;
	
	void (*resize)(struct Vector*, const size_t);
	void (*push)(struct Vector*,const uint32_t);
	void (*clear)(struct Vector*);
	void (*set)(struct Vector*, const size_t, const uint32_t);
	uint32_t (*sum)(struct Vector*);
	double (*average)(struct Vector*);
	void (*print)(struct Vector*);
	
} Vector;

// Funktionsdeklarationer:
Vector new_Vector(void);

#endif /* VECTOR_H_ */