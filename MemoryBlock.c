/* Inkluderingsdirektiv: */
#include "MemoryBlock.h"

/*************************************************************************************
* Funktionen new_MemoryBlock används för att deklarera och initiera ett nytt objekt 
* av unionen MemoryBlock, som utgörs av ett flertal pekare till ett dynamiskt fält, 
* där samma minnesutrymme används oavsett vilken datatyp som används. Via pekare 
* integer, decimal, natural samt character så kan samma minnesutrymme användas för 
* att lagra signerade eller osignerade heltal, flyttal eller tecken av någon av 
* datatyper int, double, size_t eller char. Dock kan enbart en av pekarna användas 
* vid ett givet tillfälle.
*
* Först allokeras minne till ett objekt av unionen MemoryBlock, som döps till self.
* Ifall minnesallokeringen misslyckas så returneras NULL direkt. Annars initieras
* respektive pekare till att peka på NULL, vilket betyder att de för tillfället inte 
* pekar på någon specifik minnesadress. Slutligen returneras objektet, som nu är 
* redo att användas.
*************************************************************************************/
union MemoryBlock* new_MemoryBlock(void)
{
	union MemoryBlock* self = (union MemoryBlock*)malloc(sizeof(union MemoryBlock));
	if (!self) return NULL;
	(*self).integer = NULL;
	(*self).decimal = NULL;
	(*self).natural = NULL;
	(*self).character = NULL;
	return self;
}

