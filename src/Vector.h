#ifndef VECTOR_H_
#define VECTOR_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "Serial.h"

/******************************************************************************
* Strukten Vektor anv�nds f�r att lagra heltal av datatypen uint32_t i en
* dynamisk array. Arrayen allokeras p� heapen, d�r medlemmen data pekar p�
* f�rsta element, medan elements r�knar antalet befintliga element (heltal).
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