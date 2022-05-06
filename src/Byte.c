/* Inkluderingsdirektiv: */
#include "Byte.h"

/******************************************************************************
* Funktionen new_Byte används för att deklarera samt initiera ett objekt av
* unionen Byte, som används för att läsa och skriva till 8-bitars register,
* samtidigt som enstaka bitar enkelt kan ett- eller nollställas samt läsas av.
* Ingående argument data utgörs av objektets startvärde, som tilldelas till
* unionens 8-bitars medlem data, vilket medför att de enskilda åtta bitarna
* uppdateras till detta värde på binär form. Först deklareras ett nytt objekt 
* av unionen i fråga döpt self. Därefter initieras instansvariablerna. 
* Först initieras samtliga bitar till startvärdet 0 via anrop av funktionen
* new_Bits. Därefter tilldelas startvärdet till instansvariabeln data via
* tilldelning av ingående argument med samma namn. Denna tilldelning medför
* att de enskilda bitarna tilldelas detta värde på binär form. Därefter
* returneras objektet vid återhoppet, nu redo att användas.
******************************************************************************/
union Byte new_Byte(unsigned char data)
{
	union Byte self;
	self.bits = new_Bits();
	self.data = data;
	return self;
}

/******************************************************************************
* Funktionen new_Bits utgör initieringsrutin för objekt av strukten Bits,
* som utgör enskilda bitar till ett 8-bitars register. Först skapas ett objekt
* av strukten i fråga, som döps till self, följt av att samtliga bitar 
* tilldelas startvärdet 0. Därefter returneras objekt, nu redo att användas.
******************************************************************************/
struct Bits new_Bits(void)
{
	struct Bits self;
	self.bit0 = 0;
	self.bit1 = 0;
	self.bit2 = 0;
	self.bit3 = 0;
	self.bit4 = 0;
	self.bit5 = 0;
	self.bit6 = 0;
	self.bit7 = 0;
	return self;
}