/*************************************************************************************
* Funktionen MemoryBlock_delete används för att frigöra minne för ett objekt av
* unionen MemoryBlock, vilket i innebär deallokering av ett dynamiskt fält.
* Ingående argument dataType indikerar objektets aktuella datatyp, som avgör vilken
* av objektets pekare som pekar på aktuellt fält. Datatypen undersöks, följt av att
* aktuellt fält frigörs via anrop av funktionen free. Slutligen sätts objektets
* samtliga pekare till att peka på NULL, vilket innebär att de inte pekar på någon
* specifik minnesadress.
*************************************************************************************/
void MemoryBlock_delete(union MemoryBlock* self, enum DataType* dataType)
{
	if (*dataType == INT)
	{
		free((*self).integer);
	}
	
	else if (*dataType == DOUBLE)
	{
		free((*self).decimal);
	}
	
	else if (*dataType == SIZE_T)
	{
		free((*self).natural);
	}
	
	else if (*dataType == CHAR)
	{
		free((*self).character);
	}
	
	(*self).integer = NULL;
	(*self).decimal = NULL;
	(*self).natural = NULL;
	(*self).character = NULL;
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_resize används för att ändra storleken på ett dynamiskt
* fält via omallokering, implementerat via ett objekt av unionen MemoryBlock.
* Ingående argument dataType indikerar objektets aktiva datatyp, där motsvarande
* pekare integer, decimal, natural eller character pekar på aktuellt fält. Ingående
* argument new_size utgörs av fältets nya storlek efter omallokeringen.
*
* Utefter aktiv datatyp så sker omallokering av aktuellt fält via anrop av funktionen
* realloc. För att inte förlora adressen och därmed tillgången till befintligt fält
* ifall omallokeringen misslyckas, så skapas ett kopiafält som en pekare döpt copy
* pekar på, där befintligt innehåll kopieras över. Vid lyckad minnesallokering så
* sätts motsvarande pekare integer, decimal, natural eller character till att peka
* på det nya fält som pekaren copy pekar på. Ifall minnesallokeringen lyckas så
* returneras true. Vid misslyckas minnesallokering, vilket indikeras genom att
* pekaren copy inte pekar på någon minnesadress, så returneras i stället false.
*************************************************************************************/
enum bool MemoryBlock_resize(union MemoryBlock* self, enum DataType* dataType, size_t* new_size)
{
	if (*dataType == INT)
	{
		int* copy = (int*)realloc((*self).integer, sizeof(int) * *new_size);
		if (!copy) return false;
		(*self).integer = copy;
		return true;
	}

	else if (*dataType == DOUBLE)
	{
		double* copy = (double*)realloc((*self).decimal, sizeof(double) * *new_size);
		if (!copy) return false;
		(*self).decimal = copy;
		return true;
	}

	else if (*dataType == SIZE_T)
	{
		size_t* copy = (size_t*)realloc((*self).natural, sizeof(size_t) * *new_size);
		if (!copy) return false;
		(*self).natural = copy;
		return true;
	}

	else if (*dataType == CHAR)
	{
		char* copy = (char*)realloc((*self).character, sizeof(char) * *new_size);
		if (!copy) return false;
		(*self).character = copy;
		return true;
	}

	return false;
}

/*************************************************************************************
* Funktionen MemoryBlock_assign används för att tilldela ett värde på ett specifikt
* index i ett dynamiskt fält, som en pekare tillhörande ett objekt av unionen
* MemoryBlock pekar på. Ingående argument dataType indikerar objekts aktiva datatyp,
* index indikerar på vilket index som tilldelning skall genomföras och data utgör
* en pekare till det innehåll som skall skrivas till specificerat index.
*
* Utefter aktiv datatyp så tilldelas innehållet till befintligt fält, där voidpekaren
* data typomvandlas till en pekare av aktiverad datatyp, exempelvis (int*)(data) för
* att omvandla från en voidpekare till en intpekare ifall aktuell datatyp är int.
* Därefter genomförs en dereferens för att komma åt innehållet som data pekar på.
* Till exempel, om aktuell datatyp är int och voidpekaren data har omvandlats till
* en intpekare, så sker dereferens genom att placera en asterisk framför data. Därmed
* skrivs *(int*)(data) för att först typomvandla data till en intpekare och sedan
* komma åt innehållet som data pekar på, där asterisken framför typomvandlingen
* medför dereferensen.
*************************************************************************************/
void MemoryBlock_assign(union MemoryBlock* self, enum DataType* dataType, 
						size_t* index, void* data)
{
	if (*dataType == INT)
	{
		(*self).integer[*index] = *(int*)(data);
	}
	
	else if (*dataType == DOUBLE)
	{
		(*self).decimal[*index] = *(double*)(data);
	}
	
	else if (*dataType == SIZE_T)
	{
		(*self).natural[*index] = *(size_t*)(data);
	}
	
	else if (*dataType == CHAR)
	{
		(*self).character[*index] = *(char*)(data);
	}
	
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_set används för att skriva över innehållet på ett specifikt
* index i en dynamisk array, implementerat via ett objekt av unionen MemoryBlock.
* Ingående argument dataType indikerar fältets datatyp, index utgör index till det
* elements som skall skrivas över och data utgör en pekare till det element som
* skall skrivas till aktuellt index. Beroende på aktuell datatyp så skrivs aktuell
* data som pekaren data pekar på till specificerat index. För att kunna lägga till
* datan så typomvandlas voidpekaren data först till en pekare av aktuell datatyp,
* följt av en dereferens för att komma åt innehållet som pekaren pekar på. Därmed
* genomförs exempelvis operationen *(int*)(data) för att typomvandla voidpekaren
* data till en intpekare och sedan komma åt innehållet som denna pekar på ifall
* aktuell datatyp är int. Samma princip används även för övriga datatyper.
*************************************************************************************/
void MemoryBlock_set(union MemoryBlock* self, enum DataType* dataType, 
					 size_t* index, void* data)
{
	if (*dataType == INT)
	{
		(*self).integer[*index] = *(int*)(data);
	}
	
	else if (*dataType == DOUBLE)
	{
		(*self).decimal[*index] = *(double*)(data);
	}
	
	else if (*dataType == SIZE_T)
	{
		(*self).natural[*index] = *(size_t*)(data);
	}
	
	else if (*dataType == CHAR)
	{
		(*self).character[*index] = *(char*)(data);
	}
	
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_at används för att läsa innehållet på ett givet index i
* ett dynamiskt fält, implementerat via ett objekt av unionen MemoryBlock.
* Utefter aktuell datatyp så returneras adressen till aktuellt element, vilket
* medför att en pekare till elementet i fråga returneras.
*
* Eftersom fältets datatyp varierar så passeras en voidpekare till aktuellt element,
* vilket medför att innehållet efter läsning måste typomvandlas till korrekt datatyp,
* följt av en dereferens. Som exempel, om datatypen på ett fält döpt data är int och
* elementet på index 5 skall hämtas och lagras i en variabel döpt number så bör
* följande operation genomföras:
* int number = *(int*)(MemoryBlock_at(&data, INT, 5);
*************************************************************************************/
void* MemoryBlock_at(union MemoryBlock* self, enum DataType* dataType, size_t* index)
{
	if (dataType == INT)
	{
		return &(*self).integer[*index];
	}
	
	else if (*dataType == DOUBLE)
	{
		return &((*self).decimal[*index]);
	}
	
	else if (*dataType == SIZE_T)
	{
		return &((*self).natural[*index]);
	}
	
	else if (*dataType == CHAR)
	{
		return &((*self).character[*index]);
	}
	
	return NULL;
}

/*************************************************************************************
* Funktionen MemoryBlock_print används för att skriva ut innehållet i ett dynamiskt
* fält, implementerat via ett objekt av unionen MemoryBlock. Ingående argument
* dataType indikerar fältets datatyp, medan size indikerar dess storlek. Om listan
* är tom avslutas funktionen direkt. Annars skrivs varje element ur fältet ut, där
* tecken skrivs ut på en enda rad för att implementera en sträng innehållande ett
* textstycke, medan tal skrivs ut på var sin rad. Utefter aktuell datatyp används
* lämplig formatspecificerare, där c% används för char, %d för int, %g för double
* och %zu för size_t. Utskrift sker via seriell överföring, främst via funktioner
* serial_print, serial_print_integer samt serial_print_unsigned. Innen utskrift
* påbörjas så initieras seriell överföring via anrop av funktionen init_serial.
*************************************************************************************/
void MemoryBlock_print(union MemoryBlock* self, enum DataType* dataType, size_t* size)
{
	if (!(*size)) return;
	
	init_serial();

	if (*dataType == CHAR)
	{
		register size_t i;
		
		for (i = 0; i < *size; i++)
		{
			unsigned long character = (unsigned long)((*self).character[i]);
			serial_print_unsigned("%c", &character);
		}
		
		if ((*self).character[*size - 1] != '\n')
		{
			serial_print("\n");
		}
		
		return;
	}

	serial_print("-----------------------------------------------------------------------\n");
	
	register size_t i;
	
	for (i = 0; i < *size; i++)
	{
		if (*dataType == INT)
		{
			long integer = (long)((*self).integer[i]);
			serial_print_integer("%d\n", &integer);
		}
		
		else if (*dataType == DOUBLE)
		{
			long rounded_number = (long)((*self).decimal[i] + 0.5);
			serial_print_integer("%g\n", &rounded_number);
		}
		
		else if (*dataType == SIZE_T)
		{
			unsigned long number = (unsigned long)((*self).natural[i]);
			serial_print_unsigned("%zu\n", &number);
		}
	}

	serial_print("-----------------------------------------------------------------------\n\n");
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_insert används för att lägga in ett element i en dynamisk
* array, där efterföljande element flyttas ett steg framåt. Innan denna funktion
* anropas så måste aktuellt fält ha omallokerats för att rymma ytterligare ett
* element, annars kommer inte samtliga element få plats i fältet. Det dynamiska
* fältet är implementerat via ett objekt av unionen MemoryBlock. Ingående argument
* dataType indikerar fältets datatyp, index indikerar på vilket index det nya
* elementet skall placeras, size indikerar fältets storlek och data utgör en
* pekare till det element som skall läggas till.
*
* Utefter aktuell datatyp så flyttas befintliga element som skall placeras bakom
* det nya elementet ett steg framåt, vilket realiseras via en for-sats. Slutligen
* placeras det nya elementet på specificerat index. För att komma åt innehållet
* som voidpekaren data pekar på, så typomvandlas denna först till en pekare av
* aktuell datatyp, följt av en dereferens för att komma åt det innehåll som denna
* pekar på. Som exempel, om aktuell datatyp är int, så typomvandlas först data
* till en intpekare via en explicit typomvandling (int*)(data), följt av en derefens
* via en asterisk framför denna typomvandling. Därmed skrivs *(int*)(data) för att
* först typomvandla till en pekare av aktuell datatyp, följt av en dereferens för
* att komma åt innehållet och tilldela detta till specicerat index i fältet.
*************************************************************************************/
void MemoryBlock_insert(union MemoryBlock* self, enum DataType* dataType, 
						size_t* index, size_t* size, void* data)
{
	register size_t i;
	
	if (*dataType == INT)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).integer[i] = (*self).integer[i - 1];
		}
		
		(*self).integer[*index] = *(int*)(data);
	}

	else if (*dataType == DOUBLE)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).decimal[i] = (*self).decimal[i - 1];
		}
		
		(*self).decimal[*index] = *(double*)(data);
	}
	
	else if (*dataType == SIZE_T)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).natural[i] = (*self).natural[i - 1];
		}
		
		(*self).natural[*index] = *(size_t*)(data);
	}

	else if (*dataType == CHAR)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).character[i] = (*self).character[i - 1];
		}
		
		(*self).character[*index] = *(char*)(data);
	}

	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_erase används för att ta bort ett element i en dynamisk
