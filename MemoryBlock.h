#ifndef MEMORY_BLOCK_H_
#define MEMORY_BLOCK_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"
#include "Serial.h"

/*************************************************************************************
* Unionen MemoryBlock används för att kunna välja datatyp för en pekare till ett
* minnesblock på heapen, vars datatyp kan väljas mellan int, double size_t samt char,
* där samma minnesutrymme används oavsett datatyp.
*************************************************************************************/
union MemoryBlock
{
	int* integer; /* Pekare till fält för signerade heltal. */
	double* decimal; /* Pekare till fält för flyttal. */
	size_t* natural; /* Pekare till fält för osignerade heltal. */
	char* character; /* Pekare till fält med tecken. */
};

/* Externa funktioner: */
union MemoryBlock* new_MemoryBlock(void);
void MemoryBlock_delete(union MemoryBlock* self, enum DataType* dataType);
enum bool MemoryBlock_resize(union MemoryBlock* self, enum DataType* dataType, size_t* new_size);
void MemoryBlock_assign(union MemoryBlock* self, enum DataType* dataType, 
						size_t* index, void* data);
void MemoryBlock_set(union MemoryBlock* self, enum DataType* dataType, 
					 size_t* index, void* data);
void* MemoryBlock_at(union MemoryBlock* self, enum DataType* dataType, size_t* index);
void MemoryBlock_print(union MemoryBlock* self, enum DataType* dataType, size_t* size);
void MemoryBlock_insert(union MemoryBlock* self, enum DataType* dataType, size_t* index, 
						size_t* size, void* data);
void MemoryBlock_erase(union MemoryBlock* self, enum DataType* dataType, 
					   size_t* index, size_t* size);
void MemoryBlock_sort(union MemoryBlock* self, enum DataType* dataType, size_t* size);
void MemoryBlock_sort_reverse(union MemoryBlock* self, enum DataType* dataType, size_t* size);

#endif /* MEMORY_BLOCK_H_ */