* array, där efterföljande element flyttas ett steg bakåt och därigenom skriver
* över elementet som skall tas bort. Efter att denna funktion har exekverat så bör
* aktuellt fält omallokeras för att rymma ett element mindre, då fältet annars
* rymmer ett element mer än vad arrayen skall innehålla. Det dynamiska fältet är
* implementerat via ett objekt av unionen MemoryBlock. Ingående argument dataType
* indikerar fältets datatyp, index indikerar på vilket index det nya elementet
* skall placeras och size indikerar fältets storlek.
*
* Utefter aktuell datatyp så flyttas samtliga efterföljande element ett steg bakåt,
* vilket realiseras via en for-sats. Därmed skrivs aktuellt element över, samtidigt
* som det sista elementet i arrayen förekommer på de två sista adresserna i arrayen,
* eftersom detta kopieras från befintligt sista index till föregående index utan att
* skrivas över. Därmed bör fältet sedan omallokeras till att rymma ett element
* mindre via anrop av funktionen MemoryBlock_resize, där det sista elementet i
* arrayen raderas.
*************************************************************************************/
void MemoryBlock_erase(union MemoryBlock* self, enum DataType* dataType, 
					   size_t* index, size_t* size)
{
	register size_t i;
	
	if (*dataType == INT)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).integer[i] = (*self).integer[i + 1];
		}
	}

	else if (*dataType == DOUBLE)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).decimal[i] = (*self).decimal[i + 1];
		}
	}

	else if (*dataType == SIZE_T)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).natural[i] = (*self).natural[i + 1];
		}
	}

	else if (*dataType == CHAR)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).character[i] = (*self).character[i + 1];
		}
	}

	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_sort används för att sortera innehållet i ett dynamiskt fält
* i stigande ordning, där det minsta talet placeras först och det största talet
* placeras sist. Ingående argument dataType indikerar fältets datatyp, medan size
* indikerar dess storlek. Iteration sker genom fältet via nästlade for-satser.
* Den yttre iteratorn i används för att iterera från första till näst sista element
* i fältet. Via den inre iteratorn j så jämförs elementet på index i mot samtliga
* efterföljande element, som nås via iterator j. Ifall elementet på index i är
* större än elementet på index j så byts elementen mot varandra. Detta genomförs
* genom att innehållet på index i temporärt lagras via en variabel döpt temp.
* Sedan skrivs innehållet på index j till index i, följt av att innehållet i
* variabeln temp skrivs till index j. Via likartade jämförelser och byten mellan
* alla element så sorteras fältet.
*************************************************************************************/
void MemoryBlock_sort(union MemoryBlock* self, enum DataType* dataType, size_t* size)
{
	register size_t i, j;
	
	if (*dataType == INT)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).integer[i]) > ((*self).integer[j]))
				{
					int temp = (*self).integer[i];
					(*self).integer[i] = (*self).integer[j];
					(*self).integer[j] = temp;
				}
			}
		}
	}

	else if (*dataType == DOUBLE)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).decimal[i]) > ((*self).decimal[j]))
				{
					double temp = (*self).decimal[i];
					(*self).decimal[i] = (*self).decimal[j];
					(*self).decimal[j] = temp;
				}
			}
		}
	}

	else if (*dataType == SIZE_T)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).natural[i]) > ((*self).natural[j]))
				{
					size_t temp = (*self).natural[i];
					(*self).natural[i] = (*self).natural[j];
					(*self).natural[j] = temp;
				}
			}
		}
	}

	else if (*dataType == CHAR)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).character[i]) > ((*self).character[j]))
				{
					char temp = (*self).character[i];
					(*self).character[i] = (*self).character[j];
					(*self).character[j] = temp;
				}
			}
		}
	}

	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_sort_reverse används för att sortera innehållet i ett
* dynamiskt fält i fallande ordning, där det största talet placeras först och det
* minsta talet placeras sist. Ingående argument dataType indikerar fältets datatyp,
* medan size indikerar dess storlek. Iteration sker genom fältet via implementering
* av nästlade for-satser samt två iteratorer i och j. Den yttre iteratorn i används
* för att iterera från första till näst sista element i fältet. Via inre iterator j
* så jämförs elementet på index i mot samtliga efterföljande element. Ifall elementet
* på index i är mindre än elementet på index j så byts elementen mot varandra. Detta
* åstadkommes genom att innehållet på index i temporärt lagras via en variabel döpt
* temp. Sedan skrivs innehållet på index j till index i, följt av att innehållet
* lagrat av variabeln temp skrivs till index j. Via likartade jämförelser och byten
* mellan samtliga element så sorteras fältet.
*************************************************************************************/
void MemoryBlock_sort_reverse(union MemoryBlock* self, enum DataType* dataType, size_t* size)
{
	register size_t i, j;
	
	if (*dataType == INT)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).integer[i]) < ((*self).integer[j]))
				{
					int temp = (*self).integer[i];
					(*self).integer[i] = (*self).integer[j];
					(*self).integer[j] = temp;
				}
			}
		}
	}

	else if (*dataType == DOUBLE)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).decimal[i]) < ((*self).decimal[j]))
				{
					double temp = (*self).decimal[i];
					(*self).decimal[i] = (*self).decimal[j];
					(*self).decimal[j] = temp;
				}
			}
		}
	}

	else if (*dataType == SIZE_T)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).natural[i]) < ((*self).natural[j]))
				{
					size_t temp = (*self).natural[i];
					(*self).natural[i] = (*self).natural[j];
					(*self).natural[j] = temp;
				}
			}
		}
	}

	else if (*dataType == CHAR)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).character[i]) < ((*self).character[j]))
				{
					char temp = (*self).character[i];
					(*self).character[i] = (*self).character[j];
					(*self).character[j] = temp;
				}
			}
		}
	}

	return;
